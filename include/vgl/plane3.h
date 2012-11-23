#ifndef vgl_plane_h
#define vgl_plane_h

#include "Eigen/Dense"

namespace vgl {

  //
  // TYPES
  //

  template <typename Num>
  struct Plane3 {
    Eigen::Matrix<Num, 3, 1> corner;
    Eigen::Matrix<Num, 3, 1> u, v;

    Plane3() :
      corner(), u(1, 0, 0), v(0, 1, 0)
    {}

    Plane3(Eigen::Matrix<Num, 3, 1> iCorner, Eigen::Matrix<Num, 3, 1> iU, Eigen::Matrix<Num, 3, 1> iV) :
      corner(iCorner), u(iU), v(iV)
    {}
  };


  typedef Plane3<float> Plane3f;
  typedef Plane3<double> Plane3d;


  //
  // FUNCTIONS
  //

  template <typename Num>
  Eigen::Matrix<Num, 3, 1> planePos(const Plane3<Num>& p, Num u, Num v)
  {
    return p.corner + u * p.u + v * p.v;
  }

} // namespace vgl

#endif // vgl_plane_h

