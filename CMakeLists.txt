project(VGL)
cmake_minimum_required(VERSION 2.6)


# General build properties
file(GLOB VGL_SOURCES src/*.cpp thirdparty/*.c)
file(GLOB VGL_HEADERS src/*.h)
find_library(OpenGL_LIB OpenGL gl)
find_library(GLUT_LIB GLUT glut)

include_directories(src thirdparty /opt/local/include)
link_directories(/opt/local/lib)


# Helper function which creates an example program.
function (example example_NAME)
  add_executable(${example_NAME} example/${example_NAME}.cpp)
  target_link_libraries(${example_NAME} vgl)
  install(TARGETS ${example_NAME} DESTINATION bin)
endfunction(example)

# Helper function which creates a test program.
function (test test_NAME)
  add_executable(${test_NAME} test/${test_NAME}.cpp)
  target_link_libraries(${test_NAME} vgl cppunit)
  install(TARGETS ${test_NAME} DESTINATION bin)
  add_test(NAME run-${test_NAME} COMMAND ${test_NAME})
endfunction(test)


# The VGL shared library
add_library(vgl SHARED ${VGL_SOURCES} ${VGL_THIRDPARTY_SOURCES})
target_link_libraries(vgl ${OpenGL_LIB} ${GLUT_LIB} pthread png jpeg tiff)
install(FILES ${VGL_HEADERS} DESTINATION dist/include)
install(TARGETS vgl LIBRARY DESTINATION dist/lib)


# The unit test programs
enable_testing()
test(test_quaternion)


# The example programs
example(arcball)
example(basic)
example(example)
example(imageview)
example(styled)

