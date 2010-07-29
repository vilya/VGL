#ifndef vgl_parser_h
#define vgl_parser_h

#include <stdexcept>
#include <string>

#include "vgl_math.h"

namespace vgl {


//
// Exceptions
//

class ParseException : public virtual std::exception {
public:
  char message[4096];

  ParseException(const char *msg_format...);
  virtual ~ParseException() throw() {}

  virtual const char* what() const throw();
};


//
// Interfaces
//

// Implement this and override the relevant methods to provide your own custom
// parsing behaviour.
class ParserCallbacks {
public:
  // Common material attribute names.
  static const char* kAmbientColor;
  static const char* kDiffuseColor;
  static const char* kSpecularColor;
  static const char* kTransmissivity;
  static const char* kDissolve;
  static const char* kSpecularIndex;
  static const char* kNormal;
  static const char* kBumpMap;

  // Common vertex attribute names.
  static const char* kCoordRef;
  static const char* kTexCoordRef;
  static const char* kNormalRef;
  
  // Common model attribute names.
  static const char* kMaterialName;
  static const char* kCoord;
  static const char* kTexCoord;
  static const char* kVertexNormal;
  static const char* kIntensity;

public:
  virtual void beginModel(const char* path) = 0;
  virtual void endModel() = 0;

  virtual void beginFace() = 0;
  virtual void endFace() = 0;

  virtual void beginVertex() = 0;
  virtual void endVertex() = 0;

  virtual void beginMaterial(const char* name) = 0;
  virtual void endMaterial() = 0;

  virtual void indexAttributeParsed(const char* attr, size_t value) = 0;
  virtual void floatAttributeParsed(const char* attr, float value) = 0;
  virtual void float3AttributeParsed(const char* attr, const Float3& value) = 0;
  virtual void textureAttributeParsed(const char* attr, const char* path) = 0;
  virtual void stringAttributeParsed(const char* attr, const char* value) = 0;
  // TODO: add other attribute types: matrix, string, quaternion, etc.
};


//
// Functions
//

void loadModel(ParserCallbacks* callbacks, const char* path)
  throw(ParseException);


} // namespace vgl

#endif // vgl_parser_h

