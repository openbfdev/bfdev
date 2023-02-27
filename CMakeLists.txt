cmake_minimum_required (VERSION 3.25.2)
project (bfdev C)

set(bfdev_HEADERS
    include/bfdev/bitrev.h
    include/bfdev/bsearch.h
    include/bfdev/compiler.h
    include/bfdev/const.h
    include/bfdev/container.h
    include/bfdev/list.h
    include/bfdev/poison.h
)

set(bfdev_SRCS
    ${bfdev_HEADERS}
    src/bitrev.c
    src/bsearch.c
    src/list-check.c
    src/list-sort.c
)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(bfdev_shared SHARED ${bfdev_SRCS})

add_library(bfdev_static STATIC ${bfdev_SRCS})

SET_TARGET_PROPERTIES(bfdev_shared
PROPERTIES VERSION 1.0 SOVERSION 1
OUTPUT_NAME bfdev
MACOSX_RPATH ON
LINKER_LANGUAGE C
)

SET_TARGET_PROPERTIES(bfdev_static
PROPERTIES
OUTPUT_NAME bfdev
LINKER_LANGUAGE C
)

add_library(bfdev ALIAS bfdev_shared)
add_subdirectory(examples)
