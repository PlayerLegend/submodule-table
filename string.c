#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define FLAT_INCLUDES
#include "../range/def.h"
#include "../range/alloc.h"
#include "../range/string.h"
#include "string.h"
#include "../log/log.h"

static map_string_digest sdbm_hash_range(const range_const_char * src)
{
    map_string_digest digest = 0;
    
    const char * i;

    for_range(i, *src)
    {
	digest = *i + (digest << 6) + (digest << 16) - digest;
    }
    
    return digest;
}

void map_string_base_resize(map_string_base_table * haystack, size_t size)
{
    map_string_base_table old_table = *haystack;

    range_calloc(*haystack, size);
    
    map_string_base_link ** src;

    map_string_base_link * move;

    for_range(src, old_table)
    {
	while (*src)
	{
	    move = *src;
	    *src = move->peer;
	    haystack->begin[move->child.query.digest % size] = move;
	}
    }

    free (old_table.begin);
}

map_string_base_link ** map_string_base_seek_query(map_string_base_table * haystack, const map_string_query * needle)
{
    map_string_base_link ** retval = haystack->begin + needle->digest % range_count(*haystack);

    map_string_base_link * check;

    while ( (check = *retval) )
    {
	if (check->child.query.digest == needle->digest && range_streq (&check->child.query.key.range, &needle->key.range))
	{
	    return retval;
	}

	retval = &check->peer;
    }

    return retval;
}

#define map_string_base_query_init(needle) (map_string_query){ .digest = sdbm_hash_range(needle), .key.range = *needle };

map_string_base_pair * map_string_base_include_query(map_string_base_table * haystack, const map_string_query * needle, size_t link_size)
{
    if (80 * haystack->link_count >= 100 * (size_t)range_count(*haystack))
    {
	map_string_base_resize(haystack, 2 * range_count(*haystack) + 1031);
    }
    
    map_string_base_link ** link = map_string_base_seek_query (haystack, needle);

    if (*link)
    {
	return &(*link)->child;
    }
    else
    {
	map_string_base_link * new = calloc(1, link_size);

	new->child.query.digest = needle->digest;
	range_strdup(&new->child.query.key._internal_range, &needle->key.range);
	assert (range_count(needle->key.range) == range_count(new->child.query.key.range));

	new->peer = *link;
	*link = new;

	haystack->link_count++;

	return &new->child;
    }
}

map_string_base_pair * map_string_base_include_range(map_string_base_table * haystack, const range_const_char * needle, size_t link_size)
{
    map_string_query needle_query = map_string_base_query_init(needle);
    
    return map_string_base_include_query(haystack, &needle_query, link_size);
}

map_string_base_pair * map_string_base_include_string(map_string_base_table * haystack, const char * needle, size_t link_size)
{
    range_const_char needle_range;
    range_string_init(&needle_range, needle);
    assert (strlen(needle) == (size_t)range_count(needle_range));

    return map_string_base_include_range(haystack, &needle_range, link_size);
}

map_string_base_link ** map_string_base_seek_range(map_string_base_table * haystack, const range_const_char * needle)
{
    map_string_query needle_query = map_string_base_query_init(needle);

    return map_string_base_seek_query(haystack, &needle_query);
}

map_string_base_link ** map_string_base_seek_string(map_string_base_table * haystack, const char * needle)
{
    range_const_char needle_range;
    range_string_init(&needle_range, needle);
    assert (strlen(needle) == (size_t)range_count(needle_range));
    return map_string_base_seek_range(haystack, &needle_range);
}
