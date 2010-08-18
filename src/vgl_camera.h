#ifndef vgl_camera_h
#define vgl_camera_h

#include "vgl_vec3.h"

namespace vgl {


class Camera {
public:
  Camera(unsigned int pixelWidth, unsigned int pixelHeight);
  Camera(const Vec3f& pos, const Vec3f& target, const Vec3f& up,
    float left, float right, float bottom, float top, float aperture,
    unsigned int pixelWidth, unsigned int pixelHeight);
  virtual ~Camera();

  virtual void pan(int prevX, int prevY, int currX, int currY);
  virtual void roll(int prevX, int prevY, int currX, int currY);
  virtual void move(int prevX, int prevY, int currX, int currY);
  virtual void dolly(int prevX, int prevY, int currX, int currY);
  virtual void zoom(int prevX, int prevY, int currX, int currY);

  virtual void setupProjectionMatrix();
  virtual void setupModelViewMatrix();

protected:
  Vec3f _pos, _target, _up;
  float _left, _right, _bottom, _top;
  float _aperture;
  unsigned int _pixelWidth, _pixelHeight;
};


} // namespace vgl

#endif // vgl_camera_h

