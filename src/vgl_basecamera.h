#ifndef vgl_basecamera_h
#define vgl_basecamera_h

#include "vgl_camera.h"
#include "vgl_vec3.h"

namespace vgl {


class BaseCamera : public Camera {
public:
  BaseCamera(unsigned int pixelWidth, unsigned int pixelHeight);
  BaseCamera(const Vec3f& pos, const Vec3f& target, const Vec3f& up,
    float left, float right, float bottom, float top, float aperture,
    unsigned int pixelWidth, unsigned int pixelHeight);
  virtual ~BaseCamera();

  virtual void pan(int prevX, int prevY, int currX, int currY);
  virtual void roll(int prevX, int prevY, int currX, int currY);
  virtual void move(int prevX, int prevY, int currX, int currY);
  virtual void dolly(int prevX, int prevY, int currX, int currY);
  virtual void zoom(int prevX, int prevY, int currX, int currY);

  virtual void centerView(const Vec3f& low, const Vec3f& high);
  virtual void frontView(const Vec3f& low, const Vec3f& high);
  virtual void backView(const Vec3f& low, const Vec3f& high);
  virtual void leftView(const Vec3f& low, const Vec3f& high);
  virtual void rightView(const Vec3f& low, const Vec3f& high);
  virtual void topView(const Vec3f& low, const Vec3f& high);
  virtual void bottomView(const Vec3f& low, const Vec3f& high);

  virtual void setupProjectionMatrix();
  virtual void setupModelViewMatrix();

protected:
  float distanceFrom(float highU, float lowU, float highV, float lowV) const;

protected:
  Vec3f _pos, _target, _up;
  float _left, _right, _bottom, _top;
  float _aperture;
  unsigned int _pixelWidth, _pixelHeight;
};


} // namespace vgl

#endif // vgl_basecamera_h

