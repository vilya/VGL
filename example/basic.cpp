#include "vgl.h"

class ExampleRenderer : public vgl::Renderer
{
public:
  ExampleRenderer() {}
  virtual void setup() {}
  virtual void render() {
    glColor3f(1, 0, 0);
    glutSolidTeapot(1.0);
  }
};


int main(int argc, char** argv)
{
  vgl::Camera* camera = new vgl::Camera(
      vgl::Float3(0, 0, 5), vgl::Float3(0, 0, 0), vgl::Float3(0, 1, 0),
      -1, 1, -1, 1, 30, 800, 600);
  ExampleRenderer* renderer = new ExampleRenderer();
  vgl::Viewer viewer("Example Viewer", 800, 600, camera, renderer);
  viewer.run();
}

