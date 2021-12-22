#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#define FLAT_INCLUDES
#include "../../array/range.h"
#include "../../array/buffer.h"
#include "../table.h"
#include "../table-string.h"
#include "../../keyargs/keyargs.h"
#include "../../buffer_io/buffer_io.h"
#include "../../log/log.h"

typedef unsigned long long usec;

struct timeval start;

void start_time()
{
    gettimeofday (&start, NULL); 
}

usec get_delta()
{
    struct timeval t;
    gettimeofday (&t, NULL);
    return 1e6 * (t.tv_sec - start.tv_sec) + (t.tv_usec - start.tv_usec);
}

#define TEST_SIZE ((long long)1e6)

void empty_function(char * arg)
{
}

int main()
{
    log_normal("test size: %zu", TEST_SIZE);

    char * string_list[TEST_SIZE];

    buffer_char write_string = {0};

    for (int i = 0; i < TEST_SIZE; i++)
    {
	buffer_printf(&write_string, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa%x - %x - %x", i, i, i);
	string_list[i] = strcpy (malloc (strlen (write_string.begin) + 1), write_string.begin);
	//printf("String: %s\n", string_list[i]);
    }

    // time a sleep
    
    start_time();
    sleep(1);
    usec sleep_time = get_delta();
    
    // time an empty loop
    
    start_time();

    for (int i = 0; i < TEST_SIZE; i++)
    {
	empty_function (string_list[i]);
    }

    usec empty_time = get_delta();

    // time including new keys
    
    table_string table = {0};

    table_string_resize (table, TEST_SIZE);

    start_time();

    for (int i = 0; i < TEST_SIZE; i++)
    {
	table_string_include (table, string_list[i]);
    }

    usec insert_time = get_delta();

    // time including existing keys
    
    start_time();

    for (int i = 0; i < TEST_SIZE; i++)
    {
	table_string_include (table, string_list[i]);
    }

    usec match_time = get_delta();

    log_normal ("\nusec sleep 1s:\t\t%zu\t(%f seconds)", sleep_time, (double) sleep_time / 1e6);
    log_normal ("usec empty loop:\t%zu\t(%f per)", empty_time, (double) empty_time / TEST_SIZE);
    log_normal ("usec include new:\t%zu\t(%f per)", insert_time, (double) insert_time / TEST_SIZE);
    log_normal ("usec include existing:\t%zu\t(%f per)", match_time, (double) match_time / TEST_SIZE);

    log_normal ("\nbucket count: %zu", range_count (table));
    log_normal ("items size: %zu kbytes", (table.item_count * sizeof(table_string_item)) / 1024);
    log_normal ("buckets size: %zu kbytes", (range_count(table) * sizeof(*table.begin)) / 1024);
    size_t total = (table.item_count * sizeof(table_string_item) + range_count(table) * sizeof(*table.begin));
    log_normal ("total size: %zu kbytes\t(%f bytes per)", total / 1024, (double) total / TEST_SIZE);

    for (int i = 0; i < TEST_SIZE; i++)
    {
	free (string_list[i]);
    }

    free (write_string.begin);

    if (table.item_count != TEST_SIZE)
    {
	log_error ("Table has wrong size after tests");
    }
    
    return 0;
}
