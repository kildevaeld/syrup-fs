
set(SOURCES
    main.c
    fs.c
)   

add_executable(fs_test ${SOURCES})
target_link_libraries(fs_test syrup-fs unity)
add_test(fs_test fs_test)
