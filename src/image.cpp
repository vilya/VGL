#include "vgl/image.h"

#include <libgen.h>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <jpeglib.h>  // Required for jpeg support.
#include <png.h>      // Required for png support.
#include <tiffio.h>   // Required for tiff support.

#ifdef linux
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif


namespace vgl {

//
// ImageException METHODS
//

ImageException::ImageException(const char*msg_format...) :
    std::exception()
{
  va_list args;
  va_start(args, msg_format);
  vsnprintf(message, 4096, msg_format, args);
  va_end(args);
}


const char* ImageException::what() const throw()
{
  return message;
}


//
// Image METHODS
//

RawImage::RawImage(const char *path) throw(ImageException) :
  _type(GL_RGB),
  _texId(0),
  _bytesPerPixel(0),
  _width(0),
  _height(0),
  _pixels(NULL)
{
  const char *filename = basename(const_cast<char *>(path));
  if (filename == NULL)
    throw ImageException("Invalid image filename: %s does not name a file.", filename);

  const char *ext = strrchr(filename, '.');
  if (ext == NULL)
    throw ImageException("Unknown image format.");

  FILE *file = fopen(path, "rb");
  if (file == NULL)
    throw ImageException("File not found: %s.", filename);

  try {
    if (strcasecmp(ext, ".bmp") == 0) {
      loadBMP(file);
    } else if (strcasecmp(ext, ".tga") == 0) {
      loadTGA(file);
    } else if (strcasecmp(ext, ".ppm") == 0) {
      loadPPM(file);
    } else if (strcasecmp(ext, ".jpg") == 0 || strcasecmp(ext, ".jpeg") == 0) {
      loadJPG(file);
    } else if (strcasecmp(ext, ".png") == 0) {
      loadPNG(file);
    } else if (strcasecmp(ext, ".tif") == 0 || strcasecmp(ext, ".tiff") == 0) {
      loadTIFF(path);
    } else {
      throw ImageException("Unknown image format: %s", ext);
    }
    fclose(file);
  } catch (ImageException& ex) {
    fclose(file);
    if (_pixels != NULL)
      delete _pixels;
    throw ex;
  }
}


RawImage::RawImage(int type, int bytesPerPixel, int width, int height) :
  _type(type),
  _texId(0),
  _bytesPerPixel(bytesPerPixel),
  _width(width),
  _height(height),
  _pixels(NULL)
{
  unsigned int size = _bytesPerPixel * _width * _height;
  _pixels = new unsigned char[size];
  for (unsigned int i = 0; i < size; ++i)
    _pixels[i] = 0xFF;
}


RawImage::RawImage(const RawImage& img) :
  _type(img._type),
  _texId(0),
  _bytesPerPixel(img._bytesPerPixel),
  _width(img._width),
  _height(img._height),
  _pixels(NULL)
{
  unsigned int size = _bytesPerPixel * _width * _height;
  _pixels = new unsigned char[size];
  memcpy(_pixels, img._pixels, size);
}


RawImage::~RawImage()
{
  delete[] _pixels;
}


int RawImage::getType() const
{
  return _type;
}


unsigned int RawImage::getBytesPerPixel() const
{
  return _bytesPerPixel;
}


unsigned int RawImage::getWidth() const
{
  return _width;
}


unsigned int RawImage::getHeight() const
{
  return _height;
}


unsigned char* RawImage::getPixels()
{
  return _pixels;
}


unsigned int RawImage::getTexID() const
{
  return _texId;
}


void RawImage::uploadTexture(unsigned int texId)
{
  GLenum targetType;
  switch (_type) {
    case GL_BGR:
      targetType = GL_RGB;
      break;
    case GL_BGRA:
      targetType = GL_RGBA;
      break;
    default:
      targetType = _type;
      break;
  }
  uploadTextureAs(targetType, texId);
}


void RawImage::uploadTextureAs(int targetType, unsigned int texId)
{
  if (texId == 0)
    glGenTextures(1, &_texId);
  else
    _texId = texId;

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, _texId);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexImage2D(GL_TEXTURE_2D, 0, targetType,
      _width, _height, 0, _type, GL_UNSIGNED_BYTE, _pixels);
}


void RawImage::downsampleInPlace(unsigned int downsampleX, unsigned int downsampleY)
{
  RawImage* tmp = downsample(this, downsampleX, downsampleY);

  deletePixels();
  _type = tmp->getType();
  _bytesPerPixel = tmp->getBytesPerPixel();
  _width = tmp->getWidth();
  _height = tmp->getHeight();
  _pixels = tmp->takePixels();

  delete tmp;
}


unsigned char* RawImage::takePixels()
{
  unsigned char* pixels = _pixels;
  _pixels = NULL;
  return pixels;
}


void RawImage::deletePixels()
{
  delete _pixels;
  _pixels = NULL;
}


void RawImage::loadBMP(FILE *file) throw(ImageException)
{
  // Read the header data.
  unsigned char file_header[14];
  unsigned char info_header[40];
  if (fread(file_header, sizeof(unsigned char), 14, file) < 14)
    throw ImageException("Invalid or missing texture data.");
  if (fread(info_header, sizeof(unsigned char), 40, file) < 40)
    throw ImageException("Invalid or missing texture data.");

  // TODO: inspect the header data and return suitable errors for unsupported formats.
  _type = GL_BGR;
  _bytesPerPixel = 3;
  _width = (unsigned int)info_header[4] |
           (unsigned int)info_header[5] << 8 |
           (unsigned int)info_header[6] << 16 |
           (unsigned int)info_header[7] << 24;
  _height = (unsigned int)info_header[8] |
            (unsigned int)info_header[9] << 8 |
            (unsigned int)info_header[10] << 16 |
            (unsigned int)info_header[11] << 24;

  // Read the texture data.
  unsigned int numBytes = _width * _height * _bytesPerPixel;
  _pixels = new unsigned char[numBytes];
  if (fread(_pixels, sizeof(unsigned char), numBytes, file) < numBytes)
    throw ImageException("Invalid or missing texture data.");
  // Note that the data from the file is in BGR order.
}


void RawImage::loadTGA(FILE *file) throw(ImageException)
{
  unsigned char header[18];
  fread(header, sizeof(unsigned char), 18, file);
  if (header[1] != 0) // The colormap byte.
    throw ImageException("Colormap TGA files aren't supported.");

  _width = header[0xC] + header[0xD] * 256; 
  _height = header[0xE] + header[0xF] * 256;
  unsigned int bitDepth = header[0x10];
  /* make sure we are loading a supported bit-depth */
  if (bitDepth != 32 && bitDepth != 24 && bitDepth != 8)
    throw ImageException("TGA files with a bit depth of %d aren't supported.", bitDepth);

  unsigned int numPixels = _width * _height;
  _bytesPerPixel = bitDepth / 8;
  _pixels = new unsigned char[numPixels * _bytesPerPixel];
  switch (header[2]) { // The image type byte
    case 2: // TrueColor, uncompressed
    case 3: // Monochrome, uncompressed
      tgaLoadUncompressed(file, numPixels, _bytesPerPixel, _pixels);
      break;
    case 10: // TrueColor, RLE compressed
    case 11: // Monochrome, RLE compressed
      tgaLoadRLECompressed(file, numPixels, _bytesPerPixel, _pixels);
      break;
    // Unsupported image types.
    default:
      throw ImageException("Unknown TGA image type (type code: %d).", header[2]);
  }

  if (bitDepth == 32)
    _type = GL_BGRA;
  else if (bitDepth == 24)
    _type = GL_BGR;
  else
    _type = GL_ALPHA;
}


void RawImage::tgaLoadUncompressed(FILE *file, unsigned int numPixels,
    unsigned int bytesPerPixel, unsigned char *pixels)
  throw(ImageException)
{
  unsigned int numBytes = numPixels * bytesPerPixel;
  if (fread(pixels, sizeof(unsigned char), numBytes, file) < numBytes)
    throw ImageException("Missing or invalid TGA image data.");
}


void RawImage::tgaLoadRLECompressed(FILE *file, unsigned int numPixels,
    unsigned int bytesPerPixel, unsigned char *pixels)
  throw(ImageException)
{
  const int MAX_BYTES_PER_PIXEL = 4;

  int pixelCount;
  bool isEncoded;

  unsigned int pixelsRead = 0;
  unsigned char pixel[MAX_BYTES_PER_PIXEL];
  while (pixelsRead < numPixels) {
    pixelCount = fgetc(file);
    if (pixelCount == EOF)
      throw ImageException("Missing or invalid TGA image data.");

    isEncoded = pixelCount > 127;
    pixelCount = (pixelCount & 0x7F) + 1;
    if (isEncoded) {
      if (fread(pixel, sizeof(unsigned char), bytesPerPixel, file) < bytesPerPixel)
        throw ImageException("Missing or invalid TGA image data.");
      for (int i = 0; i < pixelCount; ++i) {
        memcpy(pixels, pixel, bytesPerPixel);
        pixels += bytesPerPixel;
      }
    } else {
      unsigned int numBytes = pixelCount * bytesPerPixel;
      if (fread(pixels, sizeof(unsigned char), numBytes, file) < numBytes)
        throw ImageException("Missing or invalid TGA image data.");
      pixels += numBytes;
    }
    pixelsRead += pixelCount;
  }
}


void RawImage::loadPPM(FILE* file) throw(ImageException)
{
  int fileType = 0;
  fscanf(file, "P%d", &fileType);
  if (fileType != 3 && fileType != 6)
    throw ImageException("Unsupported PPM subformat.");

  _type = GL_RGB;
  _bytesPerPixel = 3;
  _width = ppmGetNextInt(file);
  _height = ppmGetNextInt(file);

  int maxValue = ppmGetNextInt(file);

  unsigned int numBytes = _width * _height * _bytesPerPixel;
  _pixels = new unsigned char[numBytes];
  if (fileType == 3) {
    for (int row = _height - 1; row >= 0; --row) {
      long start = row * _width * _bytesPerPixel;
      long end = start + _width * _bytesPerPixel;
      for (long i = start; i < end; ++i)
        _pixels[i] = ppmGetNextInt(file) * 255 / maxValue;
    }
  } else {
    int ch = fgetc(file);
    if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r')
      ungetc(ch, file);

    if (fread(_pixels, sizeof(unsigned char), numBytes, file) < numBytes)
      throw ImageException("Invalid or missing texture data.");
  }
}


int RawImage::ppmGetNextInt(FILE* file) throw(ImageException)
{
  int ch = fgetc(file);
  bool comment = false;
  while (ch != EOF) {
    if ((ch >= '0' && ch <= '9') && !comment) {
      int val;
      ungetc(ch, file);
      fscanf(file, "%d", &val);
      return val;
    } else if (ch == '#') {
      comment = true;
    } else if (ch == '\n') {
      comment = false;
    }
    ch = fgetc(file);
  }
  throw ImageException("Incomplete PPM file.");
}


void RawImage::loadJPG(FILE* file) throw(ImageException)
{
  jpeg_decompress_struct cinfo;
  jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  try {
    jpeg_stdio_src(&cinfo, file);

    if (jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK)
      throw ImageException("Error reading JPEG header.");

    // Set decompression parameters in cinfo here. (optional)

    if (!jpeg_start_decompress(&cinfo))
      throw ImageException("Error reading JPEG data.");

    _bytesPerPixel = sizeof(JSAMPLE) * cinfo.output_components;
    switch (cinfo.output_components) {
      case 1:
        _type = GL_ALPHA;
        break;
      case 3:
        _type = GL_RGB;
        break;
      case 4:
        _type = GL_RGBA;
        break;
    }
    _width = cinfo.output_width;
    _height = cinfo.output_height;
    _pixels = new unsigned char[_bytesPerPixel * _width * _height];
    unsigned char *p;

    while (cinfo.output_scanline < cinfo.output_height) {
      p = _pixels + (sizeof(JSAMPLE) * _bytesPerPixel * _width * (_height - cinfo.output_scanline - 1));
      jpeg_read_scanlines(&cinfo, &p, 1);
    }

    if (!jpeg_finish_decompress(&cinfo))
      throw ImageException("Error reading JPEG data.");

    jpeg_destroy_decompress(&cinfo);
  } catch (ImageException& e) {
    jpeg_destroy_decompress(&cinfo);
    throw e;
  }
}


void RawImage::loadPNG(FILE* file) throw(ImageException)
{
  png_structp pngData = NULL;
  png_infop pngInfo = NULL;
  unsigned char** rowPtrs = NULL;

  try {
    pngData = png_create_read_struct(PNG_LIBPNG_VER_STRING,
      NULL,   // error_ptr
      NULL,   // error_fn
      NULL    // warn_fn
    );
    pngInfo = png_create_info_struct(pngData);

    unsigned char header[8];
    if (fread(header, 1, 8, file) < 8)
      throw ImageException("Missing PNG header data.");
    if (png_sig_cmp(header, 0, 8) != 0)
      throw ImageException("Not a PNG file!");
    if (setjmp(png_jmpbuf(pngData)))
      throw ImageException("PNG library error.");

    png_init_io(pngData, file);
    png_set_sig_bytes(pngData, 8);
    png_read_info(pngData, pngInfo);

    png_uint_32 width, height;
    int bitDepth, colorType, iMethod, cMethod, fMethod;
    png_get_IHDR(pngData, pngInfo, &width, &height, &bitDepth, &colorType,
        &iMethod, &cMethod, &fMethod);

    int bytesPerChannel = (bitDepth / 8);
    if (bitDepth % 8 != 0)
      ++bytesPerChannel;

    _width = width;
    _height = height;
    switch (colorType) {
      case PNG_COLOR_TYPE_PALETTE:
        png_set_palette_to_rgb(pngData);
        _type = GL_RGB;
        _bytesPerPixel = bytesPerChannel * 3;
        break;
      case PNG_COLOR_TYPE_RGBA:
        _type = GL_RGBA;
        _bytesPerPixel = bytesPerChannel * 4;
        break;
      case PNG_COLOR_TYPE_RGB:
        _type = GL_RGB;
        _bytesPerPixel = bytesPerChannel * 3;
        break;
      case PNG_COLOR_TYPE_GRAY:
        if (bitDepth < 8)
          png_set_gray_1_2_4_to_8(pngData);
        _type = GL_ALPHA;
        _bytesPerPixel = bytesPerChannel;
        break;
      default:
        throw ImageException("Unknown PNG type.");
    }

    _pixels = new unsigned char[_bytesPerPixel * _width * _height];
    rowPtrs = new unsigned char*[_height];
    for (size_t i = 0; i < _height; ++i)
      rowPtrs[i] = &_pixels[_bytesPerPixel * _width * (_height - i - 1)];
    png_read_image(pngData, rowPtrs);

    png_destroy_read_struct(&pngData, &pngInfo, NULL);
    delete[] rowPtrs;
  } catch (ImageException& e) {
    png_destroy_read_struct(&pngData, &pngInfo, NULL);
    if (rowPtrs != NULL)
      delete[] rowPtrs;
    throw e;
  }
}


void RawImage::loadTIFF(const char* filename) throw(ImageException)
{
  TIFF* tiff = TIFFOpen(filename, "rb");
  if (!tiff)
    throw ImageException("Unable to open TIFF file.");

  _type = GL_RGBA;
  _bytesPerPixel = 4;
  TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &_width);
  TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &_height);

  size_t numPixels = _width * _height;
  _pixels = new unsigned char[_bytesPerPixel * numPixels];
  bool readOK = TIFFReadRGBAImage(tiff, _width, _height, (uint32*)_pixels, 0);
  TIFFClose(tiff);
  if (!readOK)
    throw ImageException("Error reading TIFF data.");
}


RawImage* downsample(RawImage* src, unsigned int downsampleX, unsigned int downsampleY)
{
  RawImage* result = new RawImage(src->getType(), src->getBytesPerPixel(),
      src->getWidth() / downsampleX, src->getHeight() / downsampleY);
  
  size_t bpp = result->getBytesPerPixel();
  size_t xStride = (downsampleX - 1) * bpp;
  for (size_t y = 0; y < result->getHeight(); ++y) {
    size_t from = y * downsampleY * src->getWidth() * bpp;
    size_t to = y * result->getWidth() * bpp;
    for (size_t x = 0; x < result->getWidth(); ++x) {
      for (size_t b = 0; b < bpp; ++b) {
        result->getPixels()[to] = src->getPixels()[from];
        ++to;
        ++from;
      }
      from += xStride;
    }
  }

  return result;
}


} // namespace vgl

