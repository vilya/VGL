#ifndef vgl_funcs_h
#define vgl_funcs_h

#include "vgl_vec3.h"

namespace vgl {


//
// OpenGL helper functions.
//

vgl::Vec3f unproject(double x, double y, double z = 0.5);


} // namespace vgl

#endif // vgl_funcs_h

