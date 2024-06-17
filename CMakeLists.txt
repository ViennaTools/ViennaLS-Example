cmake_minimum_required(VERSION 3.20)

project("Frosty")

include("cmake/cpm.cmake")

set(CUSTOM_LOOKUP_DIRS
    ""
    CACHE STRING "Directories to account for when searching installed dependencies")

list(APPEND CMAKE_PREFIX_PATH ${CUSTOM_LOOKUP_DIRS})

CPMAddPackage("gh:viennatools/viennals@4.0.0")

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ViennaLS)
