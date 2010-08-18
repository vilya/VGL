#ifndef vgl_h
#define vgl_h

// This is provided as a convenience, to save you having to include each header
// individually. You may still wish to include them separately to help keep 
// compile times down, though.

// Maths
#include "vgl_matrix3.h"
#include "vgl_matrix4.h"
#include "vgl_plane3.h"
#include "vgl_ray3.h"
#include "vgl_vec2.h"
#include "vgl_vec3.h"
#include "vgl_vec4.h"
#include "vgl_quaternion.h"

// Cameras
#include "vgl_basecamera.h"
#include "vgl_camera.h"
#include "vgl_orthocamera.h"

// Image files
#include "vgl_image.h"

// Model files
#include "vgl_parser.h"

// Rendering
#include "vgl_renderer.h"

// GUI
#include "vgl_viewer.h"

// Miscellaneous
#include "vgl_funcs.h"
#include "vgl_utils.h"

// OpenGL
#define GL_GLEXT_PROTOTYPES 1
#ifdef linux
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

#endif // vgl_h

