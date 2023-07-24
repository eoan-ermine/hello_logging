cmake_minimum_required(VERSION 3.9)
project(HelloLogging CXX)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_TOOLCHAIN_FILE conan_toolchain.cmake)
find_package(Boost 1.78.0 REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} Boost::boost)
