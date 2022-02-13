#ifndef FLAT_INCLUDES
#include <stddef.h>
#include <string.h>
#define FLAT_INCLUDES
#include "../range/def.h"
#endif

typedef void * (*map_pointer_link_new)(void * map);

#define map_pointer_pair_def(pointer_type, value_type) { pointer_type * ref; value_type value; }
#define map_pointer_link_def(link_type, pair_type) { link_type * peer; pair_type child; }
#define map_pointer_table_def(link_type) { map_pointer_link_new link_new; size_t link_count; struct range(link_type*); }

typedef struct map_voidp_to_none_value map_voidp_to_none_value;
typedef struct map_voidp_to_none_pair map_voidp_to_none_pair;
typedef struct map_voidp_to_none_link map_voidp_to_none_link;
typedef struct map_voidp_to_none_table map_voidp_to_none_table;

struct map_voidp_to_none_value {};
struct map_voidp_to_none_pair map_pointer_pair_def(void, map_voidp_to_none_value);
struct map_voidp_to_none_link map_pointer_link_def(map_voidp_to_none_link, map_voidp_to_none_pair);
struct map_voidp_to_none_table map_pointer_table_def(map_voidp_to_none_link);

map_voidp_to_none_link ** map_voidp_to_none_seek(map_voidp_to_none_table * haystack, void * needle);
map_voidp_to_none_pair * map_voidp_to_none_include(map_voidp_to_none_table * haystack, void * needle);
void map_voidp_to_none_resize(map_voidp_to_none_table * haystack, size_t size);
inline static map_voidp_to_none_link * map_voidp_to_none_unlink (map_voidp_to_none_link ** target)
{
    map_voidp_to_none_link * retval = *target;
    *target = (*target)->peer;
    return retval;
}

#define map_pointer_typedef(prefix, key_type, ...)	\
    typedef __VA_ARGS__ prefix##_value;	\
    typedef key_type* prefix##_key;		\
    typedef union prefix##_pair prefix##_pair; \
    typedef union prefix##_link prefix##_link; \
    typedef union prefix##_table prefix##_table; \
									\
    union prefix##_pair			\
    {									\
	struct map_pointer_pair_def(key_type, prefix##_value); \
	map_voidp_to_none_pair voidp_to_none_cast;			\
    };									\
									\
    union prefix##_link			\
    {									\
	struct map_pointer_link_def(prefix##_link, prefix##_pair); \
	map_voidp_to_none_link voidp_to_none_cast;			\
    };									\
    									\
    union prefix##_table			\
    {									\
	struct map_pointer_table_def(prefix##_link);			\
	map_voidp_to_none_table voidp_to_none_cast;			\
    };

#define map_pointer_funcdef(prefix)					\
    inline static prefix##_link ** prefix##_seek(prefix##_table * haystack, prefix##_key * needle) \
    {									\
	return (void*) map_voidp_to_none_seek(&haystack->voidp_to_none_cast, (void*) needle); \
    }									\
									\
    inline static prefix##_pair * prefix##_include(prefix##_table * haystack, prefix##_key * needle) \
    {									\
	return (void*) map_voidp_to_none_include(&haystack->voidp_to_none_cast, (void*) needle); \
    }									\
									\
    inline static void prefix##_resize(prefix##_table * haystack, size_t size) \
    {									\
	map_voidp_to_none_resize(&haystack->voidp_to_none_cast, size);	\
    }									\
									\
    inline static prefix##_link * prefix##_unlink(prefix##_link ** target) \
    {									\
	return (void*) map_voidp_to_none_unlink((void*) target);	\
    }

#define map_pointer_def(prefix, key_type, ...)	\
    map_pointer_typedef(prefix, key_type, __VA_ARGS__); \
    map_pointer_funcdef(prefix);

map_pointer_def(string_to_test, const char, struct{int a, b, c;});

void test()
{
    string_to_test_table table = {0};

    map_string_to_test_include(&table, "asdf");
}

#
