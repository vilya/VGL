#ifndef vgl_camera_h
#define vgl_camera_h

#include "vgl/public.h"
#include "vgl/vec3.h"

namespace vgl {

  class PUBLIC Camera {
  public:
    virtual void pan(int prevX, int prevY, int currX, int currY) = 0;
    virtual void roll(int prevX, int prevY, int currX, int currY) = 0;
    virtual void move(int prevX, int prevY, int currX, int currY) = 0;
    virtual void dolly(int prevX, int prevY, int currX, int currY) = 0;
    virtual void zoom(int prevX, int prevY, int currX, int currY) = 0;

    virtual void centerView(const Vec3f& low, const Vec3f& high) = 0;
    virtual void frontView(const Vec3f& low, const Vec3f& high) = 0;
    virtual void backView(const Vec3f& low, const Vec3f& high) = 0;
    virtual void leftView(const Vec3f& low, const Vec3f& high) = 0;
    virtual void rightView(const Vec3f& low, const Vec3f& high) = 0;
    virtual void topView(const Vec3f& low, const Vec3f& high) = 0;
    virtual void bottomView(const Vec3f& low, const Vec3f& high) = 0;

    virtual void setupProjectionMatrix() = 0;
    virtual void setupModelViewMatrix() = 0;
  };

} // namespace vgl

#endif // vgl_camera_h

