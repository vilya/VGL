#include "vgl_vec3.h"

#include "vgl_utils.h"

#include <cmath>

namespace vgl {


//
// Vec3f FUNCTIONS
//

float lengthSqr(const Vec3f& a)
{
  return dot(a, a);
}


float length(const Vec3f& a)
{
  return sqrtf(lengthSqr(a));
}


Vec3f pow(const Vec3f& a, float k)
{
  return Vec3f(powf(a.x, k), powf(a.y, k), powf(a.z, k));
}


Vec3f norm(const Vec3f& a)
{
  return a / length(a);
}


Vec3f clamp(const Vec3f& a)
{
  return Vec3f(clamp(a.x), clamp(a.y), clamp(a.z));
}


Vec3f rotateX(const Vec3f& a, float radians)
{
  float c = cosf(radians);
  float s = sinf(radians);
  return Vec3f(a.x, c * a.y - s * a.z, s * a.y + c * a.z);
}


Vec3f rotateY(const Vec3f& a, float radians)
{
  float c = cosf(radians);
  float s = sinf(radians);
  return Vec3f(c * a.x + s * a.z, a.y, -s * a.x + c * a.z);
}


Vec3f rotateZ(const Vec3f& a, float radians)
{
  float c = cosf(radians);
  float s = sinf(radians);
  return Vec3f(c * a.x - s * a.y, s * a.x + c * a.y, a.z);
}


} // namespace vgl

