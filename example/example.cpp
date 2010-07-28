#include "vgl.h"


class ExampleRenderer : public vgl::Renderer
{
public:
  virtual void render() {
    glutSolidCone(0.5f, 1.0f, 10, 10);
  }
};


int main(int argc, char** argv)
{
  vgl::Viewer viewer("Example Viewer", 800, 600,
      new vgl::Camera(vgl::Float3(0, 0, 10), vgl::Float3(0, 0, 0), vgl::Float3(0, 1, 0),
                      -1, 1, -1, 1, 30, 800, 600),
      //new ExampleRenderer());
      NULL);
  viewer.run();
}

