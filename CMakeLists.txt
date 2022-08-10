cmake_minimum_required(VERSION 3.21)
project(myloxx)

set(CMAKE_CXX_STANDARD 20)
set(BOOST_ROOT "/Users/dimashestakov/Source/CXX/libraries/boost_1_79_0")

find_package(Boost 1.79.0 REQUIRED)

add_compile_options(-Wall -Wextra -pedantic -Werror -DDEBUG_TRACE_EXECUTION)

add_executable(myloxx main.cpp src/debug.cpp src/chunk.cpp src/value.cpp src/VM.cpp)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include
        ${Boost_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${Boost_LIBRARIES})
