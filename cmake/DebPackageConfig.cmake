cmake_minimum_required(VERSION 3.0.0)

function(find_str _IN _SEP _OUT)
    string(FIND "${_IN}" "${_SEP}" _TMP)
    set(${_OUT} ${_TMP} PARENT_SCOPE)
endfunction(find_str _IN _SEP _OUT)


function(find_next _IN _OUT)
    find_str("${_IN}" "\n" _TMP)
    set(${_OUT} ${_TMP} PARENT_SCOPE)
endfunction(find_next _IN _OUT)

function(sub_next _IN _INDEX _OUT __OUT)
    find_next(${_IN} _NEXTINDEX)
    string(SUBSTRING "${_IN}" ${_INDEX} ${_NEXTINDEX} _TMP)
    math(EXPR _NEXTINDEX ${_NEXTINDEX}+1)
    string(SUBSTRING "${_IN}" ${_NEXTINDEX} -1 __TMP)
    set(${_OUT} ${_TMP} PARENT_SCOPE)
    set(${__OUT} ${__TMP} PARENT_SCOPE)
endfunction(sub_next _IN _INDEX _OUT)

function(trim_str _IN _OUT)
    string(STRIP "${_IN}" _TMP)
    set(${_OUT} ${_TMP} PARENT_SCOPE)
endfunction(trim_str _IN _OUT)

function(split_str _IN _SEP _OUT)
    string(FIND "${_IN}" "${_SEP}" _TMP_INDEX)
    if(NOT _TMP_INDEX EQUAL -1)
        string(SUBSTRING "${_IN}" 0 ${_TMP_INDEX} _TMP)
        math(EXPR _TMP_INDEX ${_TMP_INDEX}+1)
        string(SUBSTRING "${_IN}" ${_TMP_INDEX} -1 __TMP)
        set(${_OUT} "${_TMP};${__TMP}" PARENT_SCOPE)
    else()
        set(${_OUT} ${_IN} PARENT_SCOPE)
    endif(NOT _TMP_INDEX EQUAL -1)
endfunction(split_str _IN _SEP _OUT)

function(split_str_p _IN _SEP _OUT __OUT)
    split_str("${_IN}" "${_SEP}" _TMP)
    list(GET _TMP 0 __TMP)
    list(GET _TMP 1 ___TMP)
    set(${_OUT} ${__TMP} PARENT_SCOPE)
    set(${__OUT} ${___TMP} PARENT_SCOPE)
endfunction(split_str_p _IN _SEP _OUT __OUT)

function(split_str_n _IN _SEP _OUT _N)
    if(_N GREATER 1)
        set(_C ${_N})
        set(_RET "")
        set(_NEXT ${_IN})
        while(NOT _C EQUAL 0)
            split_str("${_NEXT}" "${_SEP}" _TMP)
            list(LENGTH _TMP _TMP_LEN)
            if(_TMP_LEN EQUAL 2)
                list(GET _TMP 0 __TMP)
                list(GET _TMP 1 _NEXT)
                list(APPEND _RET ${__TMP})
            else()
                break()
            endif(_TMP_LEN EQUAL 2)
            math(EXPR _C "${_C}-1")
        endwhile(NOT _C EQUAL 0)
        list(APPEND _RET ${_NEXT})
        set(${_OUT} ${_RET} PARENT_SCOPE)
    else()
        split_str("${_IN}" "${_SEP}" _TMP)
        set(${_OUT} ${_TMP} PARENT_SCOPE)
    endif(_N GREATER 1)
endfunction(split_str_n _IN _SEP _OUT _N)


function(set_package_vars _IN_KEY _IN_VAL)

    # trim_str("${_IN_KEY}" _IN_KEY)

    find_str("${_IN_KEY}" "Type" _Type)
    if(_Type EQUAL "0")
        string(TOUPPER "${_IN_VAL}" _IN_VAL_UPPER)
        string(TOLOWER "${_IN_VAL}" _IN_VAL_LOWER)
        set(CPACK_GENERATOR "${_IN_VAL_UPPER}" PARENT_SCOPE)
    endif(_Type EQUAL "0")

    find_str("${_IN_KEY}" "Package" _Package)
    if(_Package EQUAL "0")
        if(_IN_VAL STREQUAL "auto")
            set(CPACK_DEBIAN_PACKAGE_NAME "${PROJECT_NAME}" PARENT_SCOPE)
        else()
            set(CPACK_DEBIAN_PACKAGE_NAME "${_IN_VAL}" PARENT_SCOPE)
        endif(_IN_VAL STREQUAL "auto")
    endif(_Package EQUAL "0")

    find_str("${_IN_KEY}" "Version" _Version)
    if(_Version EQUAL "0")
        if(_IN_VAL STREQUAL "auto")
            set(CPACK_DEBIAN_PACKAGE_VERSION "${PROJECT_VERSION}" PARENT_SCOPE)
        else()
            set(CPACK_DEBIAN_PACKAGE_VERSION "${_IN_VAL}" PARENT_SCOPE)
        endif(_IN_VAL STREQUAL "auto")
        
    endif(_Version EQUAL "0")

    find_str("${_IN_KEY}" "CalVer" _CalVer)
    if(_CalVer EQUAL "0")
        set(CalVer "${_IN_VAL}" PARENT_SCOPE)
    endif(_CalVer EQUAL "0")

    find_str("${_IN_KEY}" "Architecture" _Architecture)
    if(_Architecture EQUAL "0")
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${_IN_VAL}" PARENT_SCOPE)    
        if(_IN_VAL STREQUAL "auto")
            execute_process(
                COMMAND dpkg --print-architecture
                OUTPUT_VARIABLE _RETV
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${_RETV}" PARENT_SCOPE)
        endif(_IN_VAL STREQUAL "auto")
    endif(_Architecture EQUAL "0")
    
    find_str("${_IN_KEY}" "Priority" _Priority)
    if(_Priority EQUAL "0")
        set(CPACK_DEBIAN_PACKAGE_PRIORITY "${_IN_VAL}" PARENT_SCOPE)
    endif(_Priority EQUAL "0")

    find_str("${_IN_KEY}" "Depends" _Depends)
    if(_Depends EQUAL "0")
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "${_IN_VAL}" PARENT_SCOPE)
    endif(_Depends EQUAL "0")

    find_str("${_IN_KEY}" "Maintainer" _Maintainer)
    if(_Maintainer EQUAL "0")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${_IN_VAL}" PARENT_SCOPE)
    endif(_Maintainer EQUAL "0")

    find_str("${_IN_KEY}" "Homepage" _Homepage)
    if(_Homepage EQUAL "0")
        set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${_IN_VAL}" PARENT_SCOPE)
    endif(_Homepage EQUAL "0")

    find_str("${_IN_KEY}" "Recommends" _Recommends)
    if(_Recommends EQUAL "0")
        set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "${_IN_VAL}" PARENT_SCOPE)
    endif(_Recommends EQUAL "0")
    
endfunction(set_package_vars _IN_KEY _IN_VAL)

function(add_package_descript IN_DES)
    set(PACKAGE_DES_PATH "${IN_DES}")

    if(EXISTS ${IN_DES})

    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${IN_DES}")
        set(PACKAGE_DES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${IN_DES}")
    else()
        message(FATAL_ERROR "!! Not Found Path: ${PACKAGE_DES_PATH}")
        return()
    endif(EXISTS ${IN_DES})
    
    file(READ ${PACKAGE_DES_PATH} DES_CONTENT)
    trim_str("${DES_CONTENT}" DES_CONTENT)


    sub_next(${DES_CONTENT} NEXT_INDEX DES_LINE DES_CONTENT)
    set(PREV_DES "")
    while(NOT DES_LINE STREQUAL "${PREV_DES}")
        find_str("${DES_LINE}" "#" _COMMENT)
        if(_COMMENT EQUAL "0")
            sub_next(${DES_CONTENT} NEXT_INDEX DES_LINE DES_CONTENT)
            continue()
        endif(_COMMENT EQUAL "0")

        find_str("${DES_LINE}" "Descrition" _DESCRIPTION)
        if(_DESCRIPTION EQUAL "0")
            break()
        endif(_DESCRIPTION EQUAL "0")

        split_str_n("${DES_LINE}" ":" _TMP 1)
        list(LENGTH _TMP _TMP_LEN)

        if(_TMP_LEN EQUAL 2)
            split_str_p("${DES_LINE}" ":" _TMP __TMP)
            trim_str("${__TMP}" __TMP)
            string(LENGTH "${__TMP}" __TMP_LENGTH)
            if(NOT __TMP_LENGTH EQUAL "0")
                set_package_vars("${_TMP}" "${__TMP}")
            endif(NOT __TMP_LENGTH EQUAL "0")
        endif(_TMP_LEN EQUAL 2)

        set(PREV_DES "${DES_LINE}")
        sub_next(${DES_CONTENT} NEXT_INDEX DES_LINE DES_CONTENT)
    endwhile(NOT DES_LINE STREQUAL "${PREV_DES}")


    find_str("${DES_LINE}" "Descrition" _DESCRIPTION)
    if(_DESCRIPTION EQUAL "0")
        split_str_p("${DES_LINE}" ":" _TMP __TMP)
        trim_str("${__TMP}" __TMP)
        set(Descrition ${__TMP})
        set(PREV_DES_LINE "")
        while(NOT PREV_DES_LINE STREQUAL DES_LINE)
            if(NOT PREV_DES_LINE STREQUAL "")
                set(Descrition "${Descrition}\n${DES_LINE}")
            endif(NOT PREV_DES_LINE STREQUAL "")
            set(PREV_DES_LINE "${DES_LINE}")
            sub_next(${DES_CONTENT} NEXT_INDEX DES_LINE DES_CONTENT)
        endwhile(NOT PREV_DES_LINE STREQUAL DES_LINE)

        set(CPACK_DEBIAN_PACKAGE_DESCRIPTION ${Descrition})
    endif(_DESCRIPTION EQUAL "0")

    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(ARCHITECTURE "amd64")
    elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "aarch64")
        set(ARCHITECTURE "arm64")
    endif()

    if("${CalVer}" STREQUAL "true")
        string(TIMESTAMP BUILD_TIME "%Y%m%d")
        set(CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_DEBIAN_PACKAGE_VERSION}-${BUILD_TIME}")
    endif("${CalVer}" STREQUAL "true")
    

    set(_Package      "${CPACK_DEBIAN_PACKAGE_NAME}")
    set(_Version      "${CPACK_DEBIAN_PACKAGE_VERSION}")
    set(_Architecture "${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

    set(_DebFileName 
        "${_Package}_${_Version}_${_Architecture}${PACKAGE_SUFFIX}.deb"
    )
    set(CPACK_DEBIAN_FILE_NAME            ${_DebFileName})

    include(CPack)

endfunction(add_package_descript IN_DES)
