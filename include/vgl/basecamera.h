#ifndef vgl_basecamera_h
#define vgl_basecamera_h

#include "vgl/camera.h"
#include "vgl/public.h"
#include "Eigen/Dense"

namespace vgl {

  class PUBLIC BaseCamera : public Camera {
  public:
    BaseCamera(unsigned int pixelWidth, unsigned int pixelHeight);
    BaseCamera(const Eigen::Vector3f& pos, const Eigen::Vector3f& target, const Eigen::Vector3f& up,
        float left, float right, float bottom, float top, float aperture,
        unsigned int pixelWidth, unsigned int pixelHeight);
    virtual ~BaseCamera();

    virtual void pan(int prevX, int prevY, int currX, int currY);
    virtual void roll(int prevX, int prevY, int currX, int currY);
    virtual void move(int prevX, int prevY, int currX, int currY);
    virtual void dolly(int prevX, int prevY, int currX, int currY);
    virtual void zoom(int prevX, int prevY, int currX, int currY);

    virtual void centerView(const Eigen::Vector3f& low, const Eigen::Vector3f& high);
    virtual void frontView(const Eigen::Vector3f& low, const Eigen::Vector3f& high);
    virtual void backView(const Eigen::Vector3f& low, const Eigen::Vector3f& high);
    virtual void leftView(const Eigen::Vector3f& low, const Eigen::Vector3f& high);
    virtual void rightView(const Eigen::Vector3f& low, const Eigen::Vector3f& high);
    virtual void topView(const Eigen::Vector3f& low, const Eigen::Vector3f& high);
    virtual void bottomView(const Eigen::Vector3f& low, const Eigen::Vector3f& high);

    virtual void setupProjectionMatrix();
    virtual void setupModelViewMatrix();

  protected:
    float distanceFrom(float highU, float lowU, float highV, float lowV) const;

  protected:
    Eigen::Vector3f _pos, _target, _up;
    float _left, _right, _bottom, _top;
    float _aperture;
    unsigned int _pixelWidth, _pixelHeight;
  };

} // namespace vgl

#endif // vgl_basecamera_h

