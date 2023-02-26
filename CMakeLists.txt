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

add_library(bfdev SHARED ${bfdev_SRCS})