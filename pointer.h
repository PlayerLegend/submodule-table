#ifndef FLAT_INCLUDES
#include <stddef.h>
#include <string.h>
#define FLAT_INCLUDES
#include "../range/def.h"
#endif

#define map_pointer_pair_def(pointer_type, value_type) { pointer_type ref; value_type value; }
#define map_pointer_link_def(link_type, pair_type) { link_type * peer; pair_type child; }
#define map_pointer_table_def(link_type) { size_t link_count; struct range(link_type*); }

#define map_pointer_type_declare(prefix)		\
    typedef struct prefix##_pair prefix##_pair;				\
    typedef struct prefix##_link prefix##_link;				\
    typedef struct prefix##_table prefix##_table;

#define map_pointer_type_define(prefix, key_type, ...)			\
    typedef __VA_ARGS__ prefix##_value;					\
    typedef key_type prefix##_key;					\
									\
    struct prefix##_pair map_pointer_pair_def(key_type, prefix##_value); \
    struct prefix##_link map_pointer_link_def(prefix##_link, prefix##_pair); \
    struct prefix##_table map_pointer_table_def(prefix##_link);

map_pointer_type_declare(map_pointer_base);
map_pointer_type_define(map_pointer_base, void*, struct {});

map_pointer_base_link ** map_pointer_base_seek(map_pointer_base_table * haystack, void * needle);
map_pointer_base_pair * map_pointer_base_include(map_pointer_base_table * haystack, void * needle, size_t link_size);
void map_pointer_base_resize(map_pointer_base_table * haystack, size_t size);
inline static map_pointer_base_link * map_pointer_base_unlink (map_pointer_base_link ** target)
{
    map_pointer_base_link * retval = *target;
    *target = (*target)->peer;
    return retval;
}

#define map_pointer_function_define(prefix)					\
    inline static prefix##_link ** prefix##_seek(prefix##_table * haystack, prefix##_key needle) \
    {									\
	return (void*) map_pointer_base_seek((void*)haystack, (void*) needle); \
    }									\
									\
    inline static prefix##_pair * prefix##_include(prefix##_table * haystack, prefix##_key needle) \
    {									\
	return (void*) map_pointer_base_include((void*)haystack, (void*) needle, sizeof(prefix##_link)); \
    }									\
									\
    inline static void prefix##_resize(prefix##_table * haystack, size_t size) \
    {									\
	map_pointer_base_resize((void*) haystack, size);		\
    }									\
									\
    inline static prefix##_link * prefix##_unlink(prefix##_link ** target) \
    {									\
	return (void*) map_pointer_base_unlink((void*) target);		\
    }

#define map_pointer_def(prefix, key_type, ...)		\
    map_pointer_type_declare(prefix);			    \
    map_pointer_type_define(prefix, key_type, __VA_ARGS__); \
    map_pointer_function_define(prefix);

#define for_map_pointer(link, bucket, table)	\
    for_range(bucket, table) for (link = *bucket; link; link = link->peer)
#
