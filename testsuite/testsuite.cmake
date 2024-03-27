# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2024 John Sanpe <sanpeqf@gmail.com>
#

function(testsuite_target name src replace)
    string(
        REGEX REPLACE ${replace} ""
        TESTSUIT_OBJECT "${BFDEV_LIBRARY}"
    )

    add_library(target OBJECT ${src})
    target_compile_options(target PRIVATE -include ${replace})

    add_executable("${name}" ${TESTSUIT_OBJECT})
    target_link_libraries("${name}" testsuite target)
endfunction()
