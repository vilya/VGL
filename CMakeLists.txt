project(VGL)
cmake_minimum_required(VERSION 2.6)


# General build properties
file(GLOB VGL_SOURCES src/*.cpp thirdparty/*.c)
file(GLOB VGL_HEADERS src/*.h)

include_directories(src thirdparty /opt/local/include)
link_directories(/opt/local/lib)


# Helper function which creates an example program.
function (example example_NAME)
  add_executable(${example_NAME} example/${example_NAME}.cpp)
  target_link_libraries(${example_NAME} vgl ${VGL_LIBS})
  if (${APPLE})
    set_target_properties(${example_NAME} PROPERTIES LINK_FLAGS "-framework OpenGL -framework GLUT")
  endif(${APPLE})
  install(TARGETS ${example_NAME} DESTINATION bin)
endfunction(example)

# Helper function which creates a test program.
function (test test_NAME)
  add_executable(${test_NAME} test/${test_NAME}.cpp)
  target_link_libraries(${test_NAME} vgl cppunit)
  if (${APPLE})
    set_target_properties(${test_NAME} PROPERTIES LINK_FLAGS "-framework OpenGL -framework GLUT")
  endif(${APPLE})
  install(TARGETS ${test_NAME} DESTINATION bin)
  add_test(NAME run-${test_NAME} COMMAND ${test_NAME})
endfunction(test)


# The VGL shared library
add_library(vgl SHARED ${VGL_SOURCES} ${VGL_THIRDPARTY_SOURCES})
if (${APPLE})
  target_link_libraries(vgl png jpeg tiff)
  set_target_properties(vgl PROPERTIES LINK_FLAGS "-framework OpenGL -framework GLUT")
else(${APPLE})
  target_link_libraries(vgl glut pthread png jpeg tiff)
endif(${APPLE})
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

