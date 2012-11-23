// Example of using VGL's model loading functions to print out info about a
// model to the terminal. This is a command line app, no gui involved.

#include "vgl.h"

#include <cstdio>
#include <cstring>
#include <vector>


class ModelInfo : public vgl::ParserCallbacks
{
public:
  ModelInfo() :
    _filename(),
    _totalFaces(0), _triangles(0), _quads(0), _polygons(0),
    _totalVertices(0), _totalNormals(0), _totalTexCoords(0),
    _totalMaterials(0), _totalTextures(0),
    _low(1e20, 1e20, 1e20), _high(-1e20, -1e20, -1e20),
    _vertsForCurrentFace(0), _maxVertsPerFace(0)
  {
  }

  virtual void beginModel(const char* path)
  {
    _filename = path;
  }

  virtual void endModel()
  {
    printStats();
  }

  virtual void beginFace()
  {
    ++_totalFaces;
    _vertsForCurrentFace = 0;
  }

  virtual void endFace()
  {
    if (_vertsForCurrentFace == 3)
      ++_triangles;
    else if (_vertsForCurrentFace == 4)
      ++_quads;
    else
      ++_polygons;

    if (_vertsForCurrentFace > _maxVertsPerFace)
      _maxVertsPerFace = _vertsForCurrentFace;
  }

  virtual void beginMaterial(const char* name)
  {
    ++_totalMaterials;
  }

  virtual void indexAttributeParsed(int attr, size_t value)
  {
    if (attr == kCoordRef)
      ++_vertsForCurrentFace;
  }

  virtual void vec3fAttributeParsed(int attr, const Eigen::Vector3f& value)
  {
    if (attr == ParserCallbacks::kCoord) {
      ++_totalVertices;
      _low = _low.cwiseMin(value);
      _high = _low.cwiseMax(value);
    }
    else if (attr == ParserCallbacks::kNormal) {
      ++_totalNormals;
    }
    else if (attr == ParserCallbacks::kTexCoord) {
      ++_totalTexCoords;
    }
  }

  virtual void textureAttributeParsed(int attr, const char* path)
  {
    ++_totalTextures;
  }

private:
  void printStats()
  {
    printf("%s\n", _filename.c_str());
    printf("x = %f - %f\n", _low.x(), _high.x());
    printf("y = %f - %f\n", _low.y(), _high.y());
    printf("z = %f - %f\n", _low.z(), _high.z());
    printf("\n");
    printf("%lu faces of up to %lu vertices\n", _totalFaces, _maxVertsPerFace);
    printf("- %lu triangles\n", _triangles);
    printf("- %lu quads\n", _quads);
    printf("- %lu larger polygons\n", _polygons);
    printf("\n");
    printf("%lu vertices\n", _totalVertices);
    printf("%lu texture coords\n", _totalTexCoords);
    printf("%lu normals\n", _totalNormals);
    printf("\n");
    printf("%lu materials\n", _totalMaterials);
    printf("%lu textures\n", _totalTextures);
  }

private:
  std::string _filename;

  size_t _totalFaces;
  size_t _triangles;
  size_t _quads;
  size_t _polygons;

  size_t _totalVertices;
  size_t _totalNormals;
  size_t _totalTexCoords;

  size_t _totalMaterials;
  size_t _totalTextures;

  Eigen::Vector3f _low;
  Eigen::Vector3f _high;

  size_t _vertsForCurrentFace;
  size_t _maxVertsPerFace;
};


int main(int argc, char** argv)
{
  if (argc <= 1) {
    fprintf(stderr, "Usage: %s <model> [ <model> ... ]\n", argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; ++i) {
    if (i > 1)
      printf("---\n");

    ModelInfo info;
    vgl::loadModel(&info, argv[i]);
  }

  return 0;
}

