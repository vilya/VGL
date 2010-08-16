#include "vgl.h"

class ArcballRenderer : public vgl::Renderer
{
public:
  ArcballRenderer() {}

  virtual void setup() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
  }

  virtual void render() {
    static const float kTeapotColor[] = { 0.6, 0.6, 0.6, 1.0 };
    static const float kSphereColor[] = { 0.3, 0.3, 0.8, 0.5 };

    glEnable(GL_LIGHTING);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, kTeapotColor);
    glutSolidTeapot(0.5);

    glDisable(GL_LIGHTING);
    glColor4fv(kSphereColor);
    glutWireSphere(1, 20, 20);
  }
};


class ArcballCamera : public vgl::Camera
{
public:
  ArcballCamera(const vgl::Vec3f& pos, const vgl::Vec3f& target, const vgl::Vec3f& up,
      float left, float right, float bottom, float top, float aperture,
      unsigned int pixelWidth, unsigned int pixelHeight) :
    vgl::Camera(pos, target, up, left, right, bottom, top, aperture, pixelWidth, pixelHeight)
  {}


  vgl::Vec3f unproject(double x, double y, double z = 0)
  {
    vgl::Matrix4d projectionMatrix;
    vgl::Matrix4d modelViewMatrix;
    vgl::Vec4i viewportMatrix;

    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix.data);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix.data);
    glGetIntegerv(GL_VIEWPORT, viewportMatrix.data);

    vgl::Vec3d point;
    gluUnProject(x, y, z,
        modelViewMatrix.data, projectionMatrix.data, viewportMatrix.data,
        &point.x, &point.y, &point.z);

    return vgl::Vec3f((float)point.x, (float)point.y, (float)point.z);
  }


  void arcballRoll(int prevX, int prevY, int currX, int currY)
  {
    if (prevX == currX && prevY == currY)
      return;

    vgl::Vec3f prev = unproject(prevX, prevY);
    vgl::Vec3f curr = unproject(currX, currY);

    //float sphereRadius = std::tan(_aperture / 2.0f) * length(_target - _pos);
    float sphereRadius = 1;

    vgl::Ray3f prevRay(_pos, prev - _pos);
    vgl::Ray3f currRay(_pos, curr - _pos);

    vgl::Vec3f prevHit, currHit;
    if ( intersectRaySphere(prevRay, _target, sphereRadius, prevHit) &&
         intersectRaySphere(currRay, _target, sphereRadius, currHit) )
    {
      float c = dot(norm(currHit), norm(prevHit));
      // Need to use the inverse rotation, since we're rotating the camera
      // rather than the object.
      vgl::Quaternionf q = inverse(vgl::rotation(
          cross(prevHit - _target, currHit - _target), std::acos(c)));
      _pos = rotate(q, _pos - _target) + _target;
    }
  }
};


class ArcballViewer : public vgl::Viewer
{
public:
  ArcballViewer(int width, int height,
                vgl::Camera* camera, vgl::Renderer* renderer) :
    vgl::Viewer("Arcball Test", width, height, camera, renderer)
  {}
  
  virtual void actionHandler(int action) {
    if (action != ACTION_ROLL_CAMERA) {
      vgl::Viewer::actionHandler(action);
      return;
    }
    ArcballCamera* cam = dynamic_cast<ArcballCamera*>(_camera);
    if (cam == NULL)
      return;

    cam->arcballRoll(_prevMouseX, (_height - 1) - _prevMouseY, _mouseX, (_height - 1) - _mouseY);
    glutPostRedisplay();
  }
};


int main(int argc, char** argv)
{
  const int kWidth = 800;
  const int kHeight = 600;

  ArcballCamera camera(
      vgl::Vec3f(0, 0, 5), vgl::Vec3f(0, 0, 0), vgl::Vec3f(0, 1, 0),
      -1, 1, -1, 1, 30, kWidth, kHeight);
  ArcballRenderer renderer;
  ArcballViewer viewer(kWidth, kHeight, &camera, &renderer);
  viewer.run();
  return 0;
}

