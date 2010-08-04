#include "vgl.h"

class ExampleRenderer : public vgl::Renderer
{
public:
  ExampleRenderer() {}

  virtual void setup() {
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    /*
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xFF00);
    */

  }

  virtual void render() {
    glPushMatrix();
    glTranslatef(-5, 0, -1);
    glScalef(10, 10, 10);
    quad(GL_FILL, vgl::Vec4f(0.6, 0, 0, 1));
    glPopMatrix();

    quad(GL_FILL, vgl::Vec4f(0.4, 0.4, 0.4, 0.5));
    quad(GL_LINE, vgl::Vec4f(0.8, 0.8, 1, 1));
  }

private:
  void quad(GLenum mode, vgl::Vec4f color) {
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    if (mode == GL_LINE) {
      glEnable(GL_POLYGON_OFFSET_LINE);
      glPolygonOffset(1, -5);
    } else {
      glDisable(GL_POLYGON_OFFSET_LINE);
    }
    glColor4fv(color.data);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5, -0.5, 0);
    glVertex3f( 0.5, -0.5, 0);
    glVertex3f( 0.5,  0.5, 0);
    glVertex3f(-0.5,  0.5, 0);
    glEnd();
  }
};


int main(int argc, char** argv)
{
  vgl::Camera* camera = new vgl::Camera(
      vgl::Vec3f(0, 0, 5), vgl::Vec3f(0, 0, 0), vgl::Vec3f(0, 1, 0),
      -1, 1, -1, 1, 30, 800, 600);
  ExampleRenderer* renderer = new ExampleRenderer();
  vgl::Viewer viewer("Example Viewer", 800, 600, camera, renderer);
  viewer.run();
}

