#ifndef vgl_vec2_h
#define vgl_vec2_h

#include "vgl_utils.h"

#include <algorithm>
#include <cmath>

namespace vgl {

//
// TYPES
//

template <typename Num>
struct Vec2 {
  union {
    struct { Num x, y; };
    struct { Num u, v; };
    Num data[2];
  };

  Vec2() : x(0), y(0) {}
  Vec2(Num k) : x(k), y(k) {}
  Vec2(Num iX, Num iY) : x(iX), y(iY) {}
  Vec2(const Vec2& a) : x(a.x), y(a.y) {}

  Num operator [] (unsigned int i) const { return data[i]; }
  Num& operator [] (unsigned int i) { return data[i]; }
};

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec2<int> Vec2i;
typedef Vec2<unsigned int> Vec2ui;


//
// Vec2 OPERATORS AND FUNCTIONS
//

template <typename Num>
Vec2<Num> operator - (const Vec2<Num>& a)
{
  return Vec2<Num>(-a.x, -a.y);
}


template <typename Num>
Vec2<Num> operator + (const Vec2<Num>& a, const Vec2<Num>& b)
{
  return Vec2<Num>(a.x + b.x, a.y + b.y);
}


template <typename Num>
Vec2<Num> operator - (const Vec2<Num>& a, const Vec2<Num>& b)
{
  return Vec2<Num>(a.x - b.x, a.y - b.y);
}


template <typename Num>
Vec2<Num> operator * (const Vec2<Num>& a, const Vec2<Num>& b)
{
  return Vec2<Num>(a.x * b.x, a.y * b.y);
}


template <typename Num>
Vec2<Num> operator * (const Vec2<Num>& a, Num k)
{
  return Vec2<Num>(a.x * k, a.y * k);
}


template <typename Num>
Vec2<Num> operator * (Num k, const Vec2<Num>& a)
{
  return Vec2<Num>(a.x * k, a.y * k);
}


template <typename Num>
Vec2<Num> operator / (const Vec2<Num>& a, const Vec2<Num>& b)
{
  return Vec2<Num>(a.x / b.x, a.y / b.y);
}


template <typename Num>
Vec2<Num> operator / (const Vec2<Num>& a, Num k)
{
  return Vec2<Num>(a.x / k, a.y / k);
}


template <typename Num>
const Vec2<Num>& operator += (Vec2<Num>& a, const Vec2<Num>& b)
{
  a.x += b.x;
  a.y += b.y;
  return a;
}


template <typename Num>
const Vec2<Num>& operator -= (Vec2<Num>& a, const Vec2<Num>& b)
{
  a.x -= b.x;
  a.y -= b.y;
  return a;
}


template <typename Num>
const Vec2<Num>& operator *= (Vec2<Num>& a, const Vec2<Num>& b)
{
  a.x *= b.x;
  a.y *= b.y;
  return a;
}


template <typename Num>
const Vec2<Num>& operator *= (Vec2<Num>& a, Num k)
{
  a.x *= k;
  a.y *= k;
  return a;
}


template <typename Num>
const Vec2<Num>& operator /= (Vec2<Num>& a, const Vec2<Num>& b)
{
  a.x /= b.x;
  a.y /= b.y;
  return a;
}


template <typename Num>
const Vec2<Num>& operator /= (Vec2<Num>& a, Num k)
{
  a.x /= k;
  a.y /= k;
  return a;
}


template <typename Num>
Num sum(const Vec2<Num>& a)
{
  return a.x + a.y;
}


template <typename Num>
Num dot(const Vec2<Num>& a, const Vec2<Num>& b)
{
  return a.x * b.x + a.y * b.y;
}


template <typename Num>
Vec2<Num> pairwiseMin(const Vec2<Num>& a, const Vec2<Num>& b)
{
  return Vec2<Num>(std::min(a.x, b.x), std::min(a.y, b.y));
}


template <typename Num>
Vec2<Num> pairwiseMax(const Vec2<Num>& a, const Vec2<Num>& b)
{
  return Vec2<Num>(std::max(a.x, b.x), std::max(a.y, b.y));
}


template <typename Num>
unsigned int minIndex(const Vec2<Num>& a)
{
  return (a[0] <= a[1]) ? 0 : 1;
}


template <typename Num>
unsigned int maxIndex(const Vec2<Num>& a)
{
  return (a[0] >= a[1]) ? 0 : 1;
}


template <typename Num>
Num lengthSqr(const Vec2<Num>& a)
{
  return dot(a, a);
}


template <typename Num>
Num length(const Vec2<Num>& a)
{
  return std::sqrt(lengthSqr(a));
}


template <typename Num>
Vec2<Num> pow(const Vec2<Num>& a, Num k)
{
  return Vec2<Num>(std::pow(a.x, k), std::pow(a.y, k));
}


template <typename Num>
Vec2<Num> norm(const Vec2<Num>& a)
{
  return a / length(a);
}


template <typename Num>
Vec2<Num> clamp(const Vec2<Num>& a)
{
  return Vec2<Num>(clamp(a.x), clamp(a.y));
}


template <typename Num>
Vec2<Num> rotate(const Vec2<Num>& a, Num radians)
{
  Num c = std::cos(radians);
  Num s = std::sin(radians);
  return Vec2<Num>(c * a.x - s * a.y, s * a.x + c * a.y);
}


} // namespace vgl

#endif // vgl_vec2_h

