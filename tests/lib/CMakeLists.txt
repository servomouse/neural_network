add_executable(save_restore_test save_restore_test.c)
target_link_libraries(save_restore_test
    utils
)

add_test(NAME save_restore_test COMMAND save_restore_test)

# Test creating file in a non-existing path and removing a folder
add_executable(utils_test_work_with_folders test_work_with_folders.c)
target_link_libraries(utils_test_work_with_folders
    utils
)

target_compile_definitions(utils_test_work_with_folders
    PRIVATE
        PROJECT_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
)

add_test(NAME utils_test_work_with_folders COMMAND utils_test_work_with_folders)
