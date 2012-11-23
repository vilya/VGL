#ifndef vgl_arcballcamera_h
#define vgl_arcballcamera_h

#include "vgl/basecamera.h"
#include "vgl/public.h"
#include "vgl/vec3.h"

namespace vgl {

  //
  // ArcballCamera class
  //

  class PUBLIC ArcballCamera : public BaseCamera
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

