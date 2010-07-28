#include "vgl_math.h"

#include <algorithm>
#include <math.h>

namespace vgl {

//
// FUNCTIONS
//

float clampf(float k)
{
  return std::max(0.0f, std::min(1.0f, k));
}


float sqr(float k)
{
  return k * k;
}


//
// Float3 OPERATORS AND FUNCTIONS
//

Float3 operator - (const Float3& a)
{
  return Float3(-a.x, -a.y, -a.z);
}


Float3 operator + (const Float3& a, const Float3& b)
{
  return Float3(a.x + b.x, a.y + b.y, a.z + b.z);
}


Float3 operator - (const Float3& a, const Float3& b)
{
  return Float3(a.x - b.x, a.y - b.y, a.z - b.z);
}


Float3 operator * (const Float3& a, const Float3& b)
{
  return Float3(a.x * b.x, a.y * b.y, a.z * b.z);
}


Float3 operator * (const Float3& a, float k)
{
  return Float3(a.x * k, a.y * k, a.z * k);
}


Float3 operator * (float k, const Float3& a)
{
  return Float3(a.x * k, a.y * k, a.z * k);
}


Float3 operator / (const Float3& a, float k)
{
  return Float3(a.x / k, a.y / k, a.z / k);
}


Float3 pow(const Float3& a, float k)
{
  return Float3(powf(a.x, k), powf(a.y, k), powf(a.z, k));
}


const Float3& operator += (Float3& a, const Float3& b)
{
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  return a;
}


const Float3& operator -= (Float3& a, const Float3& b)
{
  a.x -= b.x;
  a.y -= b.y;
  a.z -= b.z;
  return a;
}


const Float3& operator *= (Float3& a, const Float3& b)
{
  a.x *= b.x;
  a.y *= b.y;
  a.z *= b.z;
  return a;
}


const Float3& operator *= (Float3& a, float k)
{
  a.x *= k;
  a.y *= k;
  a.z *= k;
  return a;
}


const Float3& operator /= (Float3& a, const Float3& b)
{
  a.x /= b.x;
  a.y /= b.y;
  a.z /= b.z;
  return a;
}


const Float3& operator /= (Float3& a, float k)
{
  a.x /= k;
  a.y /= k;
  a.z /= k;
  return a;
}


float sum(const Float3& a)
{
  return a.x + a.y + a.z;
}


float dot(const Float3& a, const Float3& b)
{
  return sum(a * b);
}


float lengthSqr(const Float3& a)
{
  return dot(a, a);
}


float length(const Float3& a)
{
  return sqrtf(lengthSqr(a));
}


Float3 pairwiseMin(const Float3& a, const Float3& b)
{
  return Float3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}


Float3 pairwiseMax(const Float3& a, const Float3& b)
{
  return Float3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}


Float3 cross(const Float3& a, const Float3& b)
{
  return Float3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}


Float3 norm(const Float3& a)
{
  return a / length(a);
}


Float3 clamp(const Float3& a)
{
  return Float3(clampf(a.x), clampf(a.y), clampf(a.z));
}


float scalarTriple(const Float3& a, const Float3& b, const Float3& c)
{
  return dot(a, cross(b, c));
}


Ray3 reflect(const Ray3& r, const Float3& hitpos, const Float3& normal)
{
  return Ray3(hitpos, r.d - 2 * dot(r.d, normal) * normal);
}


Ray3 refract(const Ray3& r, const Float3& hitpos, const Float3& normal, float oldNi, float newNi)
{
  float n = oldNi / newNi;
  float cosThetaI = dot(normal, -r.d);
  float sin2ThetaT = sqr(n) * (1.0 - sqr(cosThetaI));
  return Ray3(hitpos, n * r.d + (n * cosThetaI - sqrtf(1.0 - sin2ThetaT)) * normal);
}


Float3 planePos(const Plane3& p, float u, float v)
{
  return p.corner + u * p.u + v * p.v;
}


} // namespace vgl

