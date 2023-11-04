# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

macro(asm_generic prefix generated compare source)
    file(GLOB srclist ${source}/*.h)
    file(GLOB cmplist ${compare}/*.h)

    foreach(srcpath ${srclist})
        string(REGEX REPLACE ".+/(.+)" "\\1" filename ${srcpath})
        list(FIND cmplist ${compare}/${filename} retval)
        set(genfile ${generated}/${filename})

        file(REMOVE ${genfile})
        if(${retval})
            file(WRITE ${genfile}
                "/*\n"
                " * Automatically generated file; DO NOT EDIT.\n"
                " * " ${PROJECT_NAME} " asm-generic\n"
                " */\n"
                "\n"
                "#include <" ${prefix} ${filename} ">\n"
            )
        endif()
    endforeach()

    unset(srclist)
    unset(cmplist)
endmacro()
