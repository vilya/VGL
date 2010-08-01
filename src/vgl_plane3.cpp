#include "vgl_plane3.h"

namespace vgl {


//
// Plane3 FUNCTIONS
//

Vec3f planePos(const Plane3& p, float u, float v)
{
  return p.corner + u * p.u + v * p.v;
}


} // namespace vgl

