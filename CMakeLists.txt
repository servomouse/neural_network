cmake_minimum_required(VERSION 3.31)
# include(config.cmake)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
project(project_name)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # Generate compile_commands.json for vscode
include(CTest)

# Set up build directories
# set(CMAKE_BUILD_TYPE Release)

# Remove default flags
# set(CMAKE_C_FLAGS "")
# set(CMAKE_CXX_FLAGS "")
# set(CMAKE_BUILD_TYPE "")

add_subdirectory(lib)
add_subdirectory(source)
add_subdirectory(tests)
add_subdirectory(train_micronets)

# Train micronet targets

# add_executable(train_linear_micronet train_linear_micronet.c)

# target_link_libraries(train_linear_micronet
#     PRIVATE
#         micronet
#         neuron
#         utils
# )
# target_include_directories(train_linear_micronet
#     PRIVATE
#         ${TESTS_DIR}
#         ${CMAKE_CURRENT_SOURCE_DIR}
# )

# set(CMAKE_C_FLAGS "")
# set(CMAKE_CXX_FLAGS "")

# target_compile_options(train_linear_micronet
#     PRIVATE
#         -Wall
#         -Wextra
#         -lm
#         -g
#         -lws2_32
#         -fstack-protector-all
# )

# target_compile_definitions(train_linear_micronet
#     PRIVATE
#         BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
# )

# add_executable(train_poly_micronet train_poly_micronet.c)

# target_link_libraries(train_poly_micronet
#     PRIVATE
#         micronet
#         neuron
#         utils
# )
# target_include_directories(train_poly_micronet
#     PRIVATE
#         ${TESTS_DIR}
#         ${CMAKE_CURRENT_SOURCE_DIR}
# )

# set(CMAKE_C_FLAGS "")
# set(CMAKE_CXX_FLAGS "")

# target_compile_options(train_poly_micronet
#     PRIVATE
#         -Wall
#         -Wextra
#         -lm
#         -g
#         -lws2_32
#         -fstack-protector-all
# )

# target_compile_definitions(train_poly_micronet
#     PRIVATE
#         BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
# )

# add_executable(train_micronet train_micronet.c)

# target_link_libraries(train_micronet
#     PRIVATE
#         network
#         neuron
#         utils
# )
# target_include_directories(train_micronet
#     PRIVATE
#         ${TESTS_DIR}
#         ${CMAKE_CURRENT_SOURCE_DIR}
# )

# set(CMAKE_C_FLAGS "")
# set(CMAKE_CXX_FLAGS "")

# target_compile_options(train_micronet
#     PRIVATE
#         -Wall
#         -Wextra
#         -lm
#         -g
#         -lws2_32
#         -fstack-protector-all
# )

# target_compile_definitions(train_micronet
#     PRIVATE
#         BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
# )

# add_executable(train_swarm train_swarm.c)

# target_link_libraries(train_swarm
#     PRIVATE
#         network
#         neuron
#         utils
# )
# target_include_directories(train_swarm
#     PRIVATE
#         ${TESTS_DIR}
#         ${CMAKE_CURRENT_SOURCE_DIR}
# )

# set(CMAKE_C_FLAGS "")
# set(CMAKE_CXX_FLAGS "")

# target_compile_options(train_swarm
#     PRIVATE
#         -Wall
#         -Wextra
#         -lm
#         -g
#         -lws2_32
#         -fstack-protector-all
# )

# target_compile_definitions(train_swarm
#     PRIVATE
#         BCKP_DIR_PATH="${PROJECT_SOURCE_DIR}/backups"
# )

# include("${PROJECT_SOURCE_DIR}/train_embedded_micronet.cmake")

add_custom_target(cleanup-build-folder
    COMMAND ${CMAKE_COMMAND} -E echo "Cleaning up build folder..."
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cleanup.cmake
)
