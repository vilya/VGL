#include "vgl_vec2.h"

#include "vgl_utils.h"

#include <cmath>

namespace vgl {


//
// Vec2f FUNCTIONS
//

float lengthSqr(const Vec2f& a)
{
  return dot(a, a);
}


float length(const Vec2f& a)
{
  return sqrtf(lengthSqr(a));
}


Vec2f pow(const Vec2f& a, float k)
{
  return Vec2f(powf(a.x, k), powf(a.y, k));
}


Vec2f norm(const Vec2f& a)
{
  return a / length(a);
}


Vec2f clamp(const Vec2f& a)
{
  return Vec2f(clamp(a.x), clamp(a.y));
}


Vec2f rotate(const Vec2f& a, float radians)
{
  float c = cosf(radians);
  float s = sinf(radians);
  return Vec2f(c * a.x - s * a.y, s * a.x + c * a.y);
}


} // namespace vgl

