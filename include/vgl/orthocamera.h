#ifndef vgl_orthocamera_h
#define vgl_orthocamera_h

#include "vgl/basecamera.h"
#include "vgl/public.h"

namespace vgl {

  class PUBLIC OrthoCamera : public BaseCamera {
  public:
    OrthoCamera(unsigned int pixelWidth, unsigned int pixelHeight);

    virtual void setupProjectionMatrix();
    virtual void setupModelViewMatrix();
  };

} // namespace vgl

#endif // vgl_orthocamera_h

