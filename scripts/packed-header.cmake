# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

function(packed_header prefix name genfile)
    file(REMOVE ${genfile})
    file(WRITE ${genfile}
        "/*\n"
        " * Automatically generated file; DO NOT EDIT.\n"
        " * " ${PROJECT_NAME} " packed-header\n"
        " */\n"
        "\n"
        "#ifndef " ${name} "\n"
        "#define " ${name} "\n"
        "\n"
    )

    foreach(source ${ARGN})
        file(GLOB srclist ${source}/*.h)
        foreach(srcpath ${srclist})
            string(REGEX REPLACE ".+/(.+)" "\\1" filename ${srcpath})
            message(STATUS "Packing header: " ${prefix} ${filename})

            file(APPEND ${genfile}
                "#include <" ${prefix} ${filename} ">\n"
            )
        endforeach()
    endforeach()

    file(APPEND ${genfile}
        "\n"
        "#endif /* " ${name} " */\n"
    )
endfunction()
