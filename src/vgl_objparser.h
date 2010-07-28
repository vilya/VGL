#ifndef vgl_objparser_h
#define vgl_objparser_h

#include "vgl_parser.h"


namespace vgl {

//
// Forward declarations
//

class ResourceManager;


//
// Functions
//

void loadOBJ(ParserCallbacks* callbacks, const char* path, ResourceManager* resources)
  throw(ParseException);


} // namespace vgl

#endif // vgl_objparser_h

