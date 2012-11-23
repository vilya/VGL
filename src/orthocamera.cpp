#include "vgl/orthocamera.h"

namespace vgl {

 
//
// OrthoCamera METHODS
//

OrthoCamera::OrthoCamera(unsigned int pixelWidth, unsigned int pixelHeight) :
  BaseCamera(pixelWidth, pixelHeight)
{
}


void OrthoCamera::setupProjectionMatrix()
{
  int viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glOrtho(viewport[0], viewport[2], viewport[1], viewport[3], -0.5, 0.5);
}


void OrthoCamera::setupModelViewMatrix()
{
}


} // namespace vgl

