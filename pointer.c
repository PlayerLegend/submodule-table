#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "pointer.h"

#include "../range/alloc.h"

#include "../log/log.h"
#include <stdbool.h>

static bool probably_prime(unsigned int n)
{
    unsigned int test_max = n / 10;

    if (n & 1)
    {
	for (unsigned int test = 3; test <= test_max; test += 2)
	{
	    if (n % test == 0)
	    {
		return false;
	    }
	}

	return true;
    }
    else
    {
	return n == 2;
    }
}

map_pointer_base_link ** map_pointer_base_seek(map_pointer_base_table * haystack, void * needle)
{
    size_t size = range_count(*haystack);
    
    if (80 * haystack->link_count >= 100 * size)
    {
	switch (size)
	{
	case 0:
	    size = 1;
	    break;

	case 1:
	    size = 7;
	    break;

	case 8:
	    size = 59;
	    break;

	default:
	    size = 2 * size + 1031;

	    for (int i = 0; i < 10; i++)
	    {
		size += 2;

		if (probably_prime(size))
		{
		    break;
		}
	    }
	    
	    break;
	}
	    
	map_pointer_base_resize(haystack, size);
    }
    
    map_pointer_base_link ** retval = haystack->begin + ((uintptr_t)needle) % range_count(*haystack);

    map_pointer_base_link * check;

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

map_pointer_base_pair * map_pointer_base_include(map_pointer_base_table * haystack, void * needle, size_t link_size)
{
    map_pointer_base_link ** link = map_pointer_base_seek(haystack, needle);

    if (*link)
    {
	return &(*link)->child;
    }
    else
    {
	map_pointer_base_link * new = calloc (1, link_size);

	new->child.ref = needle;
	
	new->peer = *link;
	*link = new;

	haystack->link_count++;
	
	return &new->child;
    }
}

void map_pointer_base_resize(map_pointer_base_table * haystack, size_t size)
{
    map_pointer_base_table old_table = *haystack;

    range_calloc(*haystack, size);
    
    map_pointer_base_link ** src;

    map_pointer_base_link * move;

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
