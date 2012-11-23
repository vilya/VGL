#ifndef vgl_orthocamera_h
#define vgl_orthocamera_h

#include "basecamera.h"

namespace vgl {


class OrthoCamera : public BaseCamera
{
public:
  OrthoCamera(unsigned int pixelWidth, unsigned int pixelHeight);

  virtual void setupProjectionMatrix();
  virtual void setupModelViewMatrix();
};


} // namespace vgl

#endif // vgl_orthocamera_h

