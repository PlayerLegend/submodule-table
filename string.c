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

void map_string_to_none_resize(map_string_to_none_table * haystack, size_t size)
{
    map_string_to_none_table old_table = *haystack;

    range_calloc(*haystack, size);
    
    map_string_to_none_link ** src;

    map_string_to_none_link * move;

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

map_string_to_none_link ** map_string_to_none_seek(map_string_to_none_table * haystack, map_string_query * needle)
{
    map_string_to_none_link ** retval = haystack->begin + needle->digest % range_count(*haystack);

    map_string_to_none_link * check;
    
    while ( (check = *retval) )
    {
	if (check->child.query.digest == needle->digest && range_streq (&check->child.query.key, &needle->key))
	{
	    return retval;
	}

	retval = &check->peer;
    }

    return retval;
}

#define map_string_to_none_query_init(needle) (map_string_query){ .digest = sdbm_hash_range(needle), .key = *needle };

map_string_to_none_pair * map_string_to_none_include(map_string_to_none_table * haystack, const range_const_char * needle)
{
    assert (haystack->link_new);
    
    if (80 * haystack->link_count >= 100 * (size_t)range_count(*haystack))
    {
	map_string_to_none_resize(haystack, 2 * range_count(*haystack) + 1031);
    }
    
    map_string_query query = map_string_to_none_query_init(needle);

    map_string_to_none_link ** link = map_string_to_none_seek (haystack, &query);

    if (*link)
    {
	return &(*link)->child;
    }
    else
    {
	map_string_to_none_link * new = haystack->link_new(haystack);

	new->child.query.digest = query.digest;
	range_strdup(&new->child.query._key, needle);
	
	new->peer = *link;
	*link = new;

	haystack->link_count++;

	return &new->child;
    }
}

map_string_to_none_pair * map_string_to_none_include_string(map_string_to_none_table * haystack, const char * needle)
{
    range_const_char needle_range = { .begin = needle, .end = needle + strlen(needle) };
    return map_string_to_none_include(haystack, &needle_range);
}
