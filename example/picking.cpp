#include "vgl.h"
#include <cstring> // for memcpy

//
// Constants
//

const unsigned int kBufferWidth = 800;
const unsigned int kBufferHeight = 600;

const char* kPickingVertexShaderPath = "pick.vert";
const char* kPickingFragmentShaderPath = "pick.frag";
const char* kDrawingVertexShaderPath = "draw.vert";
const char* kDrawingFragmentShaderPath = "draw.frag";


//
// PickingRenderer class
//

class PickingRenderer : public vgl::Renderer
{
public:
  PickingRenderer();
  virtual ~PickingRenderer();

  virtual void setup();
  virtual void render();

  bool objectAt(unsigned int x, unsigned int y, unsigned int& type, unsigned int& id);

private:
  void renderPickImage();
  void renderVisualImage();
  void cube(GLenum polygonMode, const vgl::Vec3f color);

private:
  GLuint _fbo;        // The framebuffer object we draw the pick info to.
  GLuint _pickBuffer; // The renderbuffer object for the pick data (false coloured with object IDs).
  GLuint _pickDepth;  // The renderbuffer object for the depth data while picking.
  GLuint _pbo;        // Pixel buffer for reading the pick data back into CPU memory.

  GLuint _drawingProg;  // The shader program for rendering the drawing.
  GLuint _pickingProg;  // The shader program for rendering the picking data

  GLuint _vbo[2];

  float* _pickData;  // The local copy of the pick data.
};


//
// PickingViewer class
//

class PickingViewer : public vgl::Viewer
{
public:
  // Extra actions
  enum {
    ACTION_PICK_FACE = 100, // Selected a face.
    ACTION_PICK_EDGE,       // Selected an edge.
    ACTION_PICK_VERTEX      // Selected a vertex.
  };

public:
  PickingViewer();
  virtual ~PickingViewer();

protected:
  virtual int actionForMousePress(int button, int state, int x, int y);
  virtual void actionHandler(int action);

protected:
  unsigned int _selectedFace;
  unsigned int _selectedEdge;
  unsigned int _selectedVertex;
};


//
// PickingRenderer methods
//

PickingRenderer::PickingRenderer() :
  _fbo(0),
  _pickBuffer(0),
  _pickDepth(0),
  _pbo(0),
  _pickingProg(0),
  _pickData(NULL)
{
  _vbo[0] = _vbo[1] = 0;
  _pickData = new float[3 * kBufferWidth * kBufferHeight];
}


PickingRenderer::~PickingRenderer()
{
  if (_fbo)
    glDeleteFramebuffers(1, &_fbo);
  if (_pickBuffer)
    glDeleteRenderbuffers(1, &_pickBuffer);
  if (_pickDepth)
    glDeleteRenderbuffers(1, &_pickDepth);
  if (_pbo)
    glDeleteBuffers(1, &_pbo);

  // TODO: clean up the shaders

  if (_vbo[0] || _vbo[1])
    glDeleteBuffers(2, _vbo);

  delete[] _pickData;
}


void PickingRenderer::setup()
{
  glEnable(GL_DEPTH_TEST);
  glPointSize(5);

  // Set up the pick buffer.
  glGenRenderbuffers(1, &_pickBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, _pickBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, kBufferWidth, kBufferHeight);

  // Set up the depth buffer.
  glGenRenderbuffers(1, &_pickDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, _pickDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, kBufferWidth, kBufferHeight);

  // Set up the FBO.
  glGenFramebuffers(1, &_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _pickBuffer);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _pickDepth);
  
  // Set up the PBO for reading back the pick data from the FBO.
  glGenBuffers(1, &_pbo);
  glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
  glBufferData(GL_PIXEL_PACK_BUFFER, sizeof(float) * 3 * kBufferWidth * kBufferWidth, NULL, GL_DYNAMIC_READ);

  // Check that everything is OK.
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    fprintf(stderr, "Framebuffer incomplete. Aborting.\n");
    exit(-1);
  }

  // Load and link the drawing shaders
  GLuint drawingVertexShader = vgl::loadShader(GL_VERTEX_SHADER, kDrawingVertexShaderPath);
  GLuint drawingFragmentShader = vgl::loadShader(GL_FRAGMENT_SHADER, kDrawingFragmentShaderPath);
  if (drawingVertexShader == 0 || drawingFragmentShader == 0) {
    fprintf(stderr, "Unable to load drawing shaders. Aborting.\n");
    exit(-1);
  }
  _drawingProg = vgl::linkShader(drawingVertexShader, drawingFragmentShader);

  // Load and link the picking shaders
  GLuint pickingVertexShader = vgl::loadShader(GL_VERTEX_SHADER, kPickingVertexShaderPath);
  GLuint pickingFragmentShader = vgl::loadShader(GL_FRAGMENT_SHADER, kPickingFragmentShaderPath);
  if (pickingVertexShader == 0 || pickingFragmentShader == 0) {
    fprintf(stderr, "Unable to load picking shaders. Aborting.\n");
    exit(-1);
  }
  _pickingProg = vgl::linkShader(pickingVertexShader, pickingFragmentShader);

  // Set up the vertex buffer for the cube.
  const unsigned int kFloatsPerVertex = 6; // x, y, z, type, object ID, vertex ID
  const unsigned int kNumVertices = 8;
  const unsigned int kNumTriangles = 12;
  const vgl::Vec3f lo(-0.5, -0.5, -0.5);
  const vgl::Vec3f hi(0.5, 0.5, 0.5);
  const float vertexData[] = {
    lo.x, lo.y, lo.z, 1, 1, 0,
    hi.x, lo.y, lo.z, 1, 1, 1,
    hi.x, hi.y, lo.z, 1, 1, 2,
    lo.x, hi.y, lo.z, 1, 1, 3,
  
    lo.x, lo.y, hi.z, 1, 1, 4,
    hi.x, lo.y, hi.z, 1, 1, 5,
    hi.x, hi.y, hi.z, 1, 1, 6,
    lo.x, hi.y, hi.z, 1, 1, 7
  };
  const unsigned int indexData[] = {
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
  glGenBuffers(2, _vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, kFloatsPerVertex * kNumVertices * sizeof(float), vertexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, kNumTriangles * 3 * sizeof(unsigned int), indexData, GL_STATIC_DRAW);
}


void PickingRenderer::render()
{
  // Render the selectable data
  glBindFramebuffer(GL_FRAMEBUFFER, _pickBuffer);
  renderPickImage();

  // Start reading the selectable data back asynchronously
  glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
  glReadPixels(0, 0, kBufferWidth, kBufferHeight, GL_RGB, GL_UNSIGNED_INT, (void*)0);

  // Map the selectable data so we can use it.
  float* buf = (float*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
  if (buf) {
    memcpy(_pickData, buf, sizeof(float) * 3 * kBufferWidth * kBufferHeight);
    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
  }
  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

  // Render the visual data
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //renderVisualImage();
  renderPickImage();
}


void PickingRenderer::renderPickImage()
{
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLineWidth(8.0);
  glUseProgram(_pickingProg);
  glEnableClientState(GL_COLOR_ARRAY);
  cube(GL_FILL, vgl::Vec3f(0.3, 0.3, 0.3)); // Draw the faces
  cube(GL_LINE, vgl::Vec3f(0.6, 0.6, 0.6)); // Draw the lines 
  cube(GL_POINT, vgl::Vec3f(0.9, 0.9, 0.9)); // Draw the points
}


void PickingRenderer::renderVisualImage()
{
  glClearColor(0.2, 0.2, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLineWidth(1.0);
  glUseProgram(_drawingProg);
  glDisableClientState(GL_COLOR_ARRAY);
  cube(GL_FILL, vgl::Vec3f(0.3, 0.3, 0.3)); // Draw the faces
  cube(GL_LINE, vgl::Vec3f(0.6, 0.6, 0.6)); // Draw the lines 
  cube(GL_POINT, vgl::Vec3f(0.9, 0.9, 0.9)); // Draw the points
}


bool PickingRenderer::objectAt(unsigned int sx, unsigned int sy, unsigned int& type, unsigned int& id)
{
  unsigned int index = (sy * kBufferWidth + sx) * 3;

  if (_pickData[index] == 0)
    return false;

  type = _pickData[index];
  id = _pickData[index + 1];
  return true;
}


void PickingRenderer::cube(GLenum polygonMode, const vgl::Vec3f color)
{
  switch (polygonMode) {
  case GL_LINE:
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1,  -3);
    break;
  case GL_POINT:
    glPolygonOffset(-1,  -5);
    glEnable(GL_POLYGON_OFFSET_POINT);
    break;
  case GL_FILL:
  default:
    glDisable(GL_POLYGON_OFFSET_POINT);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(0, 0);
    break;
  }
  glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
  glColor3fv(color.data);

  glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[1]);

  const unsigned int kStride = 6 * sizeof(float);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, kStride, 0);
  glColorPointer(3, GL_FLOAT, kStride, (const GLvoid*)(3 * sizeof(float)));

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDisable(GL_POLYGON_OFFSET_LINE);
  glDisable(GL_POLYGON_OFFSET_POINT);
}


//
// PickingViewer methods
//

PickingViewer::PickingViewer() :
  vgl::Viewer("Picking Example", kBufferWidth, kBufferHeight, new PickingRenderer())
{
}


PickingViewer::~PickingViewer()
{
}


int PickingViewer::actionForMousePress(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !_shiftDown && !_altDown && !_ctrlDown) {
    _selectedFace = 0;
    _selectedEdge = 0;
    _selectedVertex = 0;

    PickingRenderer* picker = (PickingRenderer*)_renderer;
    unsigned int type = 0;
    unsigned int id = 0;
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
      unsigned int sx = (unsigned int)x * kBufferWidth / _width;
      unsigned int sy = (unsigned int)y * kBufferHeight / _height;
      fprintf(stderr, "Click at %d, %d scaled to %u, %u\n", x, y, sx, sy);
      if (picker->objectAt(sx, sy, type, id)) {
        switch (type) {
          case 1: _selectedFace = id;   return ACTION_PICK_FACE;
          case 2: _selectedEdge = id;   return ACTION_PICK_EDGE;
          case 3: _selectedVertex = id; return ACTION_PICK_VERTEX;
          default:
            fprintf(stderr, "Hit unknown object type %u with id %u.\n", type, id);
            break;
        }
      }
    }
  }

  return vgl::Viewer::actionForMousePress(button, state, x, y);
}


void PickingViewer::actionHandler(int action)
{
  switch (action) {
  case ACTION_PICK_FACE:    fprintf(stderr, "Picked face %u\n", _selectedFace); break;
  case ACTION_PICK_EDGE:    fprintf(stderr, "Picked edge %u\n", _selectedEdge); break;
  case ACTION_PICK_VERTEX:  fprintf(stderr, "Picked vertex %u\n", _selectedVertex); break;
  default:
    vgl::Viewer::actionHandler(action);
    break;
  }
}


//
// Functions
//

int main(int argc, char** argv)
{
  PickingRenderer renderer;
  PickingViewer viewer;
  viewer.run();
}

