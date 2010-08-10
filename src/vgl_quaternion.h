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
  union {
    struct { Num x, y, z, w; };
    Num data[4];
  };

  Quaternion() :
    x(0), y(0), z(0), w(0) {}
  Quaternion(Num ix, Num iy, Num iz, Num iw) :
    x(ix), y(iy), z(iz), w(iw) {}
  Quaternion(const Vec3<Num>& iv, Num iw) :
    x(iv.x), y(iv.y), z(iv.z), w(iw) {}
  explicit Quaternion(const Vec4<Num>& iv) :
    x(iv.x), y(iv.y), z(iv.z), w(iv.w) {}
  Quaternion(const Quaternion<Num>& q) :
    x(q.x), y(q.y), z(q.z), w(q.w) {}

  Num operator [] (unsigned int i) const { return data[i]; }
  Num& operator [] (unsigned int i) { return data[i]; }
};


typedef Quaternion<float> Quaternionf;
typedef Quaternion<double> Quaterniond;


//
// OPERATORS AND FUNCTIONS
//

template <typename Num>
Quaternion<Num> operator - (const Quaternion<Num>& a)
{
  // TODO
}


template <typename Num>
Quaternion<Num> operator + (const Quaternion<Num>& a, const Quaternion<Num>& b)
{
  // TODO
}


template <typename Num>
Quaternion<Num> operator - (const Quaternion<Num>& a, const Quaternion<Num>& b)
{
  // TODO
}


template <typename Num>
Quaternion<Num> operator * (const Quaternion<Num>& a, const Quaternion<Num>& b)
{
  Vec3<Num> av(a.x, a.y, a.z);
  Vec3<Num> bv(b.x, b.y, b.z);
  Vec3<Num> rv = a.w * bv + b.w * av + cross(av, bv);
  return Quaternion<Num>(rv.x, rv.y, rv.z, a.w * b.w - dot(av, bv));
}


template <typename Num>
Quaternion<Num> operator * (const Quaternion<Num>& a, Num k)
{
  // TODO
}


template <typename Num>
Quaternion<Num> operator * (Num k, const Quaternion<Num>& a)
{
  // TODO
}


template <typename Num>
Quaternion<Num> operator / (const Quaternion<Num>& a, const Quaternion<Num>& b)
{
  // TODO
}


template <typename Num>
Quaternion<Num> operator / (const Quaternion<Num>& a, Num k)
{
  // TODO
}


template <typename Num>
const Quaternion<Num>& operator += (Quaternion<Num>& a, const Quaternion<Num>& b)
{
  // TODO
}


template <typename Num>
const Quaternion<Num>& operator -= (Quaternion<Num>& a, const Quaternion<Num>& b)
{
  // TODO
}


template <typename Num>
const Quaternion<Num>& operator *= (Quaternion<Num>& a, const Quaternion<Num>& b)
{
  // TODO
}


template <typename Num>
const Quaternion<Num>& operator *= (Quaternion<Num>& a, Num k)
{
  // TODO
}


template <typename Num>
const Quaternion<Num>& operator /= (Quaternion<Num>& a, const Quaternion<Num>& b)
{
  // TODO
}


template <typename Num>
const Quaternion<Num>& operator /= (Quaternion<Num>& a, Num k)
{
  // TODO
}


template <typename Num>
Num lengthSqr(const Quaternion<Num>& a)
{
  return sqr(a.x) + sqr(a.y) + sqr(a.z) + sqr(a.w);
}


template <typename Num>
Num length(const Quaternion<Num>& a)
{
  std::sqrt(lengthSqr(a));
}


template <typename Num>
Quaternion<Num> pow(const Quaternion<Num>& a, Num k)
{
  // TODO
}


template <typename Num>
Quaternion<Num> norm(const Quaternion<Num>& a)
{
  // TODO
}


template <typename Num>
Quaternion<Num> conjugate(const Quaternion<Num>& a)
{
  return Quaternion<Num>(-a.x, -a.y, -a.z, a.w);
}


template <typename Num>
Quaternion<Num> inverse(const Quaternion<Num>& a)
{
  const Quaternion<Num> c = conjugate(a);
  return c / (a * c);
}


// Rotate vector v by quaternion q.
template <typename Num>
Vec3<Num> rotate(const Quaternion<Num>& q, const Vec3<Num>& v)
{
  // FIXME: need to define multiplication operators for quaternion * vec3  and vec3 * quaternion
  return q * v * inverse(q);
}


// Rotate vector v by quaternion q.
template <typename Num>
Vec4<Num> rotate(const Quaternion<Num>& q, const Vec4<Num>& v)
{
  // FIXME: need to define multiplication operators for quaternion * vec3  and vec3 * quaternion
  return q * v * inverse(q);
}


template <typename Num>
Quaternion<Num> slerp(const Quaternion<Num>& a, const Quaternion<Num>& b, Num k)
{
  return k * a + (1 - k) * b;
}


} // namespace vgl

#endif // vgl_quaternion_h

