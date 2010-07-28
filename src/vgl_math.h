#ifndef vgl_math_h
#define vgl_math_h


namespace vgl {

//
// TYPES
//

template <typename Num>
struct Triple {
  union {
    struct { Num x, y, z; };
    struct { Num r, g, b; };
    Num data[3];
  };

  Triple() : x(0), y(0), z(0) {}
  Triple(Num k) : x(k), y(k), z(k) {}
  Triple(Num iX, Num iY, Num iZ) : x(iX), y(iY), z(iZ) {}
  Triple(const Triple& a) : x(a.x), y(a.y), z(a.z) {}

  Num operator [] (unsigned int i) const { return data[i]; }
  Num& operator [] (unsigned int i) { return data[i]; }
};

typedef Triple<float> Float3;
typedef Triple<int> Int3;


struct Ray3 {
  Float3 o, d;  // o is the origin, d is the direction vector.

  Ray3() : o(), d() {}
  Ray3(const Float3& iO, const Float3& iD) : o(iO), d(iD) {}
  Ray3(const Ray3& a) : o(a.o), d(a.d) {}
};


struct Plane3 {
  Float3 corner;
  Float3 u, v;

  Plane3() : corner(), u(1, 0, 0), v(0, 1, 0) {}
  Plane3(Float3 iCorner, Float3 iU, Float3 iV) : corner(iCorner), u(iU), v(iV) {}
};


//
// FUNCTIONS
//

float clampf(float k);
float sqr(float k);

Float3 operator - (const Float3& a);
Float3 operator + (const Float3& a, const Float3& b);
Float3 operator - (const Float3& a, const Float3& b);
Float3 operator * (const Float3& a, const Float3& b);
Float3 operator * (const Float3& a, float k);
Float3 operator * (float k, const Float3& a);
Float3 operator / (const Float3& a, float k);
Float3 pow(const Float3& a, float k);

const Float3& operator += (Float3& a, const Float3& b);
const Float3& operator -= (Float3& a, const Float3& b);
const Float3& operator *= (Float3& a, const Float3& b);
const Float3& operator *= (Float3& a, float k);
const Float3& operator /= (Float3& a, const Float3& b);
const Float3& operator /= (Float3& a, float k);

float sum(const Float3& a);
float dot(const Float3& a, const Float3& b);
float lengthSqr(const Float3& a);
float length(const Float3& a);

Float3 pairwiseMin(const Float3& a, const Float3& b);
Float3 pairwiseMax(const Float3& a, const Float3& b);
Float3 cross(const Float3& a, const Float3& b);
Float3 norm(const Float3& a);
Float3 clamp(const Float3& a);

float scalarTriple(const Float3& a, const Float3& b, const Float3& c);
Ray3 reflect(const Ray3& r, const Float3& hitpos, const Float3& normal);
Ray3 refract(const Ray3& r, const Float3& hitpos, const Float3& normal, float oldNi, float newNi);

Float3 planePos(const Plane3& p, float u, float v);

} // namespace vgl

#endif // vgl_math_h

