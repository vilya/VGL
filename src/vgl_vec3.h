#ifndef vgl_vec3_h
#define vgl_vec3_h

#include <algorithm>

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
typedef Vec3<int> Vec3i;


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
Vec3<Num> operator * (const Vec3<Num>& a, float k)
{
  return Vec3<Num>(a.x * k, a.y * k, a.z * k);
}


template <typename Num>
Vec3<Num> operator * (float k, const Vec3<Num>& a)
{
  return Vec3<Num>(a.x * k, a.y * k, a.z * k);
}


template <typename Num>
Vec3<Num> operator / (const Vec3<Num>& a, const Vec3<Num>& b)
{
  return Vec3<Num>(a.x / b.x, a.y / b.y, a.z / b.z);
}


template <typename Num>
Vec3<Num> operator / (const Vec3<Num>& a, float k)
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
float dot(const Vec3<Num>& a, const Vec3<Num>& b)
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


//
// Float3 FUNCTIONS
//

float lengthSqr(const Vec3f& a);
float length(const Vec3f& a);

Vec3f pow(const Vec3f& a, float k);
Vec3f norm(const Vec3f& a);
Vec3f clamp(const Vec3f& a);
Vec3f rotateX(const Vec3f& a, float radians);
Vec3f rotateY(const Vec3f& a, float radians);
Vec3f rotateZ(const Vec3f& a, float radians);

} // namespace vgl

#endif // vgl_vec3_h

