#ifndef vgl_image_h
#define vgl_image_h

#include <cstdio>
#include <stdexcept>

namespace vgl {


class ImageException : public std::exception {
public:
	char message[4096];

	ImageException(const char* msg_format...);
	~ImageException() throw() {}

  virtual const char* what() const throw();
};


class RawImage {
public:
  RawImage(const char* path) throw(ImageException);
  RawImage(int type, int bytesPerPixel, int width, int height);
  RawImage(const RawImage& img);
  ~RawImage();

  int getType() const { return _type; }
  unsigned int getBytesPerPixel() const { return _bytesPerPixel; }
  unsigned int getWidth() const { return _width; }
  unsigned int getHeight() const { return _height; }
  unsigned char* getPixels() { return _pixels; }

  unsigned int getTexID() const { return _texId; }
  void setTexID(unsigned int texId) { _texId = texId; }

  //! Call this if you want to free up the memory used to store the pixels,
  //! (e.g. once you've uploaded them to GPU memory) without deleting the
  //! entire object.
  void deletePixels() { delete _pixels; _pixels = NULL; }

private:
  void loadBMP(FILE* file) throw(ImageException);
  void loadTGA(FILE* file) throw(ImageException);
  void loadPPM(FILE* file) throw(ImageException);
  void loadJPG(FILE* file) throw(ImageException);
  void loadPNG(FILE* file) throw(ImageException);

  void tgaLoadUncompressed(FILE* file, unsigned int numPixels,
      unsigned int bytesPerPixel, unsigned char *pixels)
    throw(ImageException);

  void tgaLoadRLECompressed(FILE* file, unsigned int numPixels,
      unsigned int bytesPerPixel, unsigned char *pixels)
    throw(ImageException);

  int ppmGetNextInt(FILE* file) throw(ImageException);

private:
  int _type;
  unsigned int _texId;
  unsigned int _bytesPerPixel;
  unsigned int _width;
  unsigned int _height;
  unsigned char* _pixels;
};


RawImage* downsample(RawImage* src, unsigned int downsampleX, unsigned int downsampleY);

} // namespace vgl

#endif // vgl_image_h

