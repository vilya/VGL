#include "vgl.h"

#include <cstdio>


//
// CLASSES
//

class OrthoCamera : public vgl::Camera
{
public:
  OrthoCamera(unsigned int pixelWidth, unsigned int pixelHeight);

  virtual void setupProjectionMatrix();
  virtual void setupModelViewMatrix();
};


class ImageViewRenderer : public vgl::Renderer
{
public:
  ImageViewRenderer(vgl::RawImage* img);

  virtual void setup();
  virtual void render();

private:
  vgl::RawImage* _img;
  GLuint _texID;
};


//
// OrthoCamera METHODS
//

OrthoCamera::OrthoCamera(unsigned int pixelWidth, unsigned int pixelHeight) :
  vgl::Camera(pixelWidth, pixelHeight)
{
}


void OrthoCamera::setupProjectionMatrix()
{
  int viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glOrtho(viewport[0], viewport[2], viewport[1], viewport[3], -0.5, 0.5);
}


void OrthoCamera::setupModelViewMatrix()
{
}


//
// ImageViewRenderer METHODS
//

ImageViewRenderer::ImageViewRenderer(vgl::RawImage* img) :
  _img(img),
  _texID(0)
{
}


void ImageViewRenderer::setup()
{
  _img->uploadTexture();
}


void ImageViewRenderer::render()
{
  glBegin(GL_QUADS);
    
  glTexCoord2f(0, 0);
  glVertex2f(0, 0);

  glTexCoord2f(1, 0);
  glVertex2f(_img->getWidth(), 0);

  glTexCoord2f(1, 1);
  glVertex2f(_img->getWidth(), _img->getHeight());

  glTexCoord2f(0, 1);
  glVertex2f(0, _img->getHeight());
  
  glEnd();
}


//
// FUNCTIONS
//

void usage(const char* progname)
{
  fprintf(stderr, "Usage: %s <imagefile>\n", progname);
}


int main(int argc, char** argv)
{
  if (argc != 2) {
    usage(argv[0]);
    return -1;
  }

  vgl::RawImage* img = new vgl::RawImage(argv[1]);
  OrthoCamera* camera = new OrthoCamera(img->getWidth(), img->getHeight());
  ImageViewRenderer* renderer = new ImageViewRenderer(img);
  vgl::Viewer viewer("VGL Image Viewer",
      img->getWidth(), img->getHeight(), camera, renderer);
  viewer.run();
}

