#include "vgl.h"
#include <cstring> // for memcpy

//
// Constants
//

const unsigned int kBufferWidth = 1024;
const unsigned int kBufferHeight = 1024;


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
  void cube(GLenum polygonMode, const vgl::Vec3f color);

private:
  GLuint _fbo;        // The framebuffer object we draw the pick info to.
  GLuint _pickBuffer; // The renderbuffer object for the pick data (false coloured with object IDs).
  GLuint _pickDepth;  // The renderbuffer object for the depth data while picking.
  GLuint _pbo;        // Pixel buffer for reading the pick data back into CPU memory.

  unsigned int *_pickData;  // The local copy of the pick data.
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
  _pickData(NULL)
{
  _pickData = new unsigned int[3 * kBufferWidth * kBufferHeight];
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
  glBufferData(GL_PIXEL_PACK_BUFFER, sizeof(unsigned int) * 3 * kBufferWidth * kBufferWidth, NULL, GL_DYNAMIC_READ);

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
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //cube(GL_FILL, vgl::Vec3f(1.0/4294967295.0, 0.3, 0.3)); // Draw the faces
  //cube(GL_LINE, vgl::Vec3f(2.0/4294967295.0, 0.6, 0.6)); // Draw the lines 
  //cube(GL_POINT, vgl::Vec3f(3.0/4294967295.0, 0.9, 0.9)); // Draw the points
  cube(GL_FILL, vgl::Vec3f(0.1, 0.0, 0.0)); // Draw the faces
  cube(GL_LINE, vgl::Vec3f(0.2, 0.0, 0.0)); // Draw the lines 
  cube(GL_POINT, vgl::Vec3f(0.3, 0.0, 0.0)); // Draw the points

  // Start reading the selectable data back asynchronously
  glReadBuffer(GL_BACK);
  glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
  glReadPixels(0, 0, kBufferWidth, kBufferHeight, GL_RGB, GL_UNSIGNED_INT, (void*)0);

  // Render the visual data
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.2, 0.2, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  cube(GL_FILL, vgl::Vec3f(0.3, 0.3, 0.3)); // Draw the faces
  cube(GL_LINE, vgl::Vec3f(0.6, 0.6, 0.6)); // Draw the lines 
  cube(GL_POINT, vgl::Vec3f(0.9, 0.9, 0.9)); // Draw the points

  // Map the selectable data so we can use it.
  unsigned int* buf = (unsigned int*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
  if (buf) {
    memcpy(_pickData, buf, sizeof(unsigned int) * 3 * kBufferWidth * kBufferHeight);
    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
  }
  glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
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
  const vgl::Vec3f lo(-0.5, -0.5, -0.5);
  const vgl::Vec3f hi(0.5, 0.5, 0.5);

  glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
  switch (polygonMode) {
  case GL_LINE:
    glPolygonOffset(0,  -2);
    break;
  case GL_POINT:
    glPolygonOffset(0,  -4);
    break;
  case GL_FILL:
  default:
    glPolygonOffset(0, 0);
    break;
  }
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

  glEnd();
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

