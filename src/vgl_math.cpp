#include "vgl_math.h"

#include <math.h>

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
  return Vec3f(clampf(a.x), clampf(a.y), clampf(a.z));
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


Ray3 reflect(const Ray3& r, const Vec3f& hitpos, const Vec3f& normal)
{
  return Ray3(hitpos, r.d - 2 * dot(r.d, normal) * normal);
}


Ray3 refract(const Ray3& r, const Vec3f& hitpos, const Vec3f& normal, float oldNi, float newNi)
{
  float n = oldNi / newNi;
  float cosThetaI = dot(normal, -r.d);
  float sin2ThetaT = sqr(n) * (1.0 - sqr(cosThetaI));
  return Ray3(hitpos, n * r.d + (n * cosThetaI - sqrtf(1.0 - sin2ThetaT)) * normal);
}


Vec3f planePos(const Plane3& p, float u, float v)
{
  return p.corner + u * p.u + v * p.v;
}


} // namespace vgl

