#ifndef vgl_parser_h
#define vgl_parser_h

#include <stdexcept>
#include <string>

#include "vgl/matrix3.h"
#include "vgl/matrix4.h"
#include "vgl/public.h"
#include "vgl/vec2.h"
#include "vgl/vec3.h"
#include "vgl/vec4.h"

namespace vgl {

  //
  // Exceptions
  //

  class PUBLIC ParseException : public virtual std::exception {
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
  class PUBLIC ParserCallbacks {
  public:
    enum {
      // Common material attribute names.
      kAmbientColor,
      kDiffuseColor,
      kSpecularColor,
      kTransmissivity,
      kDissolve,
      kSpecularExponent,
      kNormal,
      kBumpMap,

      // Common vertex attribute names.
      kCoordRef,
      kTexCoordRef,
      kNormalRef,

      // Common model attribute names.
      kMaterialName,
      kCoord,
      kTexCoord,
      kVertexNormal,
      kIntensity
    };

  public:
    virtual void beginModel(const char* path);
    virtual void endModel();

    virtual void beginFace();
    virtual void endFace();

    virtual void beginVertex();
    virtual void endVertex();

    virtual void beginMaterial(const char* name);
    virtual void endMaterial();

    virtual void indexAttributeParsed(int attr, size_t value);
    virtual void floatAttributeParsed(int attr, float value);
    virtual void matrix3fAttributeParsed(int attr, const Matrix3f& value);
    virtual void matrix4fAttributeParsed(int attr, const Matrix4f& value);
    virtual void vec2fAttributeParsed(int attr, const Vec2f& value);
    virtual void vec3fAttributeParsed(int attr, const Vec3f& value);
    virtual void vec4fAttributeParsed(int attr, const Vec4f& value);
    virtual void textureAttributeParsed(int attr, const char* path);
    virtual void stringAttributeParsed(int attr, const char* value);
  };


  //
  // Functions
  //

  void PUBLIC loadModel(ParserCallbacks* callbacks, const char* path)
    throw(ParseException);

} // namespace vgl

#endif // vgl_parser_h

