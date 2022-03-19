#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#define FLAT_INCLUDES
#include "../../range/def.h"
#include "../../window/def.h"
#include "../../window/string.h"
#include "../../keyargs/keyargs.h"
#include "../../convert/source.h"
#include "../../convert/fd/source.h"
#include "../../convert/getline.h"
#include "../table.h"

#define table_string_value table_string_const_key value
#include "../table-string.h"

#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "../../log/log.h"
#include "../../keyargs/keyargs.h"

char * skip_isspace (char * input, bool pred)
{
    while (*input && isspace(*input) == pred)
    {
        input++;
    }

    return input;
}

void parse_key_value (char ** key, char ** value, char * input)
{
    *key = skip_isspace (input, true);
    *value = skip_isspace (*key, false);
    assert (**value);
    **value = '\0';
    *value = skip_isspace (*value + 1, true);    
    
    assert (**key);
    assert (**value);
    assert (*key != *value);
}

int main(int argc, char * argv[])
{
    window_unsigned_char read_buffer = {0};
    fd_source fd_source = fd_source_init (STDIN_FILENO, &read_buffer);
    table_string map = {0};
    range_const_char line_range;
    range_const_char end_seq = { .begin = "\n", .end = end_seq.begin + 1 };
    bool error = false;
    window_char line_copy = {0};

    char * key;
    char * value;
    char * line;

    while (convert_getline (&error, &line_range, &fd_source.source, &end_seq))
    {
	window_strcpy_range (&line_copy, &line_range);

	line = line_copy.region.begin;
	
	parse_key_value (&key, &value, line);
            
	table_string_include (map, key)->value.value = table_string_include (map, value)->query.key;
	
    }
    /*
    {
        buffer_char read_buffer = {0};
        range_char line;
        char * key;
        char * value;

        while (buffer_getline_fd (.read.fd = STDIN_FILENO,
                                  .read.buffer = &read_buffer,
                                  .line = &line))
        {
            *line.end = '\0';
            parse_key_value (&key, &value, line.begin);
            
            table_string_include (map, key)->value.value = table_string_include (map, value)->query.key;
        }

        free (read_buffer.begin);
	}*/

    void * old = map.begin;

    table_string_resize (map, 3);

    assert (old != map.begin);

    table_string_query query;
    table_string_bucket bucket;

    for (int i = 1; i < argc; i++)
    {
        query = table_string_query (argv[i]);

        assert (query.key == argv[i]);
        assert (query.digest == table_string_hash (argv[i]));

        bucket = table_string_bucket(map, query);

        assert ( (table_digest) (bucket - map.begin) % range_count (map) == (table_digest) query.digest % range_count (map) );

        table_string_match (bucket, query);

        if (*bucket)
        {
            assert ( (*bucket)->query.key != query.key );
            assert ( 0 == strcmp ( (*bucket)->query.key, query.key ) );
            assert ( (*bucket)->query.digest == query.digest );

            if ( (*bucket)->value.value )
            {
                log_normal ("Match %s -> %s", (*bucket)->query.key, (*bucket)->value.value);
            }
            else
            {
                log_normal ("Only a value: %s", (*bucket)->query.key);
            }
        }
        else
        {
            log_normal ("No match for %s", query.key);
        }
    }
    
    table_string_clear (map);
    
    return 0;
};
