#ifndef FLAT_INCLUDES
#include <stdlib.h>
#include "../range/def.h"
#endif

typedef size_t map_string_digest;
typedef struct { union { const char * string; const range_const_char range; const range_const_unsigned_char bytes; range_char _internal_range; } key; map_string_digest digest; } map_string_query;

#define map_string_pair_def(map_value) { map_string_query query; map_value value; }
#define map_string_link_def(map_link, map_pair) { map_link * peer; map_pair child; }
#define map_string_map_def(map_link) { size_t link_count; struct range(map_link*); }

#define map_string_type_declare(prefix)					\
    typedef struct prefix##_pair prefix##_pair;				\
    typedef struct prefix##_link prefix##_link;				\
    typedef struct prefix##_table prefix##_table;			\

#define map_string_type_define(prefix)	\
    struct prefix##_pair map_string_pair_def(prefix##_value);		\
    struct prefix##_link map_string_link_def(prefix##_link, prefix##_pair); \
    struct prefix##_table map_string_map_def(prefix##_link);

map_string_type_declare(map_string_base);
typedef struct {} map_string_base_value;
map_string_type_define(map_string_base);

inline static map_string_base_link * map_string_base_unlink (map_string_base_link ** target)
{
    map_string_base_link * retval = *target;
    *target = (*target)->peer;
    return retval;
}

map_string_base_link ** map_string_base_seek_query(map_string_base_table * haystack, const map_string_query * needle);
map_string_base_link ** map_string_base_seek_range(map_string_base_table * haystack, const range_const_char * needle);
map_string_base_link ** map_string_base_seek_string(map_string_base_table * haystack, const char * needle);
map_string_base_pair * map_string_base_include_range(map_string_base_table * haystack, const range_const_char * needle, size_t link_size);
map_string_base_pair * map_string_base_include_query(map_string_base_table * haystack, const map_string_query * needle, size_t link_size);
map_string_base_pair * map_string_base_include_string(map_string_base_table * haystack, const char * needle, size_t link_size);
void map_string_base_resize(map_string_base_table * haystack, size_t size);
map_string_base_pair * map_string_base_include_string(map_string_base_table * haystack, const char * needle, size_t link_size);
#define map_string_query_to_pair(pairtype, query) ((pairtype*)(query))

#define map_string_function_declare(prefix)				\
    inline static prefix##_link * prefix##_unlink(prefix##_link ** target); \
    inline static prefix##_link ** prefix##_seek_query (prefix##_table * haystack, const map_string_query * needle); \
    inline static prefix##_link ** prefix##_seek_range (prefix##_table * haystack, const range_const_char * needle); \
    inline static prefix##_link ** prefix##_seek_string (prefix##_table * haystack, const char * needle); \
    inline static prefix##_pair * prefix##_include_query (prefix##_table * haystack, map_string_query * needle); \
    inline static prefix##_pair * prefix##_include_range (prefix##_table * haystack, const range_const_char * needle); \
    inline static prefix##_pair * prefix##_include_string (prefix##_table * haystack, const char * needle); \
    inline static void prefix##_resize (prefix##_table * haystack, size_t size); \
    inline static void prefix##_table_clear (prefix##_table * target);	\

#define map_string_function_define(prefix)				\
    inline static prefix##_link * prefix##_unlink(prefix##_link ** target) \
    {									\
	return (prefix##_link*) map_string_base_unlink ((void*)target);		\
    }									\
									\
    inline static prefix##_link ** prefix##_seek_query (prefix##_table * haystack, const map_string_query * needle) \
    {									\
	return (prefix##_link**) map_string_base_seek_query((void*) haystack, (void*) needle); \
    }									\
									\
    inline static prefix##_link ** prefix##_seek_range (prefix##_table * haystack, const range_const_char * needle) \
    {									\
	return (prefix##_link**) map_string_base_seek_range((void*)haystack, needle); \
    }									\
									\
    inline static prefix##_link ** prefix##_seek_string (prefix##_table * haystack, const char * needle) \
    {									\
	return (prefix##_link**)map_string_base_seek_string((void*)haystack, needle); \
    }									\
									\
    inline static prefix##_pair * prefix##_lookup_query (const prefix##_table * haystack, const map_string_query * needle) \
    {									\
	prefix##_link * link = *prefix##_seek_query((prefix##_table*)haystack, needle); \
	return link ? &link->child : NULL;				\
    }									\
									\
    inline static prefix##_pair * prefix##_lookup_range (const prefix##_table * haystack, const range_const_char * needle) \
    {									\
	prefix##_link * link = *prefix##_seek_range((prefix##_table*)haystack, needle); \
	return link ? &link->child : NULL;				\
    }									\
									\
    inline static prefix##_pair * prefix##_lookup_string (const prefix##_table * haystack, const char * needle) \
    {									\
	prefix##_link * link = *prefix##_seek_string((void*)haystack, needle); \
	return link ? &link->child : NULL;				\
    }									\
									\
    inline static prefix##_pair * prefix##_include_query (prefix##_table * haystack, map_string_query * needle) \
    {									\
	return (prefix##_pair*)map_string_base_include_query((void*)haystack, needle, sizeof(prefix##_link)); \
    }									\
									\
    inline static prefix##_pair * prefix##_include_range (prefix##_table * haystack, const range_const_char * needle) \
    {									\
	return (prefix##_pair*)map_string_base_include_range((void*)haystack, needle, sizeof(prefix##_link)); \
    }									\
									\
    inline static prefix##_pair * prefix##_include_string (prefix##_table * haystack, const char * needle) \
    {									\
	return (prefix##_pair*)map_string_base_include_string((void*)haystack, needle, sizeof(prefix##_link)); \
    }									\
									\
    inline static void prefix##_resize (prefix##_table * haystack, size_t size) \
    {									\
	map_string_base_resize ((void*)haystack, size);			\
    };									\
									\
    inline static void prefix##_table_clear (prefix##_table * target)	\
    {									\
	prefix##_link ** bucket;						\
	prefix##_link * remove;						\
	for_range(bucket, *target)					\
	{								\
	    while ( (remove = *bucket) )				\
	    {								\
		*bucket = remove->peer;					\
		prefix##_value_clear(&remove->child.value);		\
		free (remove->child.query.key._internal_range.begin);	\
		free (remove);						\
	    }								\
	}								\
	free(target->begin);						\
    }									\
									\
    inline static void prefix##_table_free (prefix##_table * target)	\
    {									\
	prefix##_table_clear(target);					\
	free(target);							\
    }									\
									\
    inline static void prefix##_value_free (prefix##_value * target)	\
    {									\
	prefix##_value_clear(target);					\
	free(target);							\
    }									\

#define map_string_define(prefix, ...)		\
    map_string_type_declare(prefix);			\
    typedef __VA_ARGS__ prefix##_value;			\
    inline static void prefix##_value_clear(prefix##_value * target) {};	\
    map_string_type_define(prefix);	\
    map_string_function_declare(prefix);	\
    map_string_function_define(prefix);

map_string_define(string, struct{});
