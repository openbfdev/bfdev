# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

add_executable(crypto-bandwidth bandwidth.c)
target_link_libraries(crypto-bandwidth bfdev)
add_test(crypto-bandwidth crypto-bandwidth)

add_executable(crypto utils.c)
target_link_libraries(crypto bfdev)

add_test(sha1 crypto "helloworld")
add_test(sha224 crypto -l "helloworld")
add_test(sha256 crypto -e "helloworld")
add_test(md5 crypto -m "helloworld")

if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        bandwidth.c
        utils.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/crypto
    )

    install(TARGETS
        crypto
        crypto-bandwidth
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()
