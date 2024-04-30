cmake_minimum_required(VERSION 3.10)

project(chatapp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG RELEASE)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

option(RUN_YARN_BUILD "Run yarn build command" OFF)
if(RUN_YARN_BUILD)
    # Execute yarn run build command
    execute_process(COMMAND yarn run build WORKING_DIRECTORY ../web_front)
    if(NOT ${yarn_result} EQUAL 0)
        message(FATAL_ERROR "Failed to run yarn run build command")
    endif()
endif()

include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/event_loop/*" "src/tox_node/*" "src/back_end/*"  "src/back_end/controller/*")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (CMAKE_CXX_FLAGS "-g -O0")
find_package(Drogon REQUIRED)
add_subdirectory(c-toxcore)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/c-toxcore/toxcore")
target_link_libraries(${PROJECT_NAME} Drogon::Drogon -static)
target_link_libraries(${PROJECT_NAME} toxcore_static -static)
