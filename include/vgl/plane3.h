#ifndef vgl_plane_h
#define vgl_plane_h

#include "vgl_vec3.h"

namespace vgl {

//
// TYPES
//

template <typename Num>
struct Plane3 {
  Vec3<Num> corner;
  Vec3<Num> u, v;

  Plane3() : corner(), u(1, 0, 0), v(0, 1, 0) {}
  Plane3(Vec3<Num> iCorner, Vec3<Num> iU, Vec3<Num> iV) : corner(iCorner), u(iU), v(iV) {}
};


typedef Plane3<float> Plane3f;
typedef Plane3<double> Plane3d;


//
// FUNCTIONS
//

template <typename Num>
Vec3<Num> planePos(const Plane3<Num>& p, Num u, Num v)
{
  return p.corner + u * p.u + v * p.v;
}


} // namespace vgl

#endif // vgl_plane_h

