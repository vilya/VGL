#ifndef OBJViewer_plyparser_h
#define OBJViewer_plyparser_h

#include "parser.h"


namespace vgl {

//
// Functions
//

void loadPLY(ParserCallbacks* callbacks, const char* path)
  throw(ParseException);


} // namespace vgl

#endif // OBJViewer_plyparser_h

