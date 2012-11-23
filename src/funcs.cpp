#include "vgl/funcs.h"

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

Eigen::Vector3f unproject(double x, double y, double z)
{
  Eigen::Matrix4d projectionMatrix;
  Eigen::Matrix4d modelViewMatrix;
  Eigen::Vector4i viewportMatrix;

  glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix.data());
  glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix.data());
  glGetIntegerv(GL_VIEWPORT, viewportMatrix.data());

  Eigen::Vector3d point;
  gluUnProject(x, y, z,
      modelViewMatrix.data(), projectionMatrix.data(), viewportMatrix.data(),
      &point.x(), &point.y(), &point.z());

  return Eigen::Vector3f((float)point.x(), (float)point.y(), (float)point.z());
}


} // namespace vgl

