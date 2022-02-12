#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define FLAT_INCLUDES
#include "../../range/def.h"
#include "../../range/alloc.h"
#include "../../range/string.h"
#include "../string.h"

define_string_host_map(test, struct { int a, b; });

define_string_ref_map(test, struct { int c, d; });

void test_host_map()
{
    host_string_to_test_table table = {0};

    host_string_to_test_include_string(&table, "asdf")->value = (host_string_to_test_value){ .a = 1, .b = 2 };

    host_string_to_test_include_string(&table, "bcle")->value = (host_string_to_test_value){ .a = 3, .b = 5 };

    host_string_to_test_include_string(&table, "1234")->value = (host_string_to_test_value){ .a = 7, .b = 11 };

    assert (range_streq_string(&host_string_to_test_include_string(&table, "asdf")->query.key, "asdf"));
    assert (host_string_to_test_include_string(&table, "asdf") == host_string_to_test_include_string(&table, "asdf"));

    assert (host_string_to_test_include_string(&table, "asdf")->value.a == 1);
    assert (host_string_to_test_include_string(&table, "asdf")->value.b == 2);

    assert (host_string_to_test_include_string(&table, "bcle")->value.a == 3);
    assert (host_string_to_test_include_string(&table, "bcle")->value.b == 5);

    assert (host_string_to_test_include_string(&table, "1234")->value.a == 7);
    assert (host_string_to_test_include_string(&table, "1234")->value.b == 11);
}

define_string_host_map(self, host_string_to_self_pair*);
void test_host_self_map()
{
    host_string_to_self_table table = {0};

    host_string_to_self_include_string(&table, "a")->value = host_string_to_self_include_string(&table, "picture");
    host_string_to_self_include_string(&table, "is")->value = host_string_to_self_include_string(&table, "worth");
    host_string_to_self_include_string(&table, "a")->value = host_string_to_self_include_string(&table, "thousand");
    host_string_to_self_include_string(&table, "words")->value = host_string_to_self_include_string(&table, "words");

    assert (host_string_to_self_include_string(&table, "is")->value == host_string_to_self_include_string(&table, "worth"));
    assert (host_string_to_self_include_string(&table, "a")->value == host_string_to_self_include_string(&table, "thousand"));
    assert (host_string_to_self_include_string(&table, "words")->value == host_string_to_self_include_string(&table, "words"));

    assert (0 == strcmp (host_string_to_self_include_string(&table, "is")->query.key.begin, "is"));
}

define_string_ref_map(self, ref_string_to_self_pair*);
void test_ref_map()
{
    host_string_to_self_table host = {0};

    host_string_to_self_include_string(&host, "a")->value = host_string_to_self_include_string(&host, "picture");
    host_string_to_self_include_string(&host, "is")->value = host_string_to_self_include_string(&host, "worth");
    host_string_to_self_include_string(&host, "a")->value = host_string_to_self_include_string(&host, "thousand");
    host_string_to_self_include_string(&host, "words")->value = host_string_to_self_include_string(&host, "words");

    table_string_query * query_a = &host_string_to_self_include_string(&host, "a")->query;  
    table_string_query * query_is = &host_string_to_self_include_string(&host, "is")->query;
    table_string_query * query_words = &host_string_to_self_include_string(&host, "words")->query;

    ref_string_to_self_table ref = {0};

    range_calloc(ref, 100);
    
    ref_string_to_self_include(&ref, query_a)->value = ref_string_to_self_include(&ref, query_is);
    ref_string_to_self_include(&ref, query_words)->value = ref_string_to_self_include(&ref, query_a);
    
    assert (ref_string_to_self_include(&ref, query_a)->value == ref_string_to_self_include(&ref, query_is));
    assert (ref_string_to_self_include(&ref, query_words)->value == ref_string_to_self_include(&ref, query_a));
}

int main()
{
    test_host_map();
    test_host_self_map();
    test_ref_map();

    return 0;
}
