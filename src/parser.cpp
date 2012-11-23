#include "vgl/parser.h"

#include "vgl/objparser.h"
#include "vgl/plyparser.h"

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



void ParserCallbacks::indexAttributeParsed(int attr, size_t value)
{
}


void ParserCallbacks::floatAttributeParsed(int attr, float value)
{
}


void ParserCallbacks::matrix3fAttributeParsed(int attr, const Matrix3f& value)
{
}


void ParserCallbacks::matrix4fAttributeParsed(int attr, const Matrix4f& value)
{
}


void ParserCallbacks::vec2fAttributeParsed(int attr, const Vec2f& value)
{
}


void ParserCallbacks::vec3fAttributeParsed(int attr, const Vec3f& value)
{
}


void ParserCallbacks::vec4fAttributeParsed(int attr, const Vec4f& value)
{
}


void ParserCallbacks::textureAttributeParsed(int attr, const char* path)
{
}


void ParserCallbacks::stringAttributeParsed(int attr, const char* value)
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

