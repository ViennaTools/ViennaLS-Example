cmake_minimum_required(VERSION 3.20)

project("Frosty")

include("cmake/cpm.cmake")

CPMAddPackage("gh:viennatools/viennals@4.3.2")

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ViennaLS)
