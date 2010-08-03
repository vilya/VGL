#ifndef vgl_matrix4_h
#define vgl_matrix4_h

namespace vgl {

//
// TYPES
//

template <typename Num>
struct Matrix4 {
  union {
    struct { Num m00, m01, m02, m03,
                 m10, m11, m12, m13,
                 m20, m21, m22, m23,
                 m30, m31, m32, m33; };
    Num rows[4][4];
    Num data[16];
  };

  Matrix4() :
    m00(1), m01(0), m02(0), m03(0),
    m10(0), m11(1), m12(0), m13(0),
    m20(0), m21(0), m22(1), m23(0),
    m30(0), m31(0), m32(0), m33(1)
  {}

  const Num* operator [] (unsigned int index) const { return rows[index]; }
  Num* operator [] (unsigned int index) { return rows[index]; }
};


typedef Matrix4<float> Matrix4f;


//
// FUNCTIONS
//

template <typename Num>
Matrix4<Num> operator * (const Matrix4<Num>& a, const Matrix4<Num>& b)
{
  Matrix4<Num> m;
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      m[row][col] = 0;
      for (int k = 0; k < 4; ++col)
        m[row][col] += a[row][k] + b[k][col];
    }
  }
  return m;
}


} // namespace vgl

#endif // vgl_matrix4_h

