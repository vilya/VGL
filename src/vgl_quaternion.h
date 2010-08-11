#ifndef vgl_quaternion_h
#define vgl_quaternion_h

#include "vgl_vec3.h"
#include "vgl_vec4.h"

namespace vgl {

//
// TYPES
//

template <typename Num>
struct Quaternion {
  Vec3<Num> v;
  Num s;

  Quaternion() : v(), s(0) {}
  Quaternion(Num ix, Num iy, Num iz, Num iw) : v(ix, iy, iz), s(iw) {}
  Quaternion(const Vec3<Num>& iv, Num is) : v(iv), s(is) {}
  Quaternion(const Quaternion<Num>& q) : v(q.v), s(q.s) {}
};


typedef Quaternion<float> Quaternionf;
typedef Quaternion<double> Quaterniond;


//
// OPERATORS AND FUNCTIONS
//

template <typename Num>
Quaternion<Num> operator - (const Quaternion<Num>& a)
{
  return Quaternion<Num>(-a.v, -a.s);
}


template <typename Num>
Quaternion<Num> operator + (const Quaternion<Num>& a, const Quaternion<Num>& b)
{
  return Quaternion<Num>(a.v + b.v, a.s + b.s);
}


template <typename Num>
Quaternion<Num> operator - (const Quaternion<Num>& a, const Quaternion<Num>& b)
{
  return Quaternion<Num>(a.v - b.v, a.s - b.s);
}


template <typename Num>
Quaternion<Num> operator * (const Quaternion<Num>& a, const Quaternion<Num>& b)
{
  return Quaternion<Num>(
      a.s * b.v + b.s * a.v + cross(a.v, b.v),
      a.s * b.s - dot(a.v, b.v) );
}


template <typename Num>
Quaternion<Num> operator / (const Quaternion<Num>& a, Num k)
{
  return Quaternion<Num>(a.v / k, a.s / k);
}


template <typename Num>
const Quaternion<Num>& operator += (Quaternion<Num>& a, const Quaternion<Num>& b)
{
  a.v += b.v;
  a.s += b.s;
}


template <typename Num>
const Quaternion<Num>& operator -= (Quaternion<Num>& a, const Quaternion<Num>& b)
{
  a.v -= b.v;
  a.s -= b.s;
}


template <typename Num>
const Quaternion<Num>& operator *= (Quaternion<Num>& a, const Quaternion<Num>& b)
{
  a = a * b;
}


template <typename Num>
const Quaternion<Num>& operator *= (Quaternion<Num>& a, Num k)
{
  a.v *= k;
}


template <typename Num>
const Quaternion<Num>& operator /= (Quaternion<Num>& a, Num k)
{
  a.v /= k;
}


template <typename Num>
Num lengthSqr(const Quaternion<Num>& a)
{
  return lengthSqr(a.v) + sqr(a.s);
}


template <typename Num>
Num length(const Quaternion<Num>& a)
{
  return std::sqrt(lengthSqr(a));
}


template <typename Num>
Quaternion<Num> norm(const Quaternion<Num>& a)
{
  return a / length(a);
}


template <typename Num>
Quaternion<Num> conjugate(const Quaternion<Num>& a)
{
  return Quaternion<Num>(-a.v, a.s);
}


template <typename Num>
Quaternion<Num> inverse(const Quaternion<Num>& a)
{
  return conjugate(a) / lengthSqr(a);
}


// Construct a quaternion to represent a rotation around the given axis of the
// specified angle in radians.
template <typename Num>
Quaternion<Num> rotation(const Vec3<Num>& axis, Num angleInRadians)
{
  float c = std::cos(angleInRadians / 2);
  float s = std::sin(angleInRadians / 2);
  return Quaternion<Num>(s * axis, c);
}


// Rotate vector v by quaternion q.
template <typename Num>
Vec3<Num> rotate(const Quaternion<Num>& q, const Vec3<Num>& v)
{
  Quaternion<Num> result = q * Quaternion<Num>(v, 0) * conjugate(q);
  return result.v;
}


template <typename Num>
Num dot(const Quaternion<Num>& a, const Quaternion<Num>& b)
{
  return dot(a.v, b.v) + a.s * b.s;
}


} // namespace vgl

#endif // vgl_quaternion_h

