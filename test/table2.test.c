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

map_string_define(test_ab, struct { int a, b; });

void * test_ab_link_new(void * table)
{
    return calloc (1, sizeof(test_ab_link));
}

void test_host_map()
{
    test_ab_table table = {0};

    test_ab_include_string(&table, "asdf")->value = (test_ab_value){ .a = 1, .b = 2 };

    test_ab_include_string(&table, "bcle")->value = (test_ab_value){ .a = 3, .b = 5 };

    test_ab_include_string(&table, "1234")->value = (test_ab_value){ .a = 7, .b = 11 };

    assert (range_streq_string(&test_ab_include_string(&table, "asdf")->query.key.range, "asdf"));
    assert (test_ab_include_string(&table, "asdf") == test_ab_include_string(&table, "asdf"));

    assert (test_ab_include_string(&table, "asdf")->value.a == 1);
    assert (test_ab_include_string(&table, "asdf")->value.b == 2);

    assert (test_ab_include_string(&table, "bcle")->value.a == 3);
    assert (test_ab_include_string(&table, "bcle")->value.b == 5);

    assert (test_ab_include_string(&table, "1234")->value.a == 7);
    assert (test_ab_include_string(&table, "1234")->value.b == 11);
}

map_string_define(self, self_pair*);

void * self_link_new (void * table)
{
    return calloc (1, sizeof(self_link));
}

void test_host_self_map()
{
    self_table table = {0};

    self_include_string(&table, "a")->value = self_include_string(&table, "picture");
    self_include_string(&table, "is")->value = self_include_string(&table, "worth");
    self_include_string(&table, "a")->value = self_include_string(&table, "thousand");
    self_include_string(&table, "words")->value = self_include_string(&table, "words");

    assert (self_include_string(&table, "is")->value == self_include_string(&table, "worth"));
    assert (self_include_string(&table, "a")->value == self_include_string(&table, "thousand"));
    assert (self_include_string(&table, "words")->value == self_include_string(&table, "words"));

    assert (0 == strcmp (self_include_string(&table, "is")->query.key.string, "is"));
}

int main()
{
    test_host_map();
    test_host_self_map();

    return 0;
}
