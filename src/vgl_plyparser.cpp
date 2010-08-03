#include "vgl_plyparser.h"

#include "vgl_vec3.h"
#include "ply.h"  // From the thirdparty directory.

#include <cstdio>


namespace vgl {

//
// INTERNAL TYPES
//

struct PLYVertex {
  float x, y, z;
  float u, v;
  float nx, ny, nz;
  float r, g, b;
  float intensity;
  void* otherData;
};

struct PLYFace {
  unsigned char nverts;    /* number of vertex indices in list */
  int *verts;              /* vertex index list */
  void* otherData;
};


//
// GLOBAL VARIABLES
//

PlyProperty vertexProps[] = {
  // Required elements
  { (char*)"x",      PLY_FLOAT, PLY_FLOAT, offsetof(PLYVertex, x), 0, 0, 0, 0 },
  { (char*)"y",      PLY_FLOAT, PLY_FLOAT, offsetof(PLYVertex, y), 0, 0, 0, 0 },
  { (char*)"z",      PLY_FLOAT, PLY_FLOAT, offsetof(PLYVertex, z), 0, 0, 0, 0 },
  // Optional elements
  { (char*)"u",      PLY_FLOAT, PLY_FLOAT, offsetof(PLYVertex, u), 0, 0, 0, 0 },
  { (char*)"v",      PLY_FLOAT, PLY_FLOAT, offsetof(PLYVertex, v), 0, 0, 0, 0 },
  { (char*)"nx",     PLY_FLOAT, PLY_FLOAT, offsetof(PLYVertex,nx), 0, 0, 0, 0 },
  { (char*)"ny",     PLY_FLOAT, PLY_FLOAT, offsetof(PLYVertex,ny), 0, 0, 0, 0 },
  { (char*)"nz",     PLY_FLOAT, PLY_FLOAT, offsetof(PLYVertex,nz), 0, 0, 0, 0 },
  { (char*)"red",    PLY_FLOAT, PLY_FLOAT, offsetof(PLYVertex, r), 0, 0, 0, 0 },
  { (char*)"green",  PLY_FLOAT, PLY_FLOAT, offsetof(PLYVertex, g), 0, 0, 0, 0 },
  { (char*)"blue",   PLY_FLOAT, PLY_FLOAT, offsetof(PLYVertex, b), 0, 0, 0, 0 },
  { (char*)"intensity", PLY_FLOAT, PLY_FLOAT, offsetof(PLYVertex, intensity), 0, 0, 0, 0 },
  { NULL, 0, 0, 0, 0, 0, 0, 0 }
};


PlyProperty faceProps[] = { /* list of property information for a face */
  { (char*)"vertex_indices", PLY_INT, PLY_INT, offsetof(PLYFace, verts),
    1, PLY_CHAR, PLY_CHAR, offsetof(PLYFace, nverts) }
};


bool hasTexCoords = false;
bool hasNormals = false;
bool hasRGB = false;
bool hasIntensity = false;


//
// INTERNAL FUNCTIONS
//

void plyParseFaces(ParserCallbacks* callbacks, PlyFile* plySrc,
                   char* sectionName, int sectionSize, int numProperties)
  throw(ParseException)
{
}


//
// PUBLIC FUNCTIONS
//

void loadPLY(ParserCallbacks* callbacks, const char* path)
  throw(ParseException)
{
  int numElements = 0;
  char** elementNames = NULL;
  int fileType = 0;
  float version = 0.0;
  PlyFile* plySrc = ply_open_for_reading(const_cast<char*>(path),
      &numElements, &elementNames, &fileType, &version);

  for (int i = 0; i < numElements; ++i) {
    char* sectionName = elementNames[i];
    int sectionSize = 0;
    int numProperties = 0;

    PlyProperty** sectionProperties = ply_get_element_description(
        plySrc, sectionName, &sectionSize, &numProperties);

    if (strcmp("vertex", sectionName) == 0) {
      ply_get_property(plySrc, sectionName, &vertexProps[0]); 
      ply_get_property(plySrc, sectionName, &vertexProps[1]); 
      ply_get_property(plySrc, sectionName, &vertexProps[2]);

      // If there are any texture or normal coords, grab them too.
      unsigned int propMask = 0;
      for (int i = 0; i < numProperties; ++i) {
        PlyProperty* availableProp = sectionProperties[i];
        for (int j = 3; vertexProps[j].name != NULL; ++j) {
          PlyProperty* requestedProp = &vertexProps[j];
          if (strcmp(requestedProp->name, availableProp->name) == 0) {
            ply_get_property(plySrc, sectionName, requestedProp);
            propMask |= (1 << j);
          }
        }
      }
      ply_get_other_properties(plySrc, sectionName, offsetof(PLYVertex, otherData));

      hasTexCoords = propMask & (0x3 << 3); // true if the u and v bits are set.
      hasNormals = propMask & (0x7 << 5); // true if the nx, ny and nz bits are set.
      hasRGB = propMask & (0x7 << 8); // true if the r, g and b bits are set.
      hasIntensity = propMask & (0x1 << 11); // true if the intensity bit is set.
  
      for (int vertexNum = 0; vertexNum < sectionSize; ++vertexNum) {
        PLYVertex plyVert;
        ply_get_element(plySrc, &plyVert);

        callbacks->vec3fAttributeParsed(ParserCallbacks::kCoord, Vec3f(plyVert.x, plyVert.y, plyVert.z));
        if (hasTexCoords)
          callbacks->vec3fAttributeParsed(ParserCallbacks::kTexCoord, Vec3f(plyVert.u, plyVert.v, 0.0));
        if (hasNormals)
          callbacks->vec3fAttributeParsed(ParserCallbacks::kVertexNormal, Vec3f(plyVert.nx, plyVert.ny, plyVert.nz));

        if (hasRGB)
          callbacks->vec3fAttributeParsed(ParserCallbacks::kDiffuseColor, Vec3f(plyVert.r, plyVert.g, plyVert.b));
        else if (hasIntensity)
          callbacks->vec3fAttributeParsed(ParserCallbacks::kIntensity, Vec3f(plyVert.intensity, plyVert.intensity, plyVert.intensity));
      }
    } else if (strcmp("face", sectionName) == 0) {
      ply_get_property(plySrc, sectionName, &faceProps[0]);
      ply_get_other_properties(plySrc, sectionName, offsetof(PLYFace, otherData));

      for (int i = 0; i < sectionSize; ++i) {
        PLYFace plyFace;
        ply_get_element(plySrc, &plyFace);

        callbacks->beginFace();
        for (int j = 0; j < plyFace.nverts; ++j) {
          callbacks->beginVertex();
          int v = plyFace.verts[j];
          callbacks->indexAttributeParsed(ParserCallbacks::kCoordRef, v);
          if (hasTexCoords)
            callbacks->indexAttributeParsed(ParserCallbacks::kTexCoordRef, v);
          if (hasNormals)
            callbacks->indexAttributeParsed(ParserCallbacks::kVertexNormal, v);
          if (hasRGB)
            callbacks->indexAttributeParsed(ParserCallbacks::kDiffuseColor, v);
          if (hasIntensity)
            callbacks->indexAttributeParsed(ParserCallbacks::kIntensity, v);
          callbacks->endVertex();
        }
        callbacks->endFace();
      }
    } else {
      ply_get_other_element(plySrc, sectionName, sectionSize);
    }
  }

  ply_close(plySrc);
}


} // namespace vgl

