# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

macro(asm_generic prefix generated compare source)
    file(GLOB srclist ${source}/*)
    file(GLOB cmplist ${compare}/*)

    foreach(srcpath ${srclist})
        string(REGEX REPLACE ".+/(.+)" "\\1" filename ${srcpath})
        list(FIND cmplist ${compare}/${filename} retval)
        set(genfile ${generated}/${filename})

        file(REMOVE ${genfile})
        if(${retval})
            file(WRITE ${genfile}
                "/*\n"
                " * Automatically generated file; DO NOT EDIT.\n"
                " * " ${CMAKE_PROJECT_NAME} " asm-generic\n"
                " */\n"
                "#include <" ${prefix} ${filename} ">\n"
            )
        endif(${retval})
    endforeach(srcpath)

    unset(srclist)
    unset(cmplist)
endmacro(asm_generic)
