
typedef unsigned long long table_digest;

#define table_typedef(query_tag,key_type,table_name,...)		\
    typedef key_type table_##table_name##_key;				\
    typedef const key_type table_##table_name##_const_key;		\
    typedef query_tag table_##table_name##_query table_##table_name##_query; \
    query_tag table_##table_name##_query {					\
	table_digest digest;						\
	table_##table_name##_const_key key;				\
    };									\
    typedef struct table_##table_name##_item table_##table_name##_item; \
    struct table_##table_name##_item {					\
	union {								\
	    struct {							\
		table_digest _digest;					\
		table_##table_name##_key _key;						\
	    };								\
	    table_##table_name##_query query;				\
	};								\
	table_##table_name##_item * next;				\
	struct {							\
	    __VA_ARGS__;						\
	}								\
	    value;							\
    };									\
    typedef table_##table_name##_item** table_##table_name##_bucket;	\
    typedef struct table_##table_name table_##table_name;		\
    struct table_##table_name {						\
	table_digest item_count;					\
	struct range(table_##table_name##_item*);			\
    };
