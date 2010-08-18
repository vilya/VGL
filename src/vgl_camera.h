#ifndef vgl_camera_h
#define vgl_camera_h

namespace vgl {


class Camera {
public:
  virtual void pan(int prevX, int prevY, int currX, int currY) = 0;
  virtual void roll(int prevX, int prevY, int currX, int currY) = 0;
  virtual void move(int prevX, int prevY, int currX, int currY) = 0;
  virtual void dolly(int prevX, int prevY, int currX, int currY) = 0;
  virtual void zoom(int prevX, int prevY, int currX, int currY) = 0;

  virtual void setupProjectionMatrix() = 0;
  virtual void setupModelViewMatrix() = 0;
};


} // namespace vgl

#endif // vgl_camera_h

