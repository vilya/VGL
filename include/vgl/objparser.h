#ifndef vgl_objparser_h
#define vgl_objparser_h

#include "vgl/parser.h"
#include "vgl/public.h"

namespace vgl {

  //
  // Functions
  //

  PUBLIC
  void loadOBJ(ParserCallbacks* callbacks, const char* path)
    throw(ParseException);

} // namespace vgl

#endif // vgl_objparser_h

