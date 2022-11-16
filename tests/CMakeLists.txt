function(create_test name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} svector)

    add_test(NAME ${name} COMMAND ${name})
endfunction()

