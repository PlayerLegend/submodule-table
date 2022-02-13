#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "pointer.h"

#include "../range/alloc.h"

map_voidp_to_none_link ** map_voidp_to_none_seek(map_voidp_to_none_table * haystack, void * needle)
{
    map_voidp_to_none_link ** retval = haystack->begin + ((uintptr_t)needle) % range_count(*haystack);

    map_voidp_to_none_link * check;

    while ( (check = *retval) )
    {
	if (check->child.ref == needle)
	{
	    return retval;
	}

	retval = &check->peer;
    }

    return retval;
}

map_voidp_to_none_pair * map_voidp_to_none_include(map_voidp_to_none_table * haystack, void * needle)
{
    assert (haystack->link_new);
    
    if (80 * haystack->link_count >= 100 * (size_t)range_count(*haystack))
    {
	map_voidp_to_none_resize(haystack, 2 * range_count(*haystack) + 1031);
    }
    
    map_voidp_to_none_link ** link = map_voidp_to_none_seek(haystack, needle);

    if (*link)
    {
	return &(*link)->child;
    }
    else
    {
	map_voidp_to_none_link * new = haystack->link_new(haystack);

	new->child.ref = needle;
	
	new->peer = *link;
	*link = new;

	haystack->link_count++;
	
	return &new->child;
    }
}

void map_voidp_to_none_resize(map_voidp_to_none_table * haystack, size_t size)
{
    map_voidp_to_none_table old_table = *haystack;

    range_calloc(*haystack, size);
    
    map_voidp_to_none_link ** src;

    map_voidp_to_none_link * move;

    for_range(src, old_table)
    {
	while (*src)
	{
	    move = *src;
	    *src = move->peer;
	    haystack->begin[(uintptr_t)move->child.ref % size] = move;
	}
    }

    free (old_table.begin);
}
