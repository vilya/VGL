#ifndef vgl_ray_h
#define vgl_ray_h

#include "vgl/utils.h"

#include "Eigen/Dense"

#include <cmath>

namespace vgl {

  //
  // TYPES
  //
  
  template <typename Num>
  struct Ray3 {
    Eigen::Matrix<Num, 3, 1> o, d;  // o is the origin, d is the direction vector.
  
    Ray3() : o(), d() {}
    Ray3(const Eigen::Matrix<Num, 3, 1>& iO, const Eigen::Matrix<Num, 3, 1>& iD) : o(iO), d(iD) {}
    Ray3(const Ray3& a) : o(a.o), d(a.d) {}
  };
  
  
  typedef Ray3<float> Ray3f;
  typedef Ray3<double> Ray3d;
  
  
  //
  // FUNCTIONS
  //
  
  template <typename Num>
  Eigen::Matrix<Num, 3, 1> evaluate(const Ray3<Num>& r, Num t)
  {
    return r.o + t * r.d;
  }
  
  
  template <typename Num>
  Ray3<Num> reflect(const Ray3<Num>& r, const Eigen::Matrix<Num, 3, 1>& hitpos, const Eigen::Matrix<Num, 3, 1>& normal)
  {
    return Ray3<Num>(hitpos, r.d - 2 * dot(r.d, normal) * normal);
  }
  
  
  template <typename Num>
  Ray3<Num> refract(const Ray3<Num>& r, const Eigen::Matrix<Num, 3, 1>& hitpos, const Eigen::Matrix<Num, 3, 1>& normal, Num oldNi, Num newNi)
  {
    Num n = oldNi / newNi;
    Num cosThetaI = dot(normal, -r.d);
    Num sin2ThetaT = sqr(n) * (1 - sqr(cosThetaI));
    return Ray3<Num>(hitpos, n * r.d + (n * cosThetaI - std::sqrt(1 - sin2ThetaT)) * normal);
  }
  
  
  template <typename Num>
  bool intersectRaySphere(const Ray3<Num>& ray, const Eigen::Matrix<Num, 3, 1>& sphereCenter, Num sphereRadius, Eigen::Matrix<Num, 3, 1>& hitPoint)
  {
    const Num kMinT = 1e-4;
    const Num kMaxT = 1e10;
  
  	Eigen::Matrix<Num, 3, 1> m(ray.o - sphereCenter);
  
  	Num a = ray.d.squaredNorm(); // this is the same as dot(ray.d, ray.d)
  	Num b = 2 * ray.d.dot(m);
  	Num c = m.squaredNorm() - sphereRadius * sphereRadius;
  
  	Num discriminant = b * b - 4 * a * c;
  	if (discriminant > 0) {
  		float sqrt_discriminant = std::sqrt(discriminant);
  		float t = (-b - sqrt_discriminant) / (2 * a);
  		if (t < kMinT)
  			t = (-b + sqrt_discriminant) / (2 * a);
  		if (t < kMinT || t > kMaxT)
  			return false;
  
      hitPoint = evaluate(ray, t);
  		return true;
  	}
  
  	return false;
  }

} // namespace vgl

#endif // vgl_ray_h

