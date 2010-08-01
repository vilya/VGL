#ifndef vgl_h
#define vgl_h

// This is provided as a convenience, to save you having to include each header
// individually. You may still wish to include them separately to help keep 
// compile times down, though.

#include "vgl_camera.h"
#include "vgl_image.h"
#include "vgl_matrix3.h"
#include "vgl_parser.h"
#include "vgl_plane3.h"
#include "vgl_ray3.h"
#include "vgl_renderer.h"
#include "vgl_vec3.h"
#include "vgl_viewer.h"

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

