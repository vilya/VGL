#include "vgl.h"
#include <cstring> // for memcpy
#include <vector>


//
// Constants
//

const char* kVertexShader = "perface.v.glsl";
const char* kGeometryShader = "perface.g.glsl";
const char* kFragmentShader = "perface.f.glsl";


//
// PerFaceRenderer class
//

class PerFaceRenderer : public vgl::Renderer
{
public:
  PerFaceRenderer();
  virtual ~PerFaceRenderer();

  virtual void setup();
  virtual void render();

private:
  void uploadCube();
  void uploadTexture();

private:
  GLuint _prog;
  GLuint _buffers[2];
  GLuint _texture;
};


//
// PerFaceRenderer methods
//

PerFaceRenderer::PerFaceRenderer() :
  _prog(0),
  _texture(0)
{
  _buffers[0] = _buffers[1] = 0;
}


PerFaceRenderer::~PerFaceRenderer()
{
  glDeleteProgram(_prog);
  glDeleteBuffers(2, _buffers);
  glDeleteTextures(1, &_texture);
}


void PerFaceRenderer::setup()
{
  glEnable(GL_DEPTH_TEST);

  // Load and link the shaders.
  GLuint vertexShader = vgl::loadShader(GL_VERTEX_SHADER, kVertexShader);
  GLuint geometryShader = vgl::loadShader(GL_GEOMETRY_SHADER_EXT, kGeometryShader);
  GLuint fragmentShader = vgl::loadShader(GL_FRAGMENT_SHADER, kFragmentShader);
  _prog = vgl::linkShader(vertexShader, fragmentShader, geometryShader, GL_TRIANGLES, GL_TRIANGLE_STRIP, 3);

  // Flag the shaders for deletion later (as soon as they become detached)
  glDeleteShader(vertexShader);
  glDeleteShader(geometryShader);
  glDeleteShader(fragmentShader);

  // Set up the vertex buffer with data for a cube.
  glGenBuffers(2, _buffers);
  glBindBuffer(GL_ARRAY_BUFFER, _buffers[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[1]);
  uploadCube();
  vgl::checkGLError("Error during setup");

  // Set up a 1D texture to use as a lookup table for the face colours.
  glGenTextures(1, &_texture);
  glBindTexture(GL_TEXTURE_1D, _texture);
  uploadTexture();
}


void PerFaceRenderer::render()
{
  glUseProgram(_prog);
  glBindBuffer(GL_ARRAY_BUFFER, _buffers[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[1]);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glUseProgram(0);
}


void PerFaceRenderer::uploadCube()
{
  Eigen::Vector3f lo(-0.5, -0.5, -0.5);
  Eigen::Vector3f hi( 0.5,  0.5,  0.5);

  const float kVerts[8][3] = {
    { lo.x(), lo.y(), lo.z() },
    { hi.x(), lo.y(), lo.z() },
    { hi.x(), hi.y(), lo.z() },
    { lo.x(), hi.y(), lo.z() },

    { lo.x(), lo.y(), hi.z() },
    { hi.x(), lo.y(), hi.z() },
    { hi.x(), hi.y(), hi.z() },
    { lo.x(), hi.y(), hi.z() }
  };

  const GLuint kIndices[] = {
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    1, 5, 6,
    6, 2, 1,

    0, 4, 7,
    7, 3, 0,

    3, 2, 6,
    6, 7, 3,

    0, 1, 5,
    5, 4, 0
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(kVerts), kVerts, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices, GL_STATIC_DRAW);
}


void PerFaceRenderer::uploadTexture()
{
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP); // Is this needed for a 1D texture?

  const float kFaceColours[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f
  };
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, 6, 0, GL_RGB, GL_FLOAT, kFaceColours);
}


//
// Functions
//

int main(int argc, char** argv)
{
  PerFaceRenderer renderer;
  vgl::Viewer viewer("Per-face Colours", 800, 600, &renderer);
  viewer.run();
}

