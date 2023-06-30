

add_executable(demo_log demo_log.c)
target_link_libraries(demo_log bfdev)
add_test(demo_log demo_log)


if(${CMAKE_PROJECT_NAME} STREQUAL "bfdev")
    install(FILES
        demo_log.c
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/examples/log/
    )

    install(TARGETS
        demo_log
        DESTINATION
        ${CMAKE_INSTALL_DOCDIR}/bin
    )
endif()