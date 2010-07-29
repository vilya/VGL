#include "vgl_math.h"

#include <math.h>

namespace vgl {


//
// Float3 FUNCTIONS
//

float lengthSqr(const Float3& a)
{
  return dot(a, a);
}


float length(const Float3& a)
{
  return sqrtf(lengthSqr(a));
}


Float3 pow(const Float3& a, float k)
{
  return Float3(powf(a.x, k), powf(a.y, k), powf(a.z, k));
}


Float3 norm(const Float3& a)
{
  return a / length(a);
}


Float3 clamp(const Float3& a)
{
  return Float3(clampf(a.x), clampf(a.y), clampf(a.z));
}


Ray3 reflect(const Ray3& r, const Float3& hitpos, const Float3& normal)
{
  return Ray3(hitpos, r.d - 2 * dot(r.d, normal) * normal);
}


Ray3 refract(const Ray3& r, const Float3& hitpos, const Float3& normal, float oldNi, float newNi)
{
  float n = oldNi / newNi;
  float cosThetaI = dot(normal, -r.d);
  float sin2ThetaT = sqr(n) * (1.0 - sqr(cosThetaI));
  return Ray3(hitpos, n * r.d + (n * cosThetaI - sqrtf(1.0 - sin2ThetaT)) * normal);
}


Float3 planePos(const Plane3& p, float u, float v)
{
  return p.corner + u * p.u + v * p.v;
}


} // namespace vgl

