cmake_minimum_required(VERSION 2.8)

set(PREFIX ../)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PREFIX})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PREFIX})

set(CMAKE_MODULE_PATH ./)
find_package(SFML REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCL REQUIRED)

file(GLOB_RECURSE SOURCE ${PREFIX}/src/*.cpp)
include_directories(${PREFIX}/include/ ${SFML_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${OPENCL_INCLUDE_DIR})
add_definitions('-Wall' '-Wextra')
#set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} '-pg')

add_executable(main ${SOURCE})
target_link_libraries(main ${SFML_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENCL_LIBRARIES})