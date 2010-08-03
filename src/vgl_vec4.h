#ifndef vgl_vec4_h
#define vgl_vec4_h

#include <algorithm>

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
    Num data[3];
  };

  Vec4() : x(0), y(0), z(0), w(1) {}
  Vec4(Num k) : x(k), y(k), z(k), w(1) {}
  Vec4(Num iX, Num iY, Num iZ, Num iW) : x(iX), y(iY), z(iZ), w(iW) {}
  Vec4(const Vec4& a) : x(a.x), y(a.y), z(a.z), w(a.w) {}

  Num operator [] (unsigned int i) const { return data[i]; }
  Num& operator [] (unsigned int i) { return data[i]; }
};

typedef Vec4<float> Vec4f;
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


//
// Vec4f FUNCTIONS
//

float lengthSqr(const Vec4f& a);
float length(const Vec4f& a);

Vec4f pow(const Vec4f& a, float k);
Vec4f norm(const Vec4f& a);
Vec4f clamp(const Vec4f& a);
Vec4f rotateX(const Vec4f& a, float radians);
Vec4f rotateY(const Vec4f& a, float radians);
Vec4f rotateZ(const Vec4f& a, float radians);

} // namespace vgl

#endif // vgl_vec4_h

