#include "vgl_ray3.h"

#include "vgl_utils.h"

#include <cmath>

namespace vgl {

//
// Ray3 FUNCTIONS
//

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

} // namespace vgl

