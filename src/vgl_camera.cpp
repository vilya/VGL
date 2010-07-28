#include "vgl_camera.h"

#include <cmath>

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
// Camera METHODS
//

Camera::Camera(const Float3& pos, const Float3& target, const Float3& up,
    float left, float right, float bottom, float top, float aperture,
    unsigned int pixelWidth, unsigned int pixelHeight) :
  _pos(pos),
  _target(target),
  _up(up),
  _left(left),
  _right(right),
  _bottom(bottom),
  _top(top),
  _aperture(aperture),
  _pixelWidth(pixelWidth),
  _pixelHeight(pixelHeight)
{
  /* My old lookAt code from the Ray project:
  Float3 dir = target - pos;
  Float3 w = norm(dir);
  Float3 u = norm(cross(up, dir));
  Float3 v = cross(w, u);

  Camera* cam = new Camera();

  cam->proj.corner = pos + left * u + bottom * v + length(dir) * w;
  cam->proj.u = (right - left) * u / pixelWidth;
  cam->proj.v = (top - bottom) * v / pixelHeight;

  cam->lens.corner = pos - (0.5 * aperture) * u - (0.5 * aperture) * v;
  cam->lens.u = aperture * u;
  cam->lens.v = aperture * v;

  return cam;
  */
}


Camera::~Camera()
{
}


void Camera::panBy(float dx, float dy, float dz)
{
  Float3 viewVec = _target - _pos;
  // TODO: Rotate viewVec by dx degrees around the x axis
  // TODO: Rotate viewVec by dz degrees around the z axis
  // TODO: Rotate viewVec by dy degrees around the y axis
  _target = viewVec + _pos;
}


void Camera::rollBy(float dx, float dy, float dz)
{
  Float3 viewVec = _pos - _target;
  // TODO: Rotate viewVec by dx degrees around the x axis
  // TODO: Rotate viewVec by dz degrees around the z axis
  // TODO: Rotate viewVec by dy degrees around the y axis
  _pos = viewVec + _target;
}


void Camera::dollyBy(float dx, float dy, float dz)
{
  Float3 delta(dx, dy, dz);
  _pos += delta;
  _target += delta;
}


void Camera::zoomBy(float dz)
{
  _aperture *= powf(1.1, -dz / 2.0f);
}


void Camera::setupProjectionMatrix()
{
  gluPerspective(_aperture, float(_pixelWidth) / float(_pixelHeight), 0.0001, 10000.0);
}


void Camera::setupModelViewMatrix()
{
  gluLookAt(_pos.x, _pos.y, _pos.z, _target.x, _target.y, _target.z, _up.x, _up.y, _up.z);
}


} // namespace vgl

