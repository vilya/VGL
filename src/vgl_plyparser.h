#ifndef OBJViewer_plyparser_h
#define OBJViewer_plyparser_h

#include "vgl_parser.h"


namespace vgl {

//
// Forward declarations
//

class ResourceManager;


//
// Functions
//

void loadPLY(ParserCallbacks* callbacks, const char* path, ResourceManager* resources)
  throw(ParseException);


} // namespace vgl

#endif // OBJViewer_plyparser_h

