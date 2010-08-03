#include "vgl_vec4.h"

#include "vgl_utils.h"

#include <cmath>

namespace vgl {


//
// Vec4f FUNCTIONS
//

float lengthSqr(const Vec4f& a)
{
  return dot(a, a);
}


float length(const Vec4f& a)
{
  return sqrtf(lengthSqr(a));
}


Vec4f pow(const Vec4f& a, float k)
{
  return Vec4f(powf(a.x, k), powf(a.y, k), powf(a.z, k), powf(a.w, k));
}


Vec4f norm(const Vec4f& a)
{
  return a / length(a);
}


Vec4f clamp(const Vec4f& a)
{
  return Vec4f(clamp(a.x), clamp(a.y), clamp(a.z), clamp(a.w));
}


Vec4f rotateX(const Vec4f& a, float radians)
{
  float c = cosf(radians);
  float s = sinf(radians);
  return Vec4f(a.x, c * a.y - s * a.z, s * a.y + c * a.z, a.w);
}


Vec4f rotateY(const Vec4f& a, float radians)
{
  float c = cosf(radians);
  float s = sinf(radians);
  return Vec4f(c * a.x + s * a.z, a.y, -s * a.x + c * a.z, a.w);
}


Vec4f rotateZ(const Vec4f& a, float radians)
{
  float c = cosf(radians);
  float s = sinf(radians);
  return Vec4f(c * a.x - s * a.y, s * a.x + c * a.y, a.z, a.w);
}


} // namespace vgl

