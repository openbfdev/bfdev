# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2025 John Sanpe <sanpeqf@gmail.com>
#

function(compare_type result type1 type2)
    message(DEBUG "Compare type: '${type1}' '${type2}'")
    try_compile(
        COMPILE_RESULT
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/compare-type.c
        COMPILE_DEFINITIONS "-Werror"
        COMPILE_DEFINITIONS "-DTYPE1=${type1}"
        COMPILE_DEFINITIONS "-DTYPE2=${type2}"
        OUTPUT_VARIABLE OUTPUT
    )
    message(DEBUG "${OUTPUT}")
    set(${result} ${COMPILE_RESULT} PARENT_SCOPE)
endfunction()

function(get_type result name default)
    message(CHECK_START "Looking for type '${name}'")
    set(candidates "int" "long" "long long" "char" "short")

    foreach(walk IN LISTS candidates)
        compare_type(compare "${name}" "${walk}")
        if(NOT ${compare})
            compare_type(compare "${name}" "unsigned ${walk}")
        endif()

        if(${compare})
            set(type "${walk}")
            message(CHECK_PASS "found '${walk}'")
            break()
        endif()
    endforeach()

    if(NOT type)
        message(CHECK_FAIL "use default '${default}'")
        set(type "${default}")
    endif()
    set(${result} "${type}" PARENT_SCOPE)
endfunction()
