#ifndef vgl_ray_h
#define vgl_ray_h

#include "vgl_vec3.h"

namespace vgl {

//
// TYPES
//

struct Ray3 {
  Vec3f o, d;  // o is the origin, d is the direction vector.

  Ray3() : o(), d() {}
  Ray3(const Vec3f& iO, const Vec3f& iD) : o(iO), d(iD) {}
  Ray3(const Ray3& a) : o(a.o), d(a.d) {}
};


//
// FUNCTIONS
//

Ray3 reflect(const Ray3& r, const Vec3f& hitpos, const Vec3f& normal);
Ray3 refract(const Ray3& r, const Vec3f& hitpos, const Vec3f& normal, float oldNi, float newNi);

} // namespace vgl

#endif // vgl_ray_h

