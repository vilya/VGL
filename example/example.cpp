#include "vgl.h"

#include <cstdio>
#include <cstring>
#include <vector>


struct ExampleMesh
{
  std::vector<vgl::Vec3f> coords;
  std::vector<GLuint> indexes;

  ExampleMesh() : coords(), indexes() {}
};


class MeshBuilder : public vgl::ParserCallbacks
{
public:
  MeshBuilder(ExampleMesh* mesh) : _mesh(mesh) {}

  virtual void beginModel(const char* path)
  {
    _mesh->coords.clear();
    _mesh->indexes.clear();
  }

  virtual void indexAttributeParsed(int attr, size_t value)
  {
    if (attr == ParserCallbacks::kCoordRef)
      _mesh->indexes.push_back(value);
  }

  virtual void vec3fAttributeParsed(int attr, const vgl::Vec3f& value)
  {
    if (attr == ParserCallbacks::kCoord)
      _mesh->coords.push_back(value);
  }

private:
  ExampleMesh* _mesh;
};


class ExampleRenderer : public vgl::Renderer
{
public:
  ExampleRenderer(ExampleMesh* mesh) : _mesh(mesh), _bufferID(0), _indexesID(0) {}

  virtual void setup() {
    size_t bufferSize = sizeof(float) * 3 * _mesh->coords.size();

    // Get a buffer ID for the coords & allocate space for them. 
    glGenBuffers(1, &_bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, _bufferID);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
    vgl::checkGLError("Error setting up vertex buffer");
    
    // Copy the coords into the vertex buffer.
    GLfloat* vertexBuffer = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(vertexBuffer, &_mesh->coords[0], bufferSize);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    vgl::checkGLError("Error filling vertex buffer");

    // Get a buffer ID for the indexes, upload them and clear out the local copy.
    bufferSize = sizeof(GLuint) * _mesh->indexes.size();
    glGenBuffers(1, &_indexesID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexesID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
    vgl::checkGLError("Error setting up index buffer");

    // Copy the indexes into the index buffer
    GLuint* indexBuffer = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(indexBuffer, &_mesh->indexes[0], bufferSize);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    vgl::checkGLError("Error filling index buffer");
  }

  virtual void render() {
    glBindBuffer(GL_ARRAY_BUFFER, _bufferID);
    vgl::checkGLError("Unable to bind vertex buffer");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexesID);
    vgl::checkGLError("Unable to bind index buffer");

    glEnableClientState(GL_VERTEX_ARRAY);
    vgl::checkGLError("Unable to enable vertex array client state");
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawElements(GL_TRIANGLES, _mesh->indexes.size(), GL_UNSIGNED_INT, 0);
    vgl::checkGLError("Unable to draw mesh");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    vgl::checkGLError("Unable to unbind vertex buffer");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    vgl::checkGLError("Unable to unbind index buffer");
  }

private:
  ExampleMesh* _mesh;
  GLuint _bufferID, _indexesID;
};


int main(int argc, char** argv)
{
  if (argc <= 1) {
    fprintf(stderr, "Usage: %s <obj-file>\n", argv[0]);
    return 0;
  }

  ExampleMesh* mesh = new ExampleMesh();
  MeshBuilder* builder = new MeshBuilder(mesh);
  vgl::loadModel(builder, argv[1]);

  ExampleRenderer renderer(mesh);

  vgl::Viewer viewer("Example Viewer", 800, 600, &renderer);
  viewer.run();
}

