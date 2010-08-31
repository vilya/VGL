#ifndef vgl_vec3_h
#define vgl_vec3_h

#include "vgl_utils.h"

#include <algorithm>
#include <cmath>

namespace vgl {

//
// TYPES
//

template <typename Num>
struct Vec3 {
  union {
    struct { Num x, y, z; };
    struct { Num r, g, b; };
    Num data[3];
  };

  Vec3() : x(0), y(0), z(0) {}
  Vec3(Num k) : x(k), y(k), z(k) {}
  Vec3(Num iX, Num iY, Num iZ) : x(iX), y(iY), z(iZ) {}
  Vec3(const Vec3& a) : x(a.x), y(a.y), z(a.z) {}

  Num operator [] (unsigned int i) const { return data[i]; }
  Num& operator [] (unsigned int i) { return data[i]; }
};

typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<int> Vec3i;
typedef Vec3<unsigned int> Vec3ui;


//
// Vec3 OPERATORS AND FUNCTIONS
//

template <typename Num>
Vec3<Num> operator - (const Vec3<Num>& a)
{
  return Vec3<Num>(-a.x, -a.y, -a.z);
}


template <typename Num>
Vec3<Num> operator + (const Vec3<Num>& a, const Vec3<Num>& b)
{
  return Vec3<Num>(a.x + b.x, a.y + b.y, a.z + b.z);
}


template <typename Num>
Vec3<Num> operator - (const Vec3<Num>& a, const Vec3<Num>& b)
{
  return Vec3<Num>(a.x - b.x, a.y - b.y, a.z - b.z);
}


template <typename Num>
Vec3<Num> operator * (const Vec3<Num>& a, const Vec3<Num>& b)
{
  return Vec3<Num>(a.x * b.x, a.y * b.y, a.z * b.z);
}


template <typename Num>
Vec3<Num> operator * (const Vec3<Num>& a, Num k)
{
  return Vec3<Num>(a.x * k, a.y * k, a.z * k);
}


template <typename Num>
Vec3<Num> operator * (Num k, const Vec3<Num>& a)
{
  return Vec3<Num>(a.x * k, a.y * k, a.z * k);
}


template <typename Num>
Vec3<Num> operator / (const Vec3<Num>& a, const Vec3<Num>& b)
{
  return Vec3<Num>(a.x / b.x, a.y / b.y, a.z / b.z);
}


template <typename Num>
Vec3<Num> operator / (const Vec3<Num>& a, Num k)
{
  return Vec3<Num>(a.x / k, a.y / k, a.z / k);
}


template <typename Num>
const Vec3<Num>& operator += (Vec3<Num>& a, const Vec3<Num>& b)
{
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}


template <typename Num>
const Vec3<Num>& operator -= (Vec3<Num>& a, const Vec3<Num>& b)
{
  a.x -= b.x;
  a.y -= b.y;
  a.z -= b.z;
  return a;
}


template <typename Num>
const Vec3<Num>& operator *= (Vec3<Num>& a, const Vec3<Num>& b)
{
  a.x *= b.x;
  a.y *= b.y;
  a.z *= b.z;
  return a;
}


template <typename Num>
const Vec3<Num>& operator *= (Vec3<Num>& a, Num k)
{
  a.x *= k;
  a.y *= k;
  a.z *= k;
  return a;
}


template <typename Num>
const Vec3<Num>& operator /= (Vec3<Num>& a, const Vec3<Num>& b)
{
  a.x /= b.x;
  a.y /= b.y;
  a.z /= b.z;
  return a;
}


template <typename Num>
const Vec3<Num>& operator /= (Vec3<Num>& a, Num k)
{
  a.x /= k;
  a.y /= k;
  a.z /= k;
  return a;
}


template <typename Num>
Num sum(const Vec3<Num>& a)
{
  return a.x + a.y + a.z;
}


template <typename Num>
Num dot(const Vec3<Num>& a, const Vec3<Num>& b)
{
  return sum(a * b);
}


template <typename Num>
Vec3<Num> cross(const Vec3<Num>& a, const Vec3<Num>& b)
{
  return Vec3<Num>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}


template <typename Num>
Num scalarTriple(const Vec3<Num>& a, const Vec3<Num>& b, const Vec3<Num>& c)
{
  return dot(a, cross(b, c));
}


template <typename Num>
Vec3<Num> pairwiseMin(const Vec3<Num>& a, const Vec3<Num>& b)
{
  return Vec3<Num>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}


template <typename Num>
Vec3<Num> pairwiseMax(const Vec3<Num>& a, const Vec3<Num>& b)
{
  return Vec3<Num>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}


template <typename Num>
unsigned int minIndex(const Vec3<Num>& a)
{
  unsigned int index = (a[0] <= a[1]) ? 0 : 1;
  index = (a[index] <= a[2]) ? index : 2;
  return index;
}


template <typename Num>
unsigned int maxIndex(const Vec3<Num>& a)
{
  unsigned int index = (a[0] >= a[1]) ? 0 : 1;
  index = (a[index] >= a[2]) ? index : 2;
  return index;
}


template <typename Num>
Num lengthSqr(const Vec3<Num>& a)
{
  return dot(a, a);
}


template <typename Num>
Num length(const Vec3<Num>& a)
{
  return std::sqrt(lengthSqr(a));
}


template <typename Num>
Vec3<Num> pow(const Vec3<Num>& a, Num k)
{
  return Vec3<Num>(std::pow(a.x, k), std::pow(a.y, k), std::pow(a.z, k));
}


template <typename Num>
Vec3<Num> norm(const Vec3<Num>& a)
{
  return a / length(a);
}


template <typename Num>
Vec3<Num> clamp(const Vec3<Num>& a)
{
  return Vec3<Num>(clamp(a.x), clamp(a.y), clamp(a.z));
}


template <typename Num>
Vec3<Num> rotateX(const Vec3<Num>& a, Num radians)
{
  Num c = std::cos(radians);
  Num s = std::sin(radians);
  return Vec3<Num>(a.x, c * a.y - s * a.z, s * a.y + c * a.z);
}


template <typename Num>
Vec3<Num> rotateY(const Vec3<Num>& a, Num radians)
{
  Num c = std::cos(radians);
  Num s = std::sin(radians);
  return Vec3<Num>(c * a.x + s * a.z, a.y, -s * a.x + c * a.z);
}


template <typename Num>
Vec3<Num> rotateZ(const Vec3<Num>& a, Num radians)
{
  Num c = std::cos(radians);
  Num s = std::sin(radians);
  return Vec3<Num>(c * a.x - s * a.y, s * a.x + c * a.y, a.z);
}


template <typename Num>
Vec3<Num> rotate(const Vec3<Num>& a, const Vec3<Num>& axis, Num radians)
{
  Num c = std::cos(radians);
  Num s = std::cos(radians);

  Vec3<Num> n = norm(axis);
  Num x = n.x;
  Num y = n.y;
  Num z = n.z;

  Vec3<Num> out(
    ( (sqr(x) * (1 - c) + c) * a.x +
      (x * y * (1 - c) - z * s) * a.y + 
      (x * z * (1 - c) + y * s) * a.z ),

    ( (y * x * (1 - c) + z * s) * a.x +
      (sqr(y) * (1 - c) + c) * a.y +
      (y * z * (1 - c) - x * s) * a.z ),

    ( (x * z * (1 - c) - y * s) * a.x +
      (y * z * (1 - c) + x * s) * a.y +
      (sqr(z) * (1 - c) + c) * a.z )
  );
  return out;
}


} // namespace vgl

#endif // vgl_vec3_h

