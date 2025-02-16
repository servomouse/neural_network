# Save-restore functionality test
add_executable(save_restore_test save_restore_test.c)
target_link_libraries(save_restore_test
    utils
)

add_test(NAME save_restore_test COMMAND save_restore_test)

# add_dependencies(runtest save_restore_test)
