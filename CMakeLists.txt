project(VGL)
cmake_minimum_required(VERSION 2.6)

# External libraries that we use.
find_package(GLUT)
find_package(JPEG)
find_package(PNG)
find_package(OpenGL)
find_package(OpenMP)
find_package(TIFF)

add_definitions(${PNG_DEFINITIONS})

# There's no ready-made CMake module for setting up cppunit, so it's DIY time...
find_library(CPPUNIT_LIBRARY cppunit)
if (CPPUNIT_LIBRARY STREQUAL "CPPUNIT_LIBRARY-NOTFOUND")
  message(WARNING "Unable to find cppunit. Tests won't be built.")
  set(CPPUNIT_FOUND 0)
else (CPPUNIT_LIBRARY STREQUAL "CPPUNIT_LIBRARY-NOTFOUND")
  message(STATUS "Found cppunit: ${CPPUNIT_LIBRARY}")
  set(CPPUNIT_FOUND 1)
endif (CPPUNIT_LIBRARY STREQUAL "CPPUNIT_LIBRARY-NOTFOUND")


# General build properties.
file(GLOB VGL_SOURCES src/*.cpp thirdparty/*.c)
file(GLOB VGL_HEADERS src/*.h)
file(GLOB VGL_EXTRAS test/*.cpp example/*.cpp)


# Compilation and linking properties.
set_source_files_properties(${VGL_SOURCES} ${VGL_EXTRAS}
  COMPILE_FLAGS "-fvisibility=hidden ${OpenMP_CXX_FLAGS}")
include_directories(include thirdparty
  ${GLUT_INCLUDE_DIR}
  ${JPEG_INCLUDE_DIR}
  ${PNG_INCLUDE_DIR}
  ${TIFF_INCLUDE_DIR})


add_custom_command(
  OUTPUT  pick.vert pick.frag draw.vert draw.frag
  DEPENDS example/pick.vert example/pick.frag example/draw.vert example/draw.frag
  COMMAND cp ${VGL_SOURCE_DIR}/example/pick.vert ${VGL_SOURCE_DIR}/example/pick.frag ${VGL_SOURCE_DIR}/example/draw.vert ${VGL_SOURCE_DIR}/example/draw.frag ${VGL_BINARY_DIR}
)
add_custom_command(
  OUTPUT  perface.v.glsl perface.g.glsl perface.f.glsl
  DEPENDS example/perface.v.glsl example/perface.g.glsl example/perface.f.glsl
  COMMAND cp ${VGL_SOURCE_DIR}/example/perface.v.glsl ${VGL_SOURCE_DIR}/example/perface.g.glsl ${VGL_SOURCE_DIR}/example/perface.f.glsl ${VGL_BINARY_DIR}
)


# Helper function which creates an example program.
function (example example_NAME)
  add_executable(${example_NAME} example/${example_NAME}.cpp)
  target_link_libraries(${example_NAME} vgl)
  install(TARGETS ${example_NAME} DESTINATION bin)
endfunction(example)


# Helper function which creates a test program.
function (test test_NAME)
  add_executable(${test_NAME} test/${test_NAME}.cpp)
  target_link_libraries(${test_NAME} vgl ${CPPUNIT_LIBRARY})
  add_test(NAME run-${test_NAME} COMMAND ${test_NAME})
endfunction(test)


# The VGL shared library.
add_library(vgl SHARED ${VGL_SOURCES} ${VGL_THIRDPARTY_SOURCES})
target_link_libraries(vgl
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARIES}
  ${JPEG_LIBRARIES}
  ${PNG_LIBRARIES}
  ${TIFF_LIBRARIES})
install(FILES ${VGL_HEADERS} DESTINATION include)
install(TARGETS vgl LIBRARY DESTINATION lib)


# The unit tests.
if (CPPUNIT_FOUND)
  enable_testing()
  test(test_quaternion)
endif (CPPUNIT_FOUND)


# The example programs.
example(arcball)
example(basic)
example(example)
example(imageview)
example(modelinfo)
example(raymarch)
example(shadowmap)
example(stencil)
example(styled)

#example(picking)
add_executable(picking example/picking.cpp pick.frag pick.vert draw.frag draw.vert)
target_link_libraries(picking vgl)
install(TARGETS picking DESTINATION bin)

#example(perface)
add_executable(perface example/perface.cpp perface.v.glsl perface.g.glsl perface.f.glsl)
target_link_libraries(perface vgl)
install(TARGETS perface DESTINATION bin)

