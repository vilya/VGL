#ifndef vgl_matrix3_h
#define vgl_matrix3_h

namespace vgl {

//
// TYPES
//

template <typename Num>
struct Matrix3 {
  union {
    struct { Num m00, m01, m02,
                 m10, m11, m12,
                 m20, m21, m22; };
    Num rows[3][3];
    Num data[9];
  };

  Matrix3() :
    m00(1), m01(0), m02(0),
    m10(0), m11(1), m12(0),
    m20(0), m21(0), m22(1)
  {}

  const Num* operator [] (unsigned int index) const { return rows[index]; }
  Num* operator [] (unsigned int index) { return rows[index]; }
};

typedef Matrix3<float> Matrix3f;


} // namespace vgl

#define // vgl_matrix3_h

