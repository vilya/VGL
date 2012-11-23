#ifndef vgl_objparser_h
#define vgl_objparser_h

#include "parser.h"


namespace vgl {

//
// Functions
//

void loadOBJ(ParserCallbacks* callbacks, const char* path)
  throw(ParseException);


} // namespace vgl

#endif // vgl_objparser_h

