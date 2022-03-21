C_PROGRAMS += test/table-string-benchmark
C_PROGRAMS += test/table2

RUN_TESTS += test/table2

table-benchmarks: test/table-string-benchmark

#table-tests: test/run-table-string-map
#table-tests: test/table-string-map
table-tests: test/table2

test/run-table-string-map: src/table/test/table-string-map.test.sh
test/table-string-benchmark: src/log/log.o
test/table-string-benchmark: src/table/table.o
test/table-string-benchmark: src/table/test/table-string-benchmark.test.o

test/table2: src/table/test/table2.test.o
test/table2: src/table/string.o
test/table2: src/range/streq_string.o
test/table2: src/range/string_init.o
test/table2: src/range/streq.o
test/table2: src/range/strdup.o

tests: table-tests

benchmarks: table-benchmarks
