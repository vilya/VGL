#include "vgl.h"

#include <cstdio>


//
// CLASSES
//

class ImageViewRenderer : public vgl::Renderer
{
public:
  ImageViewRenderer(vgl::RawImage* img);

  virtual void setup();
  virtual void render();

private:
  vgl::RawImage* _img;
};


//
// ImageViewRenderer METHODS
//

ImageViewRenderer::ImageViewRenderer(vgl::RawImage* img) :
  _img(img)
{
}


void ImageViewRenderer::setup()
{
  // This leaves the texture bound after uploading it, so we don't need to
  // re-bind it later.
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
  ImageViewRenderer renderer(img);
  vgl::OrthoCamera camera(img->getWidth(), img->getHeight());
  vgl::Viewer viewer("VGL Image Viewer",
      img->getWidth(), img->getHeight(), &renderer, &camera);
  viewer.run();
}

