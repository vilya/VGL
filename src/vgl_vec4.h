#ifndef vgl_vec4_h
#define vgl_vec4_h

#include "vgl_utils.h"

#include <algorithm>
#include <cmath>

namespace vgl {

//
// TYPES
//

/*
 * A Vec4 is assumed to represent homogenous coordinates and the operations are
 * implemented accordingly.
 *
 * Set w = 0 for a direction vector. Set w = 1 for a point.
 */

template <typename Num>
struct Vec4 {
  union {
    struct { Num x, y, z, w; };
    struct { Num r, g, b, a; };
    Num data[4];
  };

  Vec4() : x(0), y(0), z(0), w(1) {}
  Vec4(Num k) : x(k), y(k), z(k), w(1) {}
  Vec4(Num iX, Num iY, Num iZ, Num iW) : x(iX), y(iY), z(iZ), w(iW) {}
  Vec4(const Vec4& a) : x(a.x), y(a.y), z(a.z), w(a.w) {}

  Num operator [] (unsigned int i) const { return data[i]; }
  Num& operator [] (unsigned int i) { return data[i]; }
};

typedef Vec4<float> Vec4f;
typedef Vec4<double> Vec4d;
typedef Vec4<int> Vec4i;


//
// Vec4 OPERATORS AND FUNCTIONS
//

template <typename Num>
Vec4<Num> operator - (const Vec4<Num>& a)
{
  return Vec4<Num>(-a.x, -a.y, -a.z, -a.w);
}


template <typename Num>
Vec4<Num> operator + (const Vec4<Num>& a, const Vec4<Num>& b)
{
  return Vec4<Num>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}


template <typename Num>
Vec4<Num> operator - (const Vec4<Num>& a, const Vec4<Num>& b)
{
  return Vec4<Num>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}


template <typename Num>
Vec4<Num> operator * (const Vec4<Num>& a, const Vec4<Num>& b)
{
  return Vec4<Num>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}


template <typename Num>
Vec4<Num> operator * (const Vec4<Num>& a, float k)
{
  return Vec4<Num>(a.x * k, a.y * k, a.z * k, a.w * k);
}


template <typename Num>
Vec4<Num> operator * (float k, const Vec4<Num>& a)
{
  return Vec4<Num>(a.x * k, a.y * k, a.z * k, a.w * k);
}


template <typename Num>
Vec4<Num> operator / (const Vec4<Num>& a, const Vec4<Num>& b)
{
  return Vec4<Num>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}


template <typename Num>
Vec4<Num> operator / (const Vec4<Num>& a, float k)
{
  return Vec4<Num>(a.x / k, a.y / k, a.z / k, a.w / k);
}


template <typename Num>
const Vec4<Num>& operator += (Vec4<Num>& a, const Vec4<Num>& b)
{
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  a.w += b.w;
  return a;
}


template <typename Num>
const Vec4<Num>& operator -= (Vec4<Num>& a, const Vec4<Num>& b)
{
  a.x -= b.x;
  a.y -= b.y;
  a.z -= b.z;
  a.w -= b.w;
  return a;
}


template <typename Num>
const Vec4<Num>& operator *= (Vec4<Num>& a, const Vec4<Num>& b)
{
  a.x *= b.x;
  a.y *= b.y;
  a.z *= b.z;
  a.w *= b.w;
  return a;
}


template <typename Num>
const Vec4<Num>& operator *= (Vec4<Num>& a, Num k)
{
  a.x *= k;
  a.y *= k;
  a.z *= k;
  a.w *= k;
  return a;
}


template <typename Num>
const Vec4<Num>& operator /= (Vec4<Num>& a, const Vec4<Num>& b)
{
  a.x /= b.x;
  a.y /= b.y;
  a.z /= b.z;
  a.w /= b.w;
  return a;
}


template <typename Num>
const Vec4<Num>& operator /= (Vec4<Num>& a, Num k)
{
  a.x /= k;
  a.y /= k;
  a.z /= k;
  a.w /= k;
  return a;
}


template <typename Num>
Num sum(const Vec4<Num>& a)
{
  return a.x + a.y + a.z + a.w;
}


template <typename Num>
float dot(const Vec4<Num>& a, const Vec4<Num>& b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}


template <typename Num>
Vec4<Num> cross(const Vec4<Num>& a, const Vec4<Num>& b)
{
  return Vec4<Num>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 1.0);
}


template <typename Num>
Num scalarTriple(const Vec4<Num>& a, const Vec4<Num>& b, const Vec4<Num>& c)
{
  return dot(a, cross(b, c));
}


template <typename Num>
Vec4<Num> pairwiseMin(const Vec4<Num>& a, const Vec4<Num>& b)
{
  return Vec4<Num>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
}


template <typename Num>
Vec4<Num> pairwiseMax(const Vec4<Num>& a, const Vec4<Num>& b)
{
  return Vec4<Num>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
}


template <typename Num>
Num lengthSqr(const Vec4<Num>& a)
{
  return dot(a, a);
}


template <typename Num>
Num length(const Vec4<Num>& a)
{
  return std::sqrt(lengthSqr(a));
}


template <typename Num>
Vec4<Num> pow(const Vec4<Num>& a, Num k)
{
  return Vec4<Num>(std::pow(a.x, k), std::pow(a.y, k), std::pow(a.z, k), std::pow(a.w, k));
}


template <typename Num>
Vec4<Num> norm(const Vec4<Num>& a)
{
  return a / length(a);
}


template <typename Num>
Vec4<Num> clamp(const Vec4<Num>& a)
{
  return Vec4<Num>(clamp(a.x), clamp(a.y), clamp(a.z), clamp(a.w));
}


template <typename Num>
Vec4<Num> rotateX(const Vec4<Num>& a, Num radians)
{
  Num c = std::cos(radians);
  Num s = std::sin(radians);
  return Vec4<Num>(a.x, c * a.y - s * a.z, s * a.y + c * a.z, a.w);
}


template <typename Num>
Vec4<Num> rotateY(const Vec4<Num>& a, Num radians)
{
  Num c = std::cos(radians);
  Num s = std::sin(radians);
  return Vec4<Num>(c * a.x + s * a.z, a.y, -s * a.x + c * a.z, a.w);
}


template <typename Num>
Vec4<Num> rotateZ(const Vec4<Num>& a, Num radians)
{
  Num c = std::cos(radians);
  Num s = std::sin(radians);
  return Vec4<Num>(c * a.x - s * a.y, s * a.x + c * a.y, a.z, a.w);
}


} // namespace vgl

#endif // vgl_vec4_h

