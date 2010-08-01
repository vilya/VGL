#ifndef vgl_plane_h
#define vgl_plane_h

#include "vgl_math.h"

namespace vgl {

//
// TYPES
//

struct Plane3 {
  Vec3f corner;
  Vec3f u, v;

  Plane3() : corner(), u(1, 0, 0), v(0, 1, 0) {}
  Plane3(Vec3f iCorner, Vec3f iU, Vec3f iV) : corner(iCorner), u(iU), v(iV) {}
};


//
// FUNCTIONS
//

Vec3f planePos(const Plane3& p, float u, float v);

} // namespace vgl

#endif // vgl_plane_h

