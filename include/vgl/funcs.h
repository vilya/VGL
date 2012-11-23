#ifndef vgl_funcs_h
#define vgl_funcs_h

#include "vgl/public.h"

#include "Eigen/Dense"

namespace vgl {

  //
  // OpenGL helper functions.
  //

  PUBLIC
  Eigen::Vector3f unproject(double x, double y, double z = 0.5);

} // namespace vgl

#endif // vgl_funcs_h

