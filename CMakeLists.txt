set(CMAKE_CXX_STANDARD 23)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(coredump_watcher)

set(CONAN_SYSTEM_INCLUDES "On")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address -Wall -Wextra -Wpedantic -Werror)
    add_link_options(-fsanitize=address)
endif()

set(SOURCES_MAIN
    "./src/directory_watcher.cpp"
    "./src/filename_validation.cpp"
    "./src/tar_writer.cpp"
    "./src/temporary_file.cpp"
    "./src/util.cpp"
)

set(LIBRARY_NAME "${PROJECT_NAME}_lib")
add_library(${LIBRARY_NAME} STATIC ${SOURCES_MAIN})

target_include_directories(${LIBRARY_NAME} PUBLIC "./include/")

target_link_libraries(${LIBRARY_NAME} ${CONAN_LIBS})

add_executable(${PROJECT_NAME} ${SOURCES_MAIN} "./src/main.cpp")

target_link_libraries(${PROJECT_NAME} ${LIBRARY_NAME})

enable_testing()
add_subdirectory("./tests/unit/")