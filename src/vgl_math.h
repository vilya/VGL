#ifndef vgl_math_h
#define vgl_math_h

#include <algorithm>

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

template <typename Num>
Num clampf(Num k)
{
  return std::max(Num(0), std::min(Num(1), k));
}


template <typename Num>
Num sqr(Num k)
{
  return k * k;
}


//
// Triple OPERATORS AND FUNCTIONS
//

template <typename Num>
Triple<Num> operator - (const Triple<Num>& a)
{
  return Triple<Num>(-a.x, -a.y, -a.z);
}


template <typename Num>
Triple<Num> operator + (const Triple<Num>& a, const Triple<Num>& b)
{
  return Triple<Num>(a.x + b.x, a.y + b.y, a.z + b.z);
}


template <typename Num>
Triple<Num> operator - (const Triple<Num>& a, const Triple<Num>& b)
{
  return Triple<Num>(a.x - b.x, a.y - b.y, a.z - b.z);
}


template <typename Num>
Triple<Num> operator * (const Triple<Num>& a, const Triple<Num>& b)
{
  return Triple<Num>(a.x * b.x, a.y * b.y, a.z * b.z);
}


template <typename Num>
Triple<Num> operator * (const Triple<Num>& a, float k)
{
  return Triple<Num>(a.x * k, a.y * k, a.z * k);
}


template <typename Num>
Triple<Num> operator * (float k, const Triple<Num>& a)
{
  return Triple<Num>(a.x * k, a.y * k, a.z * k);
}


template <typename Num>
Triple<Num> operator / (const Triple<Num>& a, float k)
{
  return Triple<Num>(a.x / k, a.y / k, a.z / k);
}


template <typename Num>
const Triple<Num>& operator += (Triple<Num>& a, const Triple<Num>& b)
{
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}


template <typename Num>
const Triple<Num>& operator -= (Triple<Num>& a, const Triple<Num>& b)
{
  a.x -= b.x;
  a.y -= b.y;
  a.z -= b.z;
  return a;
}


template <typename Num>
const Triple<Num>& operator *= (Triple<Num>& a, const Triple<Num>& b)
{
  a.x *= b.x;
  a.y *= b.y;
  a.z *= b.z;
  return a;
}


template <typename Num>
const Triple<Num>& operator *= (Triple<Num>& a, Num k)
{
  a.x *= k;
  a.y *= k;
  a.z *= k;
  return a;
}


template <typename Num>
const Triple<Num>& operator /= (Triple<Num>& a, const Triple<Num>& b)
{
  a.x /= b.x;
  a.y /= b.y;
  a.z /= b.z;
  return a;
}


template <typename Num>
const Triple<Num>& operator /= (Triple<Num>& a, Num k)
{
  a.x /= k;
  a.y /= k;
  a.z /= k;
  return a;
}


template <typename Num>
Num sum(const Triple<Num>& a)
{
  return a.x + a.y + a.z;
}


template <typename Num>
float dot(const Triple<Num>& a, const Triple<Num>& b)
{
  return sum(a * b);
}


template <typename Num>
Triple<Num> cross(const Triple<Num>& a, const Triple<Num>& b)
{
  return Triple<Num>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}


template <typename Num>
Num scalarTriple(const Triple<Num>& a, const Triple<Num>& b, const Triple<Num>& c)
{
  return dot(a, cross(b, c));
}


template <typename Num>
Triple<Num> pairwiseMin(const Triple<Num>& a, const Triple<Num>& b)
{
  return Triple<Num>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}


template <typename Num>
Triple<Num> pairwiseMax(const Triple<Num>& a, const Triple<Num>& b)
{
  return Triple<Num>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}


//
// Float3 FUNCTIONS
//

float lengthSqr(const Float3& a);
float length(const Float3& a);

Float3 pow(const Float3& a, float k);
Float3 norm(const Float3& a);
Float3 clamp(const Float3& a);

Ray3 reflect(const Ray3& r, const Float3& hitpos, const Float3& normal);
Ray3 refract(const Ray3& r, const Float3& hitpos, const Float3& normal, float oldNi, float newNi);

Float3 planePos(const Plane3& p, float u, float v);

} // namespace vgl

#endif // vgl_math_h

