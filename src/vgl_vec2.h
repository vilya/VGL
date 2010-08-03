#ifndef vgl_vec2_h
#define vgl_vec2_h

#include <algorithm>

namespace vgl {

//
// TYPES
//

template <typename Num>
struct Vec2 {
  union {
    struct { Num x, y; };
    struct { Num u, v; };
    Num data[3];
  };

  Vec2() : x(0), y(0) {}
  Vec2(Num k) : x(k), y(k) {}
  Vec2(Num iX, Num iY) : x(iX), y(iY) {}
  Vec2(const Vec2& a) : x(a.x), y(a.y) {}

  Num operator [] (unsigned int i) const { return data[i]; }
  Num& operator [] (unsigned int i) { return data[i]; }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;


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
Vec2<Num> operator * (const Vec2<Num>& a, float k)
{
  return Vec2<Num>(a.x * k, a.y * k);
}


template <typename Num>
Vec2<Num> operator * (float k, const Vec2<Num>& a)
{
  return Vec2<Num>(a.x * k, a.y * k);
}


template <typename Num>
Vec2<Num> operator / (const Vec2<Num>& a, const Vec2<Num>& b)
{
  return Vec2<Num>(a.x / b.x, a.y / b.y);
}


template <typename Num>
Vec2<Num> operator / (const Vec2<Num>& a, float k)
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
float dot(const Vec2<Num>& a, const Vec2<Num>& b)
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


//
// Vec2f FUNCTIONS
//

float lengthSqr(const Vec2f& a);
float length(const Vec2f& a);

Vec2f pow(const Vec2f& a, float k);
Vec2f norm(const Vec2f& a);
Vec2f clamp(const Vec2f& a);
Vec2f rotate(const Vec2f& a, float radians);

} // namespace vgl

#endif // vgl_vec2_h

