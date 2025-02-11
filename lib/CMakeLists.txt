add_library(lib
    utils.c
)

# Specify the include directory for the lib target
target_include_directories(lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
