#ifndef vgl_camera_h
#define vgl_camera_h

#include "vgl/public.h"

#include "Eigen/Dense"

namespace vgl {

  class PUBLIC Camera {
  public:
    virtual void pan(int prevX, int prevY, int currX, int currY) = 0;
    virtual void roll(int prevX, int prevY, int currX, int currY) = 0;
    virtual void move(int prevX, int prevY, int currX, int currY) = 0;
    virtual void dolly(int prevX, int prevY, int currX, int currY) = 0;
    virtual void zoom(int prevX, int prevY, int currX, int currY) = 0;

    virtual void centerView(const Eigen::Vector3f& low, const Eigen::Vector3f& high) = 0;
    virtual void frontView(const Eigen::Vector3f& low, const Eigen::Vector3f& high) = 0;
    virtual void backView(const Eigen::Vector3f& low, const Eigen::Vector3f& high) = 0;
    virtual void leftView(const Eigen::Vector3f& low, const Eigen::Vector3f& high) = 0;
    virtual void rightView(const Eigen::Vector3f& low, const Eigen::Vector3f& high) = 0;
    virtual void topView(const Eigen::Vector3f& low, const Eigen::Vector3f& high) = 0;
    virtual void bottomView(const Eigen::Vector3f& low, const Eigen::Vector3f& high) = 0;

    virtual void setupProjectionMatrix() = 0;
    virtual void setupModelViewMatrix() = 0;
  };

} // namespace vgl

#endif // vgl_camera_h

