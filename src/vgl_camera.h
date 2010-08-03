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

  virtual void panBy(float dx, float dy, float dz);
  virtual void rollBy(float dx, float dy, float dz);
  virtual void moveBy(float dx, float dy, float dz);
  virtual void zoomBy(float dz);

  virtual void setupProjectionMatrix();
  virtual void setupModelViewMatrix();

private:
  Vec3f _pos, _target, _up;
  float _left, _right, _bottom, _top;
  float _aperture;
  unsigned int _pixelWidth, _pixelHeight;
};


} // namespace vgl

#endif // vgl_camera_h

