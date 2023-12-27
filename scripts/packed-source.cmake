# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

function(packed_source genfile source)
    file(REMOVE ${genfile})

    file(WRITE ${genfile}
        "/*\n"
        " * Automatically generated file; DO NOT EDIT.\n"
        " * " ${PROJECT_NAME} " packed-source\n"
        " */\n"
        "\n"
    )

    foreach(file ${source})
        file(READ ${file} value)
        file(APPEND ${genfile} "/* File: " ${file} ". */\n")
        file(APPEND ${PROJECT_BINARY_DIR}/bfdev.c "${value}")
        file(APPEND ${genfile} "\n")

        foreach(append ${ARGN})
            file(APPEND ${genfile} ${append})
        endforeach()

        file(APPEND ${genfile} "\n")
    endforeach()
endfunction()
