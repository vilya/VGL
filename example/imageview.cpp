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
  // Load the image as a texture.
  glGenTextures(1, &_texID);

  GLenum targetType;
  switch (_img->getType()) {
    case GL_BGR:
      targetType = GL_RGB;
      break;
    case GL_BGRA:
      targetType = GL_RGBA;
      break;
    default:
      targetType = _img->getType();
      break;
  }

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, _texID);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexImage2D(GL_TEXTURE_2D, 0, targetType,
      _img->getWidth(), _img->getHeight(),
      0, _img->getType(), GL_UNSIGNED_BYTE, _img->getPixels());
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

