#include "vgl.h"

class ExampleRenderer : public vgl::Renderer
{
public:
  ExampleRenderer() {}
  virtual void setup() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
  }
  virtual void render() {
    glColor3f(1, 0, 0);
    glutSolidTeapot(1.0);
  }
};


int main(int argc, char** argv)
{
  vgl::Camera* camera = new vgl::BaseCamera(
      vgl::Vec3f(0, 0, 5), vgl::Vec3f(0, 0, 0), vgl::Vec3f(0, 1, 0),
      -1, 1, -1, 1, 30, 800, 600);
  ExampleRenderer* renderer = new ExampleRenderer();
  vgl::Viewer viewer("Example Viewer", 800, 600, camera, renderer);
  viewer.run();
}

