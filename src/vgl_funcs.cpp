#include "vgl_funcs.h"

#include "vgl_vec4.h"
#include "vgl_matrix4.h"

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

namespace vgl {


//
// OpenGL helper functions.
//

Vec3f unproject(double x, double y, double z)
{
  Matrix4d projectionMatrix;
  Matrix4d modelViewMatrix;
  Vec4i viewportMatrix;

  glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix.data);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix.data);
  glGetIntegerv(GL_VIEWPORT, viewportMatrix.data);

  Vec3d point;
  gluUnProject(x, y, z,
      modelViewMatrix.data, projectionMatrix.data, viewportMatrix.data,
      &point.x, &point.y, &point.z);

  return Vec3f((float)point.x, (float)point.y, (float)point.z);
}


} // namespace vgl

