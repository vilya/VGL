#ifndef vgl_funcs_h
#define vgl_funcs_h

#include "vgl/public.h"
#include "vgl/vec3.h"

namespace vgl {

  //
  // OpenGL helper functions.
  //

  PUBLIC
  Vec3f unproject(double x, double y, double z = 0.5);

} // namespace vgl

#endif // vgl_funcs_h

