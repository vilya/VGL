#include "vgl_arcballcamera.h"

#include "vgl_funcs.h"
#include "vgl_matrix4.h"
#include "vgl_quaternion.h"
#include "vgl_ray3.h"

namespace vgl {


//
// ArcballCamera methods
//

ArcballCamera::ArcballCamera(const Vec3f& pos,
                             const Vec3f& target,
                             const Vec3f& up,
                             float left,
                             float right,
                             float bottom,
                             float top,
                             float aperture,
                             unsigned int pixelWidth,
                             unsigned int pixelHeight) :
  BaseCamera(pos, target, up, left, right, bottom, top,
      aperture, pixelWidth, pixelHeight)
{
}



void ArcballCamera::roll(int prevX, int prevY, int currX, int currY)
{
  if (prevX == currX && prevY == currY)
    return;

  prevY = (_pixelHeight - 1) - prevY;
  currY = (_pixelHeight - 1) - currY;

  vgl::Vec3f prev = unproject(prevX, prevY);
  vgl::Vec3f curr = unproject(currX, currY);

  const float kSphereRadius = 1;

  vgl::Ray3f prevRay(_pos, prev - _pos);
  vgl::Ray3f currRay(_pos, curr - _pos);

  vgl::Vec3f prevHit, currHit;
  bool didPrevHit = intersectRaySphere(prevRay, _target, kSphereRadius, prevHit);
  bool didCurrHit = intersectRaySphere(currRay, _target, kSphereRadius, currHit);
  if ( didPrevHit && didCurrHit ) {
    float c = dot(norm(currHit), norm(prevHit));
    // Need to use the inverse rotation, since we're rotating the camera
    // rather than the scene.
    vgl::Quaternionf q = inverse(vgl::rotation(
        cross(prevHit - _target, currHit - _target), std::acos(c)));
    _pos = rotate(q, _pos - _target) + _target;
  }
}


void ArcballCamera::move(int prevX, int prevY, int currX, int currY)
{
  if (prevX == currX && prevY == currY)
    return;

  // The viewer gives us y coordinates which increase downwards. We want ones
  // which increase upwards, so we need to invert them.
  prevY = (_pixelHeight - 1) - prevY;
  currY = (_pixelHeight - 1) - currY;

  vgl::Vec3f prev = unproject(prevX, prevY, 0.6);
  vgl::Vec3f curr = unproject(currX, currY, 0.6);
  vgl::Vec3f delta = curr - prev;

  // Because we're moving the camera not the scene, we need to do the
  // opposite movement.
  _pos = (_pos - delta);
  _target = (_target - delta);
}


} // namespace vgl

