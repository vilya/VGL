#include "vgl.h"
#include <cstring> // for memcpy

//
// Constants
//

const unsigned int kBufferWidth = 800;
const unsigned int kBufferHeight = 600;


//
// Buffer
//

typedef unsigned char BufType;
const GLenum kBufInternalFormat = GL_RGB8;
const GLenum kBufFormat = GL_RGB;
const GLenum kBufType = GL_UNSIGNED_BYTE;

const BufType kTypeShift = sizeof(BufType) * 8 - 2;
const BufType kIDShift = sizeof(BufType) * 8 - 8;
#define PickColor glColor3ub

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

  void nextDrawingMode();

private:
  void renderPickImage();
  void renderVisualImage();
  void drawMode(GLenum polygonMode);
  void cube(const vgl::Vec3f& color);
  void pickableCube(BufType itemType);

private:
  GLuint _fbo;        // The framebuffer object we draw the pick info to.
  GLuint _pickBuffer; // The renderbuffer object for the pick data (false coloured with object IDs).
  GLuint _pickDepth;  // The renderbuffer object for the depth data while picking.
  GLuint _pbo;        // Pixel buffer for reading the pick data back into CPU memory.

  unsigned int _mode; // The user-selected drawing mode.
  BufType* _pickData; // The local copy of the pick data.
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
    ACTION_PICK_VERTEX,     // Selected a vertex.

    ACTION_NEXT_MODE        // Switch to the next drawing mode.
  };

public:
  PickingViewer();
  virtual ~PickingViewer();

protected:
  virtual int actionForMousePress(int button, int state, int x, int y);
  virtual int actionForKeyPress(unsigned int key, int x, int y);
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
  _mode(0),
  _pickData(NULL)
{
  _pickData = new BufType[3 * kBufferWidth * kBufferHeight];
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

  delete[] _pickData;
}


void PickingRenderer::setup()
{
  glEnable(GL_DEPTH_TEST);
 
  // Set up the pick buffer.
  glGenRenderbuffers(1, &_pickBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, _pickBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, kBufInternalFormat, kBufferWidth, kBufferHeight);

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
  glBufferData(GL_PIXEL_PACK_BUFFER, sizeof(BufType) * 3 * kBufferWidth * kBufferWidth, NULL, GL_DYNAMIC_READ);

  // Check that everything is OK.
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    fprintf(stderr, "Framebuffer incomplete. Aborting.\n");
    exit(-1);
  }
}


void PickingRenderer::render()
{
  // Render the selectable data
  glBindFramebuffer(GL_FRAMEBUFFER, _pickBuffer);
  renderPickImage();

  // Start reading the selectable data back asynchronously
  glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
  glReadPixels(0, 0, kBufferWidth, kBufferHeight, kBufFormat, kBufType, (void*)0);

  // Render the visual data
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  if (_mode == 0)
    renderVisualImage();
  else
    renderPickImage();

  // Map the selectable data so we can use it.
  float* buf = (float*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
  if (buf) {
    memcpy(_pickData, buf, sizeof(BufType) * 3 * kBufferWidth * kBufferHeight);
    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
  }
  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}


bool PickingRenderer::objectAt(unsigned int sx, unsigned int sy, unsigned int& type, unsigned int& id)
{
  sy = kBufferHeight + 1 - sy;
  unsigned int index = (sy * kBufferWidth + sx) * 3;

  if (_pickData[index] == 0)
    return false;

  type = (unsigned int)_pickData[index] >> kTypeShift;
  id = (unsigned int)_pickData[index + 2] >> kIDShift;
  return true;
}


void PickingRenderer::nextDrawingMode()
{
  _mode = 1 - _mode;
  glutPostRedisplay();
}


void PickingRenderer::renderPickImage()
{
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLineWidth(8.0);
  glPointSize(10.0);

  drawMode(GL_FILL);
  pickableCube(1); // Draw the faces
  drawMode(GL_LINE);
  pickableCube(2); // Draw the lines 
  drawMode(GL_POINT);
  pickableCube(3); // Draw the points
}


void PickingRenderer::renderVisualImage()
{
  glClearColor(0.2, 0.2, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLineWidth(1.0);
  glPointSize(5.0);

  drawMode(GL_FILL);
  cube(vgl::Vec3f(0.3, 0.3, 0.3)); // Draw the faces
  drawMode(GL_LINE);
  cube(vgl::Vec3f(0.6, 0.6, 0.6)); // Draw the lines 
  drawMode(GL_POINT);
  cube(vgl::Vec3f(0.9, 0.9, 0.9)); // Draw the points
}


void PickingRenderer::drawMode(GLenum polygonMode)
{
  glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
  switch (polygonMode) {
  case GL_LINE:
    glDisable(GL_POLYGON_OFFSET_POINT);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1,  -3);
    break;
  case GL_POINT:
    glPolygonOffset(-1,  -5);
    glEnable(GL_POLYGON_OFFSET_POINT);
    glDisable(GL_POLYGON_OFFSET_LINE);
    break;
  case GL_FILL:
  default:
    glDisable(GL_POLYGON_OFFSET_POINT);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(0, 0);
    break;
  }
}


void PickingRenderer::cube(const vgl::Vec3f& color)
{
  vgl::Vec3f lo(-0.5, -0.5, -0.5);
  vgl::Vec3f hi( 0.5,  0.5,  0.5);

  glColor3fv(color.data);
  glBegin(GL_QUADS);

  glVertex3f(lo.x, lo.y, lo.z);
  glVertex3f(hi.x, lo.y, lo.z);
  glVertex3f(hi.x, hi.y, lo.z);
  glVertex3f(lo.x, hi.y, lo.z);

  glVertex3f(lo.x, lo.y, hi.z);
  glVertex3f(hi.x, lo.y, hi.z);
  glVertex3f(hi.x, hi.y, hi.z);
  glVertex3f(lo.x, hi.y, hi.z);

  glVertex3f(lo.x, lo.y, lo.z);
  glVertex3f(lo.x, lo.y, hi.z);
  glVertex3f(lo.x, hi.y, hi.z);
  glVertex3f(lo.x, hi.y, lo.z);

  glVertex3f(hi.x, lo.y, lo.z);
  glVertex3f(hi.x, lo.y, hi.z);
  glVertex3f(hi.x, hi.y, hi.z);
  glVertex3f(hi.x, hi.y, lo.z);

  glVertex3f(lo.x, lo.y, lo.z);
  glVertex3f(hi.x, lo.y, lo.z);
  glVertex3f(hi.x, lo.y, hi.z);
  glVertex3f(lo.x, lo.y, hi.z);

  glVertex3f(lo.x, hi.y, lo.z);
  glVertex3f(hi.x, hi.y, lo.z);
  glVertex3f(hi.x, hi.y, hi.z);
  glVertex3f(lo.x, hi.y, hi.z);

  glEnd();
}


void PickingRenderer::pickableCube(BufType itemType)
{
  vgl::Vec3f lo(-0.5, -0.5, -0.5);
  vgl::Vec3f hi( 0.5,  0.5,  0.5);

  const float kVerts[24][3] = {
    { lo.x, lo.y, lo.z },
    { hi.x, lo.y, lo.z },
    { hi.x, hi.y, lo.z },
    { lo.x, hi.y, lo.z },

    { lo.x, lo.y, hi.z },
    { hi.x, lo.y, hi.z },
    { hi.x, hi.y, hi.z },
    { lo.x, hi.y, hi.z },

    { lo.x, lo.y, lo.z },
    { lo.x, lo.y, hi.z },
    { lo.x, hi.y, hi.z },
    { lo.x, hi.y, lo.z },

    { hi.x, lo.y, lo.z },
    { hi.x, lo.y, hi.z },
    { hi.x, hi.y, hi.z },
    { hi.x, hi.y, lo.z },

    { lo.x, lo.y, lo.z },
    { hi.x, lo.y, lo.z },
    { hi.x, lo.y, hi.z },
    { lo.x, lo.y, hi.z },

    { lo.x, hi.y, lo.z },
    { hi.x, hi.y, lo.z },
    { hi.x, hi.y, hi.z },
    { lo.x, hi.y, hi.z }
  };

  switch (itemType) {
  case 1: // Faces
    glBegin(GL_QUADS);
    for (unsigned int i = 0; i < 6; ++i) {
      PickColor(itemType << kTypeShift, 0, i << kIDShift);
      for (int f = i * 4; f < (i + 1) * 4; ++f)
        glVertex3fv(kVerts[f]);
    }
    glEnd();
    break;
  case 2: // Edges
    glBegin(GL_LINES);
    for (unsigned int i = 0; i < 6; ++i) {
      int prev = i * 4 + 3;
      for (unsigned int f = i * 4; f < (i + 1) * 4; ++f) {
        PickColor(itemType << kTypeShift, 0, f << kIDShift);
        glVertex3fv(kVerts[prev]);
        glVertex3fv(kVerts[f]);
        prev = f;
      }
    }
    glEnd();
    break;
  case 3: // Verts
    glBegin(GL_QUADS);
    for (unsigned int i = 0; i < 24; ++i) {
      PickColor(itemType << kTypeShift, 0, i << kIDShift);
      glVertex3fv(kVerts[i]);
    }
    glEnd();
    break;
  default: // Regular drawing.
    break;
  }
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
      if (picker->objectAt(sx, sy, type, id)) {
        switch (type) {
          case 1: _selectedFace = id;   return ACTION_PICK_FACE;
          case 2: _selectedEdge = id;   return ACTION_PICK_EDGE;
          case 3: _selectedVertex = id; return ACTION_PICK_VERTEX;
          default:
            fprintf(stderr, "Unknown object type %u with id %u.\n", type, id);
            break;
        }
      }
      else {
        fprintf(stderr, "Background.\n");
      }
    }
  }

  return vgl::Viewer::actionForMousePress(button, state, x, y);
}


int PickingViewer::actionForKeyPress(unsigned int key, int x, int y)
{
  if (key == ' ')
    return ACTION_NEXT_MODE;
  return vgl::Viewer::actionForKeyPress(key, x, y);
}


void PickingViewer::actionHandler(int action)
{
  switch (action) {
  case ACTION_PICK_FACE:    fprintf(stderr, "Picked face %u\n", _selectedFace); break;
  case ACTION_PICK_EDGE:    fprintf(stderr, "Picked edge %u\n", _selectedEdge); break;
  case ACTION_PICK_VERTEX:  fprintf(stderr, "Picked vertex %u\n", _selectedVertex); break;
  case ACTION_NEXT_MODE:    ((PickingRenderer*)_renderer)->nextDrawingMode(); break;
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

