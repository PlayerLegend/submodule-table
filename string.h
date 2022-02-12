#ifndef FLAT_INCLUDES
#include <stddef.h>
#include <string.h>
#define FLAT_INCLUDES
#include "../range/def.h"
#endif

typedef size_t table_string_digest;
typedef struct { union { range_const_char key; range_char _key; }; table_string_digest digest; } table_string_query;
typedef void * (*table_string_link_new)(void * table);

// host

#define host_string_pair_def(host_value) { table_string_query query; host_value value; }
#define host_string_link_def(host_link, host_pair) { host_link * peer; host_pair child; }
#define host_string_table_def(host_link) { table_string_link_new link_new; size_t link_count; struct range(host_link*); }

typedef struct {} host_string_to_none_value;
typedef struct host_string_pair_def(host_string_to_none_value) host_string_to_none_pair;
typedef struct host_string_to_none_link host_string_link_def(struct host_string_to_none_link, host_string_to_none_pair) host_string_to_none_link;
typedef struct host_string_table_def(host_string_to_none_link) host_string_to_none_table;

inline static host_string_to_none_link * host_string_unlink (host_string_to_none_link ** target)
{
    host_string_to_none_link * retval = *target;
    *target = (*target)->peer;
    return retval;
}

host_string_to_none_link ** host_string_to_none_seek(host_string_to_none_table * haystack, table_string_query * needle);
host_string_to_none_pair * host_string_to_none_include(host_string_to_none_table * haystack, const range_const_char * needle);
void host_string_to_none_resize(host_string_to_none_table * haystack, size_t size);
host_string_to_none_pair * host_string_to_none_include_string(host_string_to_none_table * haystack, const char * needle);
#define table_string_query_to_pair(pairtype, query) ((pairtype*)(query))

#define host_string_map_typedef(name)			\
    typedef union host_string_to_##name##_pair host_string_to_##name##_pair; \
    typedef union host_string_to_##name##_link host_string_to_##name##_link; \
    typedef union host_string_to_##name##_table host_string_to_##name##_table; \

#define host_string_map_define(name, ...)				\
    typedef __VA_ARGS__ host_string_to_##name##_value;			\
    union host_string_to_##name##_pair { struct host_string_pair_def(host_string_to_##name##_value); host_string_to_none_pair none_cast; }; \
    union host_string_to_##name##_link { struct host_string_link_def(union host_string_to_##name##_link, host_string_to_##name##_pair); host_string_to_none_link none_cast; }; \
    union host_string_to_##name##_table { struct host_string_table_def(host_string_to_##name##_link); host_string_to_none_table none_cast; }; \
    inline static host_string_to_##name##_link * host_string_to_##name##_unlink(host_string_to_##name##_link ** target) { return (void*) host_string_unlink ((void*)target); } \
    inline static host_string_to_##name##_link ** host_string_to_##name##_seek (host_string_to_##name##_table * haystack, table_string_query * needle) { return (void*) host_string_to_none_seek((void*) haystack, (void*) needle); } \
    inline static host_string_to_##name##_pair * host_string_to_##name##_include (host_string_to_##name##_table * haystack, const range_const_char * needle) { return (void*) host_string_to_none_include((void*)haystack, needle); } \
    inline static host_string_to_##name##_pair * host_string_to_##name##_include_string (host_string_to_##name##_table * haystack, const char * needle) { return (void*)host_string_to_none_include_string((void*)haystack, needle); } \
    inline static void host_string_to_##name##_resize (host_string_to_##name##_table * haystack, size_t size) { host_string_to_none_resize ((void*)haystack, size); };

// ref

#define ref_string_pair_def(ref_value) { table_string_query * query; ref_value value; }
#define ref_string_link_def(ref_link, ref_pair) { ref_link * peer; ref_pair child; }
#define ref_string_table_def(ref_link) { table_string_link_new link_new; size_t link_count; struct range(ref_link*); }

typedef struct {} ref_string_to_none_value;
typedef struct ref_string_pair_def(ref_string_to_none_value) ref_string_to_none_pair;
typedef struct ref_string_to_none_link ref_string_link_def(struct ref_string_to_none_link, ref_string_to_none_pair) ref_string_to_none_link;
typedef struct ref_string_table_def(ref_string_to_none_link) ref_string_to_none_table;

inline static ref_string_to_none_link * ref_string_to_none_unlink (ref_string_to_none_link ** target)
{
    ref_string_to_none_link * retval = *target;
    *target = (*target)->peer;
    return retval;
}

ref_string_to_none_link ** ref_string_to_none_seek(ref_string_to_none_table * haystack, table_string_query * needle);
ref_string_to_none_pair * ref_string_to_none_include(ref_string_to_none_table * haystack, table_string_query * needle);
void ref_string_to_none_resize(ref_string_to_none_table * haystack, size_t size);

#define ref_string_map_typedef(name, ...)		\
    typedef union ref_string_to_##name##_pair ref_string_to_##name##_pair;				\
    typedef union ref_string_to_##name##_link ref_string_to_##name##_link; \
    typedef union ref_string_to_##name##_table ref_string_to_##name##_table; \

#define ref_string_map_define(name, ...)				\
    typedef __VA_ARGS__ ref_string_to_##name##_value;			\
    union ref_string_to_##name##_pair { struct ref_string_pair_def(ref_string_to_##name##_value); ref_string_to_none_pair none_cast; }; \
    union ref_string_to_##name##_link { struct ref_string_link_def(union ref_string_to_##name##_link, ref_string_to_##name##_pair); ref_string_to_none_link none_cast; }; \
    union ref_string_to_##name##_table { struct ref_string_table_def(ref_string_to_##name##_link); ref_string_to_none_table none_cast; }; \
    inline static ref_string_to_##name##_link * ref_string_to_##name##_unlink(ref_string_to_##name##_link ** target) { return (void*) ref_string_to_none_unlink ((void*)target); } \
    inline static ref_string_to_##name##_link ** ref_string_to_##name##_seek (ref_string_to_##name##_table * haystack, table_string_query * needle) { return (void*) ref_string_to_none_seek((void*) haystack, (void*) needle); } \
    inline static ref_string_to_##name##_pair * ref_string_to_##name##_include (ref_string_to_##name##_table * haystack, table_string_query * needle) { return (void*) ref_string_to_none_include((void*)haystack, needle); } \
    inline static void ref_string_to_##name##_resize (ref_string_to_##name##_table * haystack, size_t size) { ref_string_to_none_resize ((void*)haystack, size); };

#define table_string_link_size(table) sizeof( **(table).begin )

#define for_table_bucket(bucket, table) for_range(bucket, table)
#define for_table_bucket_link(link, bucket) for (link = *(bucket); link; link = (link)->peer)
