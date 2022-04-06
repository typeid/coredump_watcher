project(${CMAKE_PROJECT_NAME}-tests)

add_executable(${PROJECT_NAME}
    "./src/main.cpp"
    "./src/test.util.cpp"
    "./src/test.name_validation.cpp"
    "./src/test.temporary_file.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include/")

target_link_libraries(${PROJECT_NAME} ${LIBRARY_NAME} ${CONAN_LIBS})

add_test(NAME ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMAND ${PROJECT_NAME})