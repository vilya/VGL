#ifndef vgl_arcballcamera_h
#define vgl_arcballcamera_h

#include "vgl_basecamera.h"
#include "vgl_vec3.h"

namespace vgl {


class ArcballCamera : public BaseCamera
{
public:
  ArcballCamera(const Vec3f& pos, const Vec3f& target, const Vec3f& up,
      float left, float right, float bottom, float top, float aperture,
      unsigned int pixelWidth, unsigned int pixelHeight);

  virtual void roll(int prevX, int prevY, int currX, int currY);
  virtual void move(int prevX, int prevY, int currX, int currY);
};


} // namespace vgl

#endif // vgl_arcballcamera_h

