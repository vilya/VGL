#ifndef vgl_h
#define vgl_h

// This is provided as a convenience, to save you having to include each header
// individually. You may still wish to include them separately to help keep 
// compile times down, though.

// Maths
#include <Eigen/Dense>
#include "vgl/plane3.h"
#include "vgl/ray3.h"

// Cameras
#include "vgl/arcballcamera.h"
#include "vgl/basecamera.h"
#include "vgl/camera.h"
#include "vgl/orthocamera.h"

// Image files
#include "vgl/image.h"

// Model files
#include "vgl/parser.h"

// Rendering
#include "vgl/renderer.h"

// GUI
#include "vgl/viewer.h"

// Miscellaneous
#include "vgl/funcs.h"
#include "vgl/utils.h"

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

