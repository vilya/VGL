#ifndef vgl_parser_h
#define vgl_parser_h

#include <stdexcept>
#include <string>

#include "vgl_math.h"

namespace vgl {

//
// Forward declarations
//

class Face;
class Material;
class RawImage;
class ResourceManager;


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
  virtual void beginModel(const char* path) = 0;
  virtual void endModel() = 0;

  virtual void coordParsed(const Float3& coord) = 0;
  virtual void texCoordParsed(const Float3& coord) = 0;
  virtual void normalParsed(const Float3& normal) = 0;
  virtual void colorParsed(const Float3& color) = 0;
  virtual void faceParsed(Face* face) = 0;
  virtual void materialParsed(const std::string& name, Material* material) = 0;
  virtual void textureParsed(RawImage* texture) = 0;
};


//
// Functions
//

void loadModel(ParserCallbacks* callbacks, const char* path, ResourceManager* resources)
  throw(ParseException);


} // namespace vgl

#endif // vgl_parser_h

