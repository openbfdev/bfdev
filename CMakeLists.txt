cmake_minimum_required (VERSION 3.25.2)
project (bfdev C)

set(bfdev_HEADERS
    include/list.h
)

set(bfdev_SRCS
    ${bfdev_HEADERS}
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
