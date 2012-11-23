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


int main(int argc, char** argv)
{
  const int kWidth = 800;
  const int kHeight = 600;

  ArcballRenderer renderer;
  vgl::ArcballCamera camera(
      Eigen::Vector3f(0, 0, 5), Eigen::Vector3f(0, 0, 0), Eigen::Vector3f(0, 1, 0),
      -1, 1, -1, 1, 30, kWidth, kHeight);
  vgl::Viewer viewer("Arcball Test", kWidth, kHeight, &renderer, &camera);
  viewer.run();
  return 0;
}

