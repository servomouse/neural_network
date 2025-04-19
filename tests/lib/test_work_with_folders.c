#include <stdio.h>
#include "utils.h"

#define TEST_DIRECTORY_PATH PROJECT_DIR_PATH"/test_dir_to_be_removed"

int main(int argc, char* argv[])
{
    // Check that the folder does not exist
    if (is_dir_exist(TEST_DIRECTORY_PATH)) {
        printf("Error: The %s folder exists\n", TEST_DIRECTORY_PATH);
        exit(EXIT_FAILURE);
    } else {
        printf("The %s folder does not exist\n", TEST_DIRECTORY_PATH);
    }

    // Open file no matter what
    FILE *file = fopen_no_matter_what(TEST_DIRECTORY_PATH"/test.txt", "ab");
    if (file != NULL) {
        fprintf (file, "%s", "buffer");
        fclose(file);
    } else {
        printf("File write error!");
        exit(EXIT_FAILURE);
    }

    // Check that now the folder does exist
    if (is_dir_exist(TEST_DIRECTORY_PATH)) {
        printf("The %s folder exists\n", TEST_DIRECTORY_PATH);
    } else {
        printf("Error: The %s folder does not exist\n", TEST_DIRECTORY_PATH);
        exit(EXIT_FAILURE);
    }

    remove_directory(TEST_DIRECTORY_PATH);

    // Check that the folder does not exist
    if (is_dir_exist(TEST_DIRECTORY_PATH)) {
        printf("Error: The %s folder exists\n", TEST_DIRECTORY_PATH);
        exit(EXIT_FAILURE);
    } else {
        printf("The %s folder does not exist\n", TEST_DIRECTORY_PATH);
    }

    return EXIT_SUCCESS;
}