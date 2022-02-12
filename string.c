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

static table_string_digest sdbm_hash_range(const range_const_char * src)
{
    table_string_digest digest = 0;
    
    const char * i;

    for_range(i, *src)
    {
	digest = *i + (digest << 6) + (digest << 16) - digest;
    }
    
    return digest;
}

void host_string_to_none_resize(host_string_to_none_table * haystack, size_t size)
{
    host_string_to_none_table old_table = *haystack;

    range_calloc(*haystack, size);
    
    host_string_to_none_link ** src;

    host_string_to_none_link * move;

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

host_string_to_none_link ** host_string_to_none_seek(host_string_to_none_table * haystack, table_string_query * needle)
{
    host_string_to_none_link ** retval = haystack->begin + needle->digest % range_count(*haystack);

    host_string_to_none_link * check;
    
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

#define host_string_to_none_query_init(needle) (table_string_query){ .digest = sdbm_hash_range(needle), .key = *needle };

host_string_to_none_pair * host_string_to_none_include(host_string_to_none_table * haystack, const range_const_char * needle)
{
    assert (haystack->link_new);
    
    if (80 * haystack->link_count >= 100 * (size_t)range_count(*haystack))
    {
	host_string_to_none_resize(haystack, 2 * range_count(*haystack) + 1031);
    }
    
    table_string_query query = host_string_to_none_query_init(needle);

    host_string_to_none_link ** link = host_string_to_none_seek (haystack, &query);

    if (*link)
    {
	return &(*link)->child;
    }
    else
    {
	host_string_to_none_link * new = haystack->link_new(haystack);

	new->child.query.digest = query.digest;
	range_strdup(&new->child.query._key, needle);
	
	new->peer = *link;
	*link = new;

	haystack->link_count++;

	return &new->child;
    }
}

ref_string_to_none_link ** ref_string_to_none_seek(ref_string_to_none_table * haystack, table_string_query * needle)
{
    ref_string_to_none_link ** retval = haystack->begin + ((uintptr_t)needle) % range_count(*haystack);

    ref_string_to_none_link * check;

    while ( (check = *retval) )
    {
	if (check->child.query == needle)
	{
	    return retval;
	}

	retval = &check->peer;
    }

    return retval;
}

ref_string_to_none_pair * ref_string_to_none_include(ref_string_to_none_table * haystack, table_string_query * needle)
{
    assert (haystack->link_new);
    
    if (80 * haystack->link_count >= 100 * (size_t)range_count(*haystack))
    {
	ref_string_to_none_resize(haystack, 2 * range_count(*haystack) + 1031);
    }
    
    ref_string_to_none_link ** link = ref_string_to_none_seek(haystack, needle);

    if (*link)
    {
	return &(*link)->child;
    }
    else
    {
	ref_string_to_none_link * new = haystack->link_new(haystack);

	new->child.query = needle;
	
	new->peer = *link;
	*link = new;

	haystack->link_count++;
	
	return &new->child;
    }
}

void ref_string_to_none_resize(ref_string_to_none_table * haystack, size_t size)
{
    ref_string_to_none_table old_table = *haystack;

    range_calloc(*haystack, size);
    
    ref_string_to_none_link ** src;

    ref_string_to_none_link * move;

    for_range(src, old_table)
    {
	while (*src)
	{
	    move = *src;
	    *src = move->peer;
	    haystack->begin[(uintptr_t)move->child.query % size] = move;
	}
    }

    free (old_table.begin);
}
host_string_to_none_pair * host_string_to_none_include_string(host_string_to_none_table * haystack, const char * needle)
{
    range_const_char needle_range = { .begin = needle, .end = needle + strlen(needle) };
    return host_string_to_none_include(haystack, &needle_range);
}
