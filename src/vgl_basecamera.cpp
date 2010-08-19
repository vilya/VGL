#include "vgl_basecamera.h"

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

BaseCamera::BaseCamera(unsigned int pixelWidth, unsigned int pixelHeight) :
  _pos(Vec3f(0, 0, 10)),
  _target(Vec3f(0, 0, 0)),
  _up(Vec3f(0, 1, 0)), 
  _left(-1),
  _right(1),
  _bottom(-1),
  _top(-1),
  _aperture(30),
  _pixelWidth(pixelWidth),
  _pixelHeight(pixelHeight)
{
}


BaseCamera::BaseCamera(const Vec3f& pos, const Vec3f& target, const Vec3f& up,
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
}


BaseCamera::~BaseCamera()
{
}


void BaseCamera::pan(int prevX, int prevY, int currX, int currY)
{
  float dx = (currX - prevX) / 2.0f;
  float dy = (currY - prevY) / 2.0f;

  Vec3f viewVec = _target - _pos;
  viewVec = rotateX(viewVec, dy / (float)(2 * M_PI));
  viewVec = rotateY(viewVec, dx / (float)(2 * M_PI));
  _target = viewVec + _pos;
}


void BaseCamera::roll(int prevX, int prevY, int currX, int currY)
{
  float dx = (currX - prevX) / 2.0f;
  float dy = (currY - prevY) / 2.0f;

  Vec3f viewVec = _pos - _target;
  viewVec = rotateX(viewVec, dy / (float)(2 * M_PI));
  viewVec = rotateY(viewVec, dx / (float)(2 * M_PI));
  _pos = viewVec + _target;
}


void BaseCamera::move(int prevX, int prevY, int currX, int currY)
{
  float dx = (currX - prevX) / (float)_pixelWidth;
  float dy = (currY - prevY) / (float)_pixelHeight;

  Vec3f delta(dx, dy, 0);
  _pos += delta;
  _target += delta;
}


void BaseCamera::dolly(int prevX, int prevY, int currX, int currY)
{
  float dx = (currX - prevX) / (float)_pixelWidth;
  float dy = (currY - prevY) / (float)_pixelHeight;
  float dz = (abs(dx) >= abs(dy)) ? dx : -dy;

  dz *= 2;
  _pos.z += dz;
  _target.z += dz;
}


void BaseCamera::zoom(int prevX, int prevY, int currX, int currY)
{
  float dx = (currX - prevX) / (float)_pixelWidth;
  float dy = (currY - prevY) / (float)_pixelHeight;
  float dz = (abs(dx) >= abs(dy)) ? dx : -dy;

  dz *= -2;
  _aperture *= powf(1.1, dz);
}


void BaseCamera::setupProjectionMatrix()
{
  float distance = length(_target - _pos);
  gluPerspective(_aperture, float(_pixelWidth) / float(_pixelHeight), distance * 0.1, distance * 2.0);
}


void BaseCamera::setupModelViewMatrix()
{
  gluLookAt(_pos.x, _pos.y, _pos.z, _target.x, _target.y, _target.z, _up.x, _up.y, _up.z);
}


} // namespace vgl

