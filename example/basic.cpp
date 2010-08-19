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
  ExampleRenderer renderer;
  vgl::Viewer viewer("Example Viewer", 800, 600, &renderer);
  viewer.run();
}

