set(PROJECT_NAME "smart_network")
set(PROJECT_VERSION "1.0")


set(CMAKE_BUILD_TYPE "")    # This removes standard compilation flags
# set(CMAKE_C_FLAGS "")
# set(CMAKE_CXX_FLAGS "")
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER c++)

set(CMAKE_C_FLAGS_INIT "")
set(CMAKE_CXX_FLAGS_INIT "")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "")
set(CMAKE_EXE_LINKER_FLAGS_INIT "")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -lm -g -Wall -lws2_32 -fstack-protector-all")
set(SHARED_FLAGS "-shared")

# Define a custom build type
# set(CMAKE_BUILD_TYPE "Regular" CACHE STRING "Build type: Regular or Shared")

# Set the default build type to Regular
# set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Regular" "Shared")

# Define the compilation flags for each build type
# if (${CMAKE_BUILD_TYPE} STREQUAL "Regular")
#     set(SHARED_FLAGS "")
# elseif (${CMAKE_BUILD_TYPE} STREQUAL "Shared")
#     set(SHARED_FLAGS "-shared")
# endif()
