#ifndef FLAT_INCLUDES
#include <stddef.h>
#include <string.h>
#define FLAT_INCLUDES
#include "../range/def.h"
#endif

typedef size_t map_string_digest;
typedef struct { union { range_const_char key; range_char _key; }; map_string_digest digest; } map_string_query;
typedef void * (*map_string_link_new)(void * table);

#define map_string_pair_def(map_value) { map_string_query query; map_value value; }
#define map_string_link_def(map_link, map_pair) { map_link * peer; map_pair child; }
#define map_string_map_def(map_link) { map_string_link_new link_new; size_t link_count; struct range(map_link*); }

typedef struct {} map_string_to_none_value;
typedef struct map_string_pair_def(map_string_to_none_value) map_string_to_none_pair;
typedef struct map_string_to_none_link map_string_link_def(struct map_string_to_none_link, map_string_to_none_pair) map_string_to_none_link;
typedef struct map_string_map_def(map_string_to_none_link) map_string_to_none_table;

inline static map_string_to_none_link * map_string_to_none_unlink (map_string_to_none_link ** target)
{
    map_string_to_none_link * retval = *target;
    *target = (*target)->peer;
    return retval;
}

map_string_to_none_link ** map_string_to_none_seek(map_string_to_none_table * haystack, map_string_query * needle);
map_string_to_none_pair * map_string_to_none_include(map_string_to_none_table * haystack, const range_const_char * needle);
void map_string_to_none_resize(map_string_to_none_table * haystack, size_t size);
map_string_to_none_pair * map_string_to_none_include_string(map_string_to_none_table * haystack, const char * needle);
#define map_string_query_to_pair(pairtype, query) ((pairtype*)(query))

#define map_string_typedef(prefix, ...)				\
    typedef union prefix##_pair prefix##_pair;				\
    typedef union prefix##_link prefix##_link;				\
    typedef union prefix##_table prefix##_table;			\
    typedef __VA_ARGS__ prefix##_value;					\
									\
    union prefix##_pair							\
    {									\
	struct map_string_pair_def(prefix##_value); map_string_to_none_pair none_cast; \
    };									\
									\
    union prefix##_link							\
    {									\
	struct map_string_link_def(union prefix##_link, prefix##_pair); map_string_to_none_link none_cast; \
    };									\
									\
    union prefix##_table						\
    {									\
	struct map_string_map_def(prefix##_link); map_string_to_none_table none_cast; \
    };									\

#define map_string_funcdef(prefix)					\
    inline static prefix##_link * prefix##_unlink(prefix##_link ** target) \
    {									\
	return (void*) map_string_to_none_unlink ((void*)target);	\
    }									\
									\
    inline static prefix##_link ** prefix##_seek (prefix##_table * haystack, map_string_query * needle) \
    {									\
	return (void*) map_string_to_none_seek((void*) haystack, (void*) needle); \
    }									\
									\
    inline static prefix##_pair * prefix##_include (prefix##_table * haystack, const range_const_char * needle) \
    {									\
	return (void*) map_string_to_none_include((void*)haystack, needle); \
    }									\
									\
    inline static prefix##_pair * prefix##_include_string (prefix##_table * haystack, const char * needle) \
    {									\
	return (void*)map_string_to_none_include_string((void*)haystack, needle); \
    }									\
									\
    inline static void prefix##_resize (prefix##_table * haystack, size_t size) \
    {									\
	map_string_to_none_resize ((void*)haystack, size);		\
    };

#define map_string_define(prefix, ...)		\
    map_string_typedef(prefix, __VA_ARGS__);	\
    map_string_funcdef(prefix);

