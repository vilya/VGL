#include "vgl.h"


//
// Classes
//

class StencilRenderer : public vgl::Renderer
{
public:
  virtual void setup();
  virtual void render();
};


//
// StencilRenderer methods
//

void StencilRenderer::setup()
{
  int viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  int width = viewport[0];
  int height = viewport[1];

  const unsigned char pixels[64] = {
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
  };

  width = 1024;
  height = 768;
  gluOrtho2D(0, width, 0, height);

  // Fill the stencil buffer.
  glClearStencil(0);
  glClear(GL_STENCIL_BUFFER_BIT);
  glColorMask(0, 0, 0, 0);
  glEnable(GL_STENCIL_TEST);
  glDisable(GL_DEPTH_TEST);
  glStencilFunc(GL_EQUAL, 1, 1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  for (int y = 0; y < height; y += 8) {
    for (int x = 0; x < width; x += 8) {
      glRasterPos2i(x, y);
      glDrawPixels(8, 8, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, pixels);
    }
  }
  vgl::checkGLError("fucked up the stencil buffer initialisation");

  glColorMask(1, 1, 1, 1);
  glEnable(GL_DEPTH_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}


void StencilRenderer::render()
{
  // Draw the left eye image.
  glStencilFunc(GL_EQUAL, 1, 1);
  glColor3f(0, 0, 1);
  glutSolidTeapot(1.0);

  // Draw the right eye image.
  glStencilFunc(GL_NOTEQUAL, 1, 1);
  glColor3f(0, 1, 1);
  glutSolidTeapot(1.0);

//  glDisable(GL_STENCIL_TEST);

//  int viewport[4];
//  glGetIntegerv(GL_VIEWPORT, viewport);
//  int width = 1024;//viewport[0];
//  int height = 768;//viewport[1];

//  const unsigned char pixels[64] = {
//    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
//    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
//    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
//    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
//    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
//    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
//    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
//    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
//  };
//  gluOrtho2D(0, width, 0, height);
////  glRasterPos2i(0, 0);
////  glDrawPixels(8, 8, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
////  glRasterPos2f(0.5, 0.5);
////  glDrawPixels(8, 8, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
//  for (int y = 0; y < height; y += 8) {
//    for (int x = 0; x < width; x += 8) {
//      glRasterPos2f(x, y);
//      glDrawPixels(8, 8, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
//    }
//  }
//  vgl::checkGLError("fucked up the stencil buffer initialisation");
}


//
// Functions
//

int main(int argc, char** argv)
{
  StencilRenderer renderer;
  vgl::Viewer viewer("Stencil buffer test", 1024, 768, &renderer);
  viewer.run();
  return 0;
}
