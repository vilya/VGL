#ifndef vgl_plyparser_h
#define vgl_plyparser_h

#include "vgl/parser.h"
#include "vgl/public.h"

namespace vgl {

  //
  // Functions
  //

  PUBLIC
  void loadPLY(ParserCallbacks* callbacks, const char* path)
    throw(ParseException);

} // namespace vgl

#endif // vgl_plyparser_h

