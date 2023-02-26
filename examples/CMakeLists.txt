add_executable(list-benchmark list-benchmark.c)

set_source_files_properties(list-benchmark 
PROPERTIES COMPILE_FLAGS
"-Wall -Wno-unused-parameter")
target_link_libraries(list-benchmark bfdev)


add_executable(list-selftest list-selftest.c)

set_source_files_properties(list-selftest 
PROPERTIES COMPILE_FLAGS
"-Wall -Wno-unused-parameter")

target_link_libraries(list-selftest bfdev)