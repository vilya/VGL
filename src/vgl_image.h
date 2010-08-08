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

  int getType() const;
  unsigned int getBytesPerPixel() const;
  unsigned int getWidth() const;
  unsigned int getHeight() const;
  unsigned char* getPixels();

  unsigned int getTexID() const;
  void uploadTexture(unsigned int texID = 0);
  void uploadTextureAs(int targetType, unsigned int texID = 0);

  void downsampleInPlace(unsigned int downsampleX, unsigned int downsampleY);

  //! Like getPixels, but this transfers ownership of the pixel memory to the
  //! caller.
  unsigned char* takePixels();

  //! Call this if you want to free up the memory used to store the pixels,
  //! (e.g. once you've uploaded them to GPU memory) without deleting the
  //! entire object.
  void deletePixels();

private:
  void loadBMP(FILE* file) throw(ImageException);
  void loadTGA(FILE* file) throw(ImageException);
  void loadPPM(FILE* file) throw(ImageException);
  void loadJPG(FILE* file) throw(ImageException);
  void loadPNG(FILE* file) throw(ImageException);
  void loadTIFF(const char* filename) throw(ImageException);

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

