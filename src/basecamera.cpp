#include "vgl/basecamera.h"

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


void BaseCamera::centerView(const Vec3f& low, const Vec3f& high)
{
  _target = (high + low) / 2.0f;
  float distance = (high.z - low.z) / 10.0f;
  _pos = _target + Vec3f(0, 0, distance);
  _up = Vec3f(0, 1, 0);
}


void BaseCamera::frontView(const Vec3f& low, const Vec3f& high)
{
  _target = (high + low) / 2.0f;
  float distance = (high.z - low.z) / 2.0f +
      distanceFrom(high.x, low.x, high.y, low.y);
  _pos = _target + Vec3f(0, 0, distance);
  _up = Vec3f(0, 1, 0);
}


void BaseCamera::backView(const Vec3f& low, const Vec3f& high)
{
  _target = (high + low) / 2.0f;
  float distance = (high.z - low.z) / 2.0f +
      distanceFrom(high.x, low.x, high.y, low.y);
  _pos = _target + Vec3f(0, 0, -distance);
  _up = Vec3f(0, 1, 0);
}


void BaseCamera::leftView(const Vec3f& low, const Vec3f& high)
{
  _target = (high + low) / 2.0f;
  float distance = (high.x - low.x) / 2.0f +
      distanceFrom(high.z, low.z, high.y, low.y);
  _pos = _target + Vec3f(-distance, 0, 0);
  _up = Vec3f(0, 1, 0);
}


void BaseCamera::rightView(const Vec3f& low, const Vec3f& high)
{
  _target = (high + low) / 2.0f;
  float distance = (high.x - low.x) / 2.0f +
      distanceFrom(high.z, low.z, high.y, low.y);
  _pos = _target + Vec3f(distance, 0, 0);
  _up = Vec3f(0, 1, 0);
}


void BaseCamera::topView(const Vec3f& low, const Vec3f& high)
{
  _target = (high + low) / 2.0f;
  float distance = (high.y - low.y) +
      distanceFrom(high.x, low.x, high.z, low.z);
  _pos = _target + Vec3f(0, distance, 0);
  _up = Vec3f(0, 0, -1);
}


void BaseCamera::bottomView(const Vec3f& low, const Vec3f& high)
{
  _target = (high + low) / 2.0f;
  float distance = (high.y - low.y) +
      distanceFrom(high.x, low.x, high.z, low.z);
  _pos = _target + Vec3f(0, -distance, 0);
  _up = Vec3f(0, 0, 1);
}


float BaseCamera::distanceFrom(float highU, float lowU, float highV, float lowV) const
{
  float opposite = std::max(highU - lowU, highV - lowV) / 2.0;
  float angle = (_aperture / 2.0) * M_PI / 180.0;
  float adjacent = (opposite / tanf(angle));
  return adjacent;
}


void BaseCamera::setupProjectionMatrix()
{
  float distance = length(_target - _pos);
  gluPerspective(_aperture, float(_pixelWidth) / float(_pixelHeight),
      distance * 0.1, distance * 2.0);
}


void BaseCamera::setupModelViewMatrix()
{
  gluLookAt(_pos.x, _pos.y, _pos.z, _target.x, _target.y, _target.z, _up.x, _up.y, _up.z);
}


} // namespace vgl

