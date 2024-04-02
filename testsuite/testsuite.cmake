# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

function(testsuite_target name src replace)
    string(
        REGEX REPLACE ${replace} ""
        TESTSUIT_OBJECT "${BFDEV_LIBRARY}"
    )

    add_library("${name}_target" OBJECT ${src})
    target_compile_options("${name}_target" PRIVATE -include ${replace})
    bfdev_dependencies("${name}_target")

    add_executable("${name}" ${TESTSUIT_OBJECT})
    target_link_libraries("${name}" "${name}_target" testsuite)
endfunction()
