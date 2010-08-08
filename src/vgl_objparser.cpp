#include "vgl_objparser.h"

#include "vgl_image.h"
#include "vgl_parser.h"
#include "vgl_utils.h"
#include "vgl_vec3.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <libgen.h>
#include <map>


namespace vgl {

//
// CONSTANTS
//

const unsigned int _MAX_LINE_LEN = 4096;


enum MTLFileLineType {
  MTL_LINETYPE_UNKNOWN,
  MTL_LINETYPE_BLANK,
  MTL_LINETYPE_COMMENT,
  MTL_LINETYPE_NEWMTL,
  MTL_LINETYPE_KA,
  MTL_LINETYPE_KD,
  MTL_LINETYPE_KE,
  MTL_LINETYPE_KM,
  MTL_LINETYPE_KS,
  MTL_LINETYPE_TF,
  MTL_LINETYPE_TR,
  MTL_LINETYPE_D,
  MTL_LINETYPE_NS,
  MTL_LINETYPE_NI,
  MTL_LINETYPE_ILLUM,
  MTL_LINETYPE_MAP_KA,
  MTL_LINETYPE_MAP_KD,
  MTL_LINETYPE_MAP_KE,
  MTL_LINETYPE_MAP_KM,
  MTL_LINETYPE_MAP_KS,
  MTL_LINETYPE_MAP_D,
  MTL_LINETYPE_MAP_BUMP,
  MTL_LINETYPE_BUMP 
};


enum OBJFileLineType {
  OBJ_LINETYPE_UNKNOWN,
  OBJ_LINETYPE_BLANK,
  OBJ_LINETYPE_COMMENT,
  OBJ_LINETYPE_V,
  OBJ_LINETYPE_VT,
  OBJ_LINETYPE_VP,
  OBJ_LINETYPE_VN,
  OBJ_LINETYPE_F,
  OBJ_LINETYPE_FO,
  OBJ_LINETYPE_G,
  OBJ_LINETYPE_S,
  OBJ_LINETYPE_USEMTL,
  OBJ_LINETYPE_MTLLIB,
  OBJ_LINETYPE_O
};


//
// FUNCTIONS
//

bool isSpace(char ch) {
  return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}


bool isDigit(char ch) {
  return (ch >= '0' && ch <= '9');
}


bool isLetter(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}


bool isEnd(char ch) {
  return (ch == '\0');
}


bool isCommentStart(char ch) {
  return (ch == '#');
}


void eatSpace(char*& col, bool required=false) throw(ParseException) {
  if (required && !isSpace(*col))
    throw ParseException("Expected whitespace but got %s", col);
  while (isSpace(*col))
    ++col;
}


void eatChar(char ch, char*& col) throw(ParseException) {
  if (*col != ch)
    throw ParseException("Expected %c but got %c", ch, *col);
  ++col;
}


//
// GENERIC PARSING
//

float parseFloat(char *line, char*& col) throw(ParseException) {
  col = line;
  if (*col == '-' || *col == '+')
    ++col;
  while (isDigit(*col))
    ++col;
  if (*col == '.') {
    ++col;
    while (isDigit(*col))
      ++col;
  }
  if (*col == 'e' || *col == 'E') {
    ++col;
    if (*col == '+' || *col == '-')
      ++col;
    while (isDigit(*col))
      ++col;
  }
  if (col > line) {
    float val;
    if (sscanf(line, "%f", &val) == 1)
      return val;
  }
  throw ParseException("Expected a float value but got %s", line);
}


int parseInt(char* line, char*& col) throw(ParseException) {
  col = line;
  if (*col == '-' || *col == '+')
    ++col;
  while (isDigit(*col))
    ++col;

  if (col > line) {
    int val;
    if (sscanf(line, "%d", &val) == 1)
      return val;
  }
  throw ParseException("Expected an int value but got \"%s\"", line);
}


std::string parseIdentifier(char* line, char*& col) throw(ParseException) {
  col = line;
  while (*col== '.' || *col == '_' || *col == '-' || *col == '(' || *col == ')' ||
      isLetter(*col) || isDigit(*col))
    ++col;

  if (col > line) {
    char buf[_MAX_LINE_LEN];
    snprintf(buf, (int)(col - line) + 1, "%s", line);
    return std::string(buf);
  }
  throw ParseException("Expected an identifier but got \"%s\"", line);
}


std::string parseFilename(char* line, char*& col) throw(ParseException) {
  col = line;
  char quote = '\0';
  while (!isEnd(*col) && !isCommentStart(*col)) {
    if (*col == '\\') {
      ++col;
    } else if (*col == '"' || *col == '\'') {
      if (!quote)
        quote = *col;
      else if (quote == *col)
        quote = '\0';
    }
    ++col;
  }

  // Trim off trailing whitespace.
  while (col > line && isSpace(*(col - 1)))
    --col;

  if (quote)
    throw ParseException("Unclosed filename string: missing closing %c character", quote);

  char buf[_MAX_LINE_LEN];
  snprintf(buf, (int)(col - line) + 1, "%s", line);
  return std::string(buf);
}


//
// MTL FILE PARSING
//

MTLFileLineType mtlParseLineType(char* line, char*& col) throw(ParseException) {
  static const struct {
    const char* token;
    MTLFileLineType lineType;
  } LINE_TYPES[] = {
    { "newmtl", MTL_LINETYPE_NEWMTL },
    { "Ka", MTL_LINETYPE_KA },
    { "Kd", MTL_LINETYPE_KD },
    { "Ke", MTL_LINETYPE_KE },
    { "Km", MTL_LINETYPE_KM },
    { "Ks", MTL_LINETYPE_KS },
    { "Tf", MTL_LINETYPE_TF },
    { "Tr", MTL_LINETYPE_TR },
    { "d", MTL_LINETYPE_D },
    { "Ns", MTL_LINETYPE_NS },
    { "Ni", MTL_LINETYPE_NI },
    { "illum", MTL_LINETYPE_ILLUM },
    { "map_Ka", MTL_LINETYPE_MAP_KA },
    { "map_Kd", MTL_LINETYPE_MAP_KD },
    { "map_Ke", MTL_LINETYPE_MAP_KE },
    { "map_Km", MTL_LINETYPE_MAP_KM },
    { "map_Ks", MTL_LINETYPE_MAP_KS },
    { "map_D", MTL_LINETYPE_MAP_D },
    { "map_Bump", MTL_LINETYPE_MAP_BUMP },
    { "bump", MTL_LINETYPE_MAP_BUMP },
    { "#", MTL_LINETYPE_COMMENT },
    { NULL, MTL_LINETYPE_UNKNOWN }
  };

  col = line;
  while (*col == '_' || isLetter(*col) || isDigit(*col))
    ++col;

  int len = col - line;
  if (len == 0) {
    return MTL_LINETYPE_BLANK;
  } else {
    for (unsigned int i = 0; LINE_TYPES[i].token != NULL; ++i) {
      if (strncasecmp(LINE_TYPES[i].token, line, len) == 0)
        return LINE_TYPES[i].lineType;
    }
    return MTL_LINETYPE_UNKNOWN;
  }
}


std::string mtlParseNEWMTL(char* line, char*& col) throw(ParseException) {
  col = line;
  eatSpace(col, true);
  std::string name = parseIdentifier(col, col);
  return name;
}


Vec3f mtlParseColor(char* line, char*& col) throw(ParseException) {
  col = line;
  eatSpace(col, true);
  float r = parseFloat(col, col);
  eatSpace(col, false);
  if (isEnd(*col) || isCommentStart(*col))
    return Vec3f(r, r, r);
  float g = parseFloat(col, col);
  eatSpace(col, true);
  float b = parseFloat(col, col);
  return Vec3f(r, g, b);
}


float mtlParseFloat(char *line, char*& col) throw(ParseException)
{
  col = line;
  eatSpace(col, true);
  float val = parseFloat(col, col);
  return val;
}


std::string mtlParseTexture(char* line, char*& col, const char* baseDir) throw(ParseException)
{
  col = line;
  eatSpace(col, true);

  const char* resolved = resolveFilename(baseDir, parseFilename(col, col).c_str());
  std::string filename(resolved);

  fprintf(stderr, "Loading texture %s...", filename.c_str());

  return filename;
}


void loadMaterialLibrary(const char* path, ParserCallbacks* callbacks)
  throw(ParseException)
{
  fprintf(stderr, "Loading mtllib %s...\n", path);

  FILE *f = fopen(path, "r");
  if (f == NULL)
    throw ParseException("Unable to open mtl file %s: %s\n", path, strerror(errno));
  
  char line[_MAX_LINE_LEN];
  char *col;
  unsigned int line_no = 0;

  char baseDir[_MAX_LINE_LEN];
  snprintf(baseDir, _MAX_LINE_LEN, "%s", dirname(const_cast<char*>(path)));

  std::string materialName;
  std::string texPath;
  try {
    while (!feof(f)) {
      ++line_no;

      memset(line, 0, _MAX_LINE_LEN);
      if (fgets(line, _MAX_LINE_LEN, f) == NULL) {
        if (ferror(f))
          throw ParseException("Error reading from file: %s", strerror(ferror(f)));
        else
          break;
      }

      col = line;
      eatSpace(col);

      MTLFileLineType lineType = mtlParseLineType(col, col);
      if (lineType != MTL_LINETYPE_BLANK && lineType != MTL_LINETYPE_COMMENT && lineType != MTL_LINETYPE_NEWMTL) {
        if (materialName == "")
          throw ParseException("Defining a material property without declaring a material name.");
      }

      switch (lineType) {
        case MTL_LINETYPE_NEWMTL:
          if (materialName != "")
            callbacks->endMaterial();
          materialName = mtlParseNEWMTL(col, col);
          callbacks->beginMaterial(materialName.c_str());
          //if (materials.count(materialName) > 0)
          //  throw ParseException("Redefinition of material %s", materialName.c_str());
          break;
        case MTL_LINETYPE_KA:
          callbacks->vec3fAttributeParsed(ParserCallbacks::kAmbientColor, mtlParseColor(col, col));
          break;
        case MTL_LINETYPE_KD:
          callbacks->vec3fAttributeParsed(ParserCallbacks::kDiffuseColor, mtlParseColor(col, col));
          break;
        case MTL_LINETYPE_KS:
          callbacks->vec3fAttributeParsed(ParserCallbacks::kSpecularColor, mtlParseColor(col, col));
          break;
        case MTL_LINETYPE_TF:
          callbacks->vec3fAttributeParsed(ParserCallbacks::kTransmissivity, mtlParseColor(col, col));
          break;
        case MTL_LINETYPE_D:
          callbacks->floatAttributeParsed(ParserCallbacks::kDissolve, mtlParseFloat(col, col));
          break;
        case MTL_LINETYPE_NS:
          callbacks->floatAttributeParsed(ParserCallbacks::kSpecularExponent, mtlParseFloat(col, col));
          break;
        case MTL_LINETYPE_MAP_KA:
          texPath = mtlParseTexture(col, col, baseDir);
          callbacks->textureAttributeParsed(ParserCallbacks::kAmbientColor, texPath.c_str());
          break;
        case MTL_LINETYPE_MAP_KD:
          texPath = mtlParseTexture(col, col, baseDir);
          callbacks->textureAttributeParsed(ParserCallbacks::kDiffuseColor, texPath.c_str());
          break;
        case MTL_LINETYPE_MAP_KS:
          texPath = mtlParseTexture(col, col, baseDir);
          callbacks->textureAttributeParsed(ParserCallbacks::kSpecularColor, texPath.c_str());
          break;
        case MTL_LINETYPE_MAP_D:
          texPath = mtlParseTexture(col, col, baseDir);
          callbacks->textureAttributeParsed(ParserCallbacks::kDissolve, texPath.c_str());
          break;
        case MTL_LINETYPE_MAP_BUMP:
          texPath = mtlParseTexture(col, col, baseDir);
          callbacks->textureAttributeParsed(ParserCallbacks::kBumpMap, texPath.c_str());
          break;
        case MTL_LINETYPE_KE:
        case MTL_LINETYPE_KM:
        case MTL_LINETYPE_MAP_KE:
        case MTL_LINETYPE_MAP_KM:
        case MTL_LINETYPE_NI:
        case MTL_LINETYPE_ILLUM:
        case MTL_LINETYPE_TR:
        case MTL_LINETYPE_BUMP:
          // TODO: handle these.
          while (!isEnd(*col))
            ++col;
          break;
        case MTL_LINETYPE_BLANK:
        case MTL_LINETYPE_COMMENT:
          // Ignore these types of line.
          break;
        default:
          throw ParseException("Unknown line type: %s", line);
      }

      eatSpace(col);
      if (!isCommentStart(*col) && !isEnd(*col))
        throw ParseException("Unexpected trailing characters: %s", col);
    }

    if (materialName != "")
      callbacks->endMaterial();

    fclose(f);
    fprintf(stderr, "Finished parsing mtllib %s\n", path);
  } catch (ParseException& ex) {
    fclose(f);
    throw ParseException("[%s: line %d, col %d] %s\n", path, line_no, (int)(col - line), ex.message);
  }
}



//
// OBJ FILE PARSING
//

OBJFileLineType objParseLineType(char* line, char*& col) throw(ParseException) {
  static const struct {
    const char* token;
    OBJFileLineType lineType;
  } LINE_TYPES[] = {
    { "#", OBJ_LINETYPE_COMMENT },
    { "v", OBJ_LINETYPE_V },
    { "vt", OBJ_LINETYPE_VT },
    { "vp", OBJ_LINETYPE_VP },
    { "vn", OBJ_LINETYPE_VN },
    { "f", OBJ_LINETYPE_F },
    { "fo", OBJ_LINETYPE_FO },
    { "g", OBJ_LINETYPE_G },
    { "s", OBJ_LINETYPE_S },
    { "usemtl", OBJ_LINETYPE_USEMTL },
    { "mtllib", OBJ_LINETYPE_MTLLIB },
    { "o", OBJ_LINETYPE_O },
    { NULL, OBJ_LINETYPE_UNKNOWN }
  };

  col = line;
  while (isLetter(*col) || isDigit(*col))
    ++col;

  int len = col - line;
  if (len == 0) {
    return OBJ_LINETYPE_BLANK;
  } else {
    for (unsigned int i = 0; LINE_TYPES[i].token != NULL; ++i) {
      if (strncmp(LINE_TYPES[i].token, line, len) == 0)
        return LINE_TYPES[i].lineType;
    }
    return OBJ_LINETYPE_UNKNOWN;
  };
}


Vec3f objParseV(char *line, char*& col) throw(ParseException) {
  col = line;
  eatSpace(col, true);
  float x = parseFloat(col, col);
  eatSpace(col, true);
  float y = parseFloat(col, col);
  eatSpace(col, true);
  float z = parseFloat(col, col);
  eatSpace(col);
  if (isEnd(*col) || isCommentStart(*col))
    return Vec3f(x, y, z);
  float w = parseFloat(col, col);
  return Vec3f(x / w, y / w, z / w);
}


Vec3f objParseVT(char *line, char*& col) throw(ParseException) {
  col = line;
  eatSpace(col, true);
  float u = parseFloat(col, col);
  eatSpace(col);
  if (isEnd(*col) || isCommentStart(*col))
    return Vec3f(u, 0.0f, 0.0f);
  float v = parseFloat(col, col);
  eatSpace(col);
  if (isEnd(*col) || isCommentStart(*col))
    return Vec3f(u, v, 0.0f);
  float w = parseFloat(col, col);
  return Vec3f(u, v, w);
}


Vec3f objParseVP(char *line, char*& col) throw(ParseException) {
  col = line;
  eatSpace(col, true);
  float u = parseFloat(col, col);
  eatSpace(col);
  if (isEnd(*col) || isCommentStart(*col))
    return Vec3f(u, 0.0f, 0.0f);
  float v = parseFloat(col, col);
  eatSpace(col);
  if (isEnd(*col) || isCommentStart(*col))
    return Vec3f(u, v, 0.0f);
  float w = parseFloat(col, col);
  return Vec3f(u, v, w);
}


Vec3f objParseVN(char* line, char*& col) throw(ParseException) {
  col = line;
  eatSpace(col, true);
  float i = parseFloat(col, col);
  eatSpace(col, true);
  float j = parseFloat(col, col);
  eatSpace(col, true);
  float k = parseFloat(col, col);
  return Vec3f(i, j, k);
}


void objParseVertex(char *line, char*& col, ParserCallbacks* callbacks) throw(ParseException) {
  // TODO: check for -ve indexes and resolve them to +ve ones.

  col = line;

  callbacks->beginVertex();

  int v = parseInt(col, col) - 1;
  callbacks->indexAttributeParsed(ParserCallbacks::kCoordRef, (size_t)v);

  if (*col == '/') {
    eatChar('/', col);
    if (*col == '-' || isDigit(*col)) {
      int vt = parseInt(col, col) - 1;
      callbacks->indexAttributeParsed(ParserCallbacks::kTexCoordRef, (size_t)vt);
    }
    if (*col == '/') {
      eatChar('/', col);
      if (*col == '-' || isDigit(*col)) {
        int vn = parseInt(col, col) - 1;
        callbacks->indexAttributeParsed(ParserCallbacks::kNormalRef, (size_t)vn);
      }
    }
  }

  callbacks->endVertex();
}


void objParseFace(char* line, char*& col, ParserCallbacks* callbacks) throw(ParseException) {
  callbacks->beginFace();
  col = line;
  while (!isEnd(*col) && !isCommentStart(*col)) {
    eatSpace(col, true);
    if (!isEnd(*col) && !isCommentStart(*col))
      objParseVertex(col, col, callbacks);
  }
  callbacks->endFace();
}


void objParseMTLLIB(char* line, char*& col,
    ParserCallbacks* callbacks, const char* baseDir)
  throw(ParseException)
{
  col = line;
  while (!isEnd(*col) && !isCommentStart(*col)) {
    eatSpace(col, true);
    if (!isEnd(*col) && !isCommentStart(*col)) {
      const char* resolved = resolveFilename(baseDir, parseFilename(col, col).c_str());
      std::string filename(resolved);
      loadMaterialLibrary(filename.c_str(), callbacks);
    }
  }
}


void objParseUSEMTL(char *line, char*& col, ParserCallbacks* callbacks)
  throw(ParseException)
{
  col = line;
  eatSpace(col, true);
  std::string name = parseIdentifier(col, col);
  callbacks->stringAttributeParsed(ParserCallbacks::kMaterialName, name.c_str());
}


//
// PUBLIC FUNCTIONS
//

void loadOBJ(ParserCallbacks* callbacks, const char* path)
  throw(ParseException)
{
  FILE *f = fopen(path, "r");
  if (f == NULL)
    throw ParseException("Unable to open file %s.\n", path);
  
  char line[_MAX_LINE_LEN];
  char *col;
  unsigned int line_no = 0;

  try {
    callbacks->beginModel(path);
    while (!feof(f)) {
      ++line_no;

      if (fgets(line, _MAX_LINE_LEN, f) == NULL) {
        if (ferror(f))
          throw ParseException("Error reading from file %s", strerror(ferror(f)));
        else
          break;
      }

      col = line;
      eatSpace(col);
      switch (objParseLineType(col, col)) {
        case OBJ_LINETYPE_V:
          callbacks->vec3fAttributeParsed(ParserCallbacks::kCoord, objParseV(col, col));
          break;
        case OBJ_LINETYPE_VT:
          callbacks->vec3fAttributeParsed(ParserCallbacks::kTexCoord, objParseVT(col, col));
          break;
        case OBJ_LINETYPE_VN:
          callbacks->vec3fAttributeParsed(ParserCallbacks::kVertexNormal, objParseVN(col, col));
          break;
        case OBJ_LINETYPE_F:
        case OBJ_LINETYPE_FO:
          callbacks->beginFace();
          objParseFace(col, col, callbacks);
          callbacks->endFace();
          break;
        case OBJ_LINETYPE_USEMTL:
          objParseUSEMTL(col, col, callbacks);
          break;
        case OBJ_LINETYPE_MTLLIB:
          objParseMTLLIB(col, col, callbacks, dirname(const_cast<char*>(path)));
          break;
        case OBJ_LINETYPE_VP:
        case OBJ_LINETYPE_G:
        case OBJ_LINETYPE_S:
        case OBJ_LINETYPE_O:
          // TODO: handle this.
          while (!isEnd(*col))
            ++col;
          break;
        case OBJ_LINETYPE_BLANK:
        case OBJ_LINETYPE_COMMENT:
          // Ignore these types of lines.
          break;
        default:
          throw ParseException("Unknown line type %s", line);
      }

      eatSpace(col);
      if (!isCommentStart(*col) && !isEnd(*col))
        throw ParseException("Unexpected trailing characters: %s", col);
    }
    callbacks->endModel();
    fclose(f);
  } catch (ParseException& ex) {
    fclose(f);
    throw ParseException("[%s: line %d, col %d] %s\n", path, line_no, (int)(col - line), ex.what());
  }
}


} // namespace vgl

