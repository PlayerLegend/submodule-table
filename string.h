#ifndef FLAT_INCLUDES
#include <stddef.h>
#include <string.h>
#define FLAT_INCLUDES
#include "../range/def.h"
#endif

typedef size_t map_string_digest;
typedef struct { union { const char * string; const range_const_char range; range_char _internal_range; } key; map_string_digest digest; } map_string_query;

#define map_string_pair_def(map_value) { map_string_query query; map_value value; }
#define map_string_link_def(map_link, map_pair) { map_link * peer; map_pair child; }
#define map_string_map_def(map_link) { size_t link_count; struct range(map_link*); }

#define map_string_typedef(prefix, ...)					\
    typedef struct prefix##_pair prefix##_pair;				\
    typedef struct prefix##_link prefix##_link;				\
    typedef struct prefix##_table prefix##_table;			\
    typedef __VA_ARGS__ prefix##_value;					\
									\
    struct prefix##_pair map_string_pair_def(prefix##_value);		\
    struct prefix##_link map_string_link_def(prefix##_link, prefix##_pair); \
    struct prefix##_table map_string_map_def(prefix##_link);

map_string_typedef(map_string_base, struct{});

inline static map_string_base_link * map_string_base_unlink (map_string_base_link ** target)
{
    map_string_base_link * retval = *target;
    *target = (*target)->peer;
    return retval;
}

map_string_base_link ** map_string_base_seek(map_string_base_table * haystack, const map_string_query * needle);
map_string_base_pair * map_string_base_include_range(map_string_base_table * haystack, const range_const_char * needle, size_t link_size);
map_string_base_pair * map_string_base_include_query(map_string_base_table * haystack, const map_string_query * needle, size_t link_size);
map_string_base_pair * map_string_base_include_string(map_string_base_table * haystack, const char * needle, size_t link_size);
void map_string_base_resize(map_string_base_table * haystack, size_t size);
map_string_base_pair * map_string_base_include_string(map_string_base_table * haystack, const char * needle, size_t link_size);
#define map_string_query_to_pair(pairtype, query) ((pairtype*)(query))

#define map_string_funcdef(prefix)					\
    inline static prefix##_link * prefix##_unlink(prefix##_link ** target) \
    {									\
	return (void*) map_string_base_unlink ((void*)target);		\
    }									\
									\
    inline static prefix##_link ** prefix##_seek (prefix##_table * haystack, const map_string_query * needle) \
    {									\
	return (void*) map_string_base_seek((void*) haystack, (void*) needle); \
    }									\
									\
    inline static prefix##_pair * prefix##_include_query (prefix##_table * haystack, map_string_query * needle) \
    {									\
	return (void*)map_string_base_include_query((void*)haystack, needle, sizeof(prefix##_link)); \
    }									\
									\
    inline static prefix##_pair * prefix##_include_range (prefix##_table * haystack, const range_const_char * needle) \
    {									\
	return (void*)map_string_base_include_range((void*)haystack, needle, sizeof(prefix##_link)); \
    }									\
									\
    inline static prefix##_pair * prefix##_include_string (prefix##_table * haystack, const char * needle) \
    {									\
	return (void*)map_string_base_include_string((void*)haystack, needle, sizeof(prefix##_link)); \
    }									\
									\
    inline static void prefix##_resize (prefix##_table * haystack, size_t size) \
    {									\
	map_string_base_resize ((void*)haystack, size);			\
    };

#define map_string_define(prefix, ...)		\
    map_string_typedef(prefix, __VA_ARGS__);	\
    map_string_funcdef(prefix);

map_string_define(string, struct{});
