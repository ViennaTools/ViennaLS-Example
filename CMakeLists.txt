cmake_minimum_required(VERSION 3.4)

project("Frosty")

# automaticall append standard path for ViennaLSConfig.cmake
# might need to be changed for different install directories
# not necessary when installed to /usr/local (not recommended)
set(ViennaLS_DIR "${ViennaLS_DIR}/ViennaLS/lib/cmake/ViennaLS/")

find_package(ViennaLS REQUIRED)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${VIENNALS_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${VIENNALS_LIBRARIES})
