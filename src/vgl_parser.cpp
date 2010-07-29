#include "vgl_parser.h"

#include "vgl_objparser.h"
#include "vgl_plyparser.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <libgen.h>

namespace vgl {

//
// EXCEPTION METHODS
//

ParseException::ParseException(const char *msg_format...) :
  std::exception()
{
  va_list args;
  va_start(args, msg_format);
  vsnprintf(message, sizeof(message), msg_format, args);
  va_end(args);
}


const char* ParseException::what() const throw()
{
  return message;
}


//
// ParserCallbacks
//

const char* ParserCallbacks::kAmbientColor = "Ka";
const char* ParserCallbacks::kDiffuseColor = "Kd";
const char* ParserCallbacks::kSpecularColor = "Ks";
const char* ParserCallbacks::kTransmissivity = "Tf";
const char* ParserCallbacks::kDissolve = "D";
const char* ParserCallbacks::kSpecularIndex = "Ns";
const char* ParserCallbacks::kNormal = "N";
const char* ParserCallbacks::kBumpMap = "Bump";

const char* ParserCallbacks::kCoordRef = "vi";
const char* ParserCallbacks::kTexCoordRef = "vti";
const char* ParserCallbacks::kNormalRef = "vni";

const char* ParserCallbacks::kMaterialName = "usemtl";
const char* ParserCallbacks::kCoord = "v";
const char* ParserCallbacks::kTexCoord = "vt";
const char* ParserCallbacks::kVertexNormal = "vn";
const char* ParserCallbacks::kIntensity = "I";


void ParserCallbacks::beginModel(const char* path)
{
}


void ParserCallbacks::endModel()
{
}



void ParserCallbacks::beginFace()
{
}


void ParserCallbacks::endFace()
{
}



void ParserCallbacks::beginVertex()
{
}


void ParserCallbacks::endVertex()
{
}



void ParserCallbacks::beginMaterial(const char* name)
{
}


void ParserCallbacks::endMaterial()
{
}



void ParserCallbacks::indexAttributeParsed(const char* attr, size_t value)
{
}


void ParserCallbacks::floatAttributeParsed(const char* attr, float value)
{
}


void ParserCallbacks::float3AttributeParsed(const char* attr, const Float3& value)
{
}


void ParserCallbacks::textureAttributeParsed(const char* attr, const char* path)
{
}


void ParserCallbacks::stringAttributeParsed(const char* attr, const char* value)
{
}


//
// PUBLIC FUNCTIONS
//

void loadModel(ParserCallbacks* callbacks, const char* path)
  throw(ParseException)
{
  if (callbacks == NULL)
    throw ParseException("You didn't provide any callbacks; parsing will do nothing!");

  const char *filename = basename(const_cast<char *>(path));
  if (filename == NULL)
    throw ParseException("Invalid model filename: %s does not name a file.", filename);

  // Check that the file exists.
  FILE *file = fopen(path, "rb");
  if (file == NULL)
    throw ParseException("File not found: %s.", path);
  else
    fclose(file);

  const char *ext = strrchr(filename, '.');
  if (ext == NULL)
    throw ParseException("Unknown model format.");

  if (strcasecmp(ext, ".obj") == 0)
    loadOBJ(callbacks, path);
  else if (strcasecmp(ext, ".ply") == 0)
    loadPLY(callbacks, path);
  else
    throw ParseException("Unknown model format: %s", ext);
}


} // namespace vgl

