#include "vgl_viewer.h"

#include "vgl_arcballcamera.h"
#include "vgl_camera.h"
#include "vgl_renderer.h"
#include "vgl_vec3.h"

#include <cstdlib>

#ifdef linux
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif

namespace vgl {

//
// GLOBAL VARIABLES
//

static Viewer* gCurrentViewer = NULL;


//
// LOCAL FUNCTIONS
//

static void render()
{
  if (gCurrentViewer != NULL)
    gCurrentViewer->render();
}


static void resize(int width, int height)
{
  if (gCurrentViewer != NULL)
    gCurrentViewer->resize(width, height);
}


static void keyPressed(unsigned char key, int x, int y)
{
  if (gCurrentViewer != NULL)
    gCurrentViewer->keyPressed(key, x, y);
}


static void mousePressed(int button, int state, int x, int y)
{
  if (gCurrentViewer != NULL)
    gCurrentViewer->mousePressed(button, state, x, y);
}


static void mouseDragged(int x, int y)
{
  if (gCurrentViewer != NULL)
    gCurrentViewer->mouseDragged(x, y);
}


//
// METHODS
//

Viewer::Viewer(
    const char* title,
    int width,
    int height,
    Renderer* renderer,
    Camera* camera) :
  _windowWidth(width),
  _windowHeight(height),
  _width(width),
  _height(height),
  _mouseX(0),
  _mouseY(0),
  _prevMouseX(0),
  _prevMouseY(0),
  _state(0),
  _key(0),
  _button(-1),
  _shiftDown(false),
  _altDown(false),
  _ctrlDown(false),
  _fullscreen(false),
  _renderer(renderer),
  _camera(camera)
{
  int argc = 0;
  char* argv[] = { NULL };

  if (gCurrentViewer != NULL)
    return; // TODO throw an exception here?
  gCurrentViewer = this;

  // Setup GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(width, height);
  glutCreateWindow((title != NULL) ? title : "Unnamed");

  // Setup GLUT callbacks.
  glutDisplayFunc(vgl::render);
  glutReshapeFunc(vgl::resize);
  glutKeyboardFunc(vgl::keyPressed);
  glutMouseFunc(vgl::mousePressed);
  glutMotionFunc(vgl::mouseDragged);

  if (_camera == NULL) {
    _camera = new vgl::ArcballCamera(
        vgl::Vec3f(0, 0, 5), vgl::Vec3f(0, 0, 0), vgl::Vec3f(0, 1, 0),
        -1, 1, -1, 1, 30, _width, _height);
  }

  if (_renderer != NULL)
    _renderer->setup();
}


Viewer::~Viewer()
{
  delete _renderer;
  delete _camera;
}


void Viewer::render()
{
  // Clear the viewport.
  glClearColor(0.2, 0.2, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up the camera.
  if (_camera != NULL) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    _camera->setupProjectionMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    _camera->setupModelViewMatrix();
  }

  // Render the scene.
  glMatrixMode(GL_MODELVIEW);
  if (_renderer != NULL)
    _renderer->render();
  else
    glutSolidTeapot(1.0);

  // Swap the buffers.
  glutSwapBuffers();
}


void Viewer::resize(int width, int height)
{
  if (!_fullscreen) {
    _windowWidth = width;
    _windowHeight = height;
  }
  _width = width;
  _height = height;

  glViewport(0, 0, _width, _height);
}


void Viewer::keyPressed(unsigned char key, int x, int y)
{
  _prevMouseX = _mouseX;
  _prevMouseY = _mouseY;
  _mouseX = x;
  _mouseY = y;
  _key = key;

  int action = actionForKeyPress(key, x, y);
  actionHandler(action);
}


void Viewer::mousePressed(int button, int state, int x, int y)
{
  _prevMouseX = _mouseX;
  _prevMouseY = _mouseY;
  _mouseX = x;
  _mouseY = y;
  _state = state;
  _button = button;

  int modifiers = glutGetModifiers();
  _shiftDown = (modifiers & GLUT_ACTIVE_SHIFT) != 0;
  _altDown = (modifiers & GLUT_ACTIVE_ALT) != 0;
  _ctrlDown = (modifiers & GLUT_ACTIVE_CTRL) != 0;

  int action = actionForMousePress(button, state, x, y);
  actionHandler(action);
}


void Viewer::mouseDragged(int x, int y)
{
  _prevMouseX = _mouseX;
  _prevMouseY = _mouseY;
  _mouseX = x;
  _mouseY = y;

  int action = actionForMouseDrag(x, y);
  actionHandler(action);
}


void Viewer::run()
{
  glutMainLoop();
}


int Viewer::actionForKeyPress(unsigned int key, int x, int y)
{
  switch (key) {
  case 27: // Esc key
    return ACTION_QUIT;
  case 'f':
    return ACTION_FULLSCREEN_TOGGLE;
  default:
    return ACTION_IGNORE;
  }
}


int Viewer::actionForMousePress(int button, int state, int x, int y)
{
  if (state != GLUT_DOWN)
    return ACTION_IGNORE;

  switch (button) {
  case 3: // Mouse wheel up
    return _shiftDown ? ACTION_ZOOM_CAMERA_IN : ACTION_DOLLY_CAMERA_IN;
  case 4: // Mouse wheel down
    return _shiftDown ? ACTION_ZOOM_CAMERA_OUT : ACTION_DOLLY_CAMERA_OUT;
  default:
    return ACTION_IGNORE;
  }
}


int Viewer::actionForMouseDrag(int x, int y)
{
  switch (_button) {
  case GLUT_LEFT_BUTTON:
    if (_shiftDown)
      return ACTION_PAN_CAMERA;
    else
      return ACTION_ROLL_CAMERA;
  case GLUT_MIDDLE_BUTTON:
    if (_shiftDown)
      return ACTION_DOLLY_CAMERA;
    else
      return ACTION_MOVE_CAMERA;
  case GLUT_RIGHT_BUTTON:
    return ACTION_ZOOM_CAMERA;
  default:
    return ACTION_IGNORE;
  }
}


void Viewer::actionHandler(int action)
{
  // If it's any camera action but we don't have a camera, abort now.
  if (_camera == NULL && action >= ACTION_RESET_CAMERA && action <= ACTION_ROLL_CAMERA)
    return;

  switch (action) {
  case ACTION_QUIT:
    exit(0);
    break;

  case ACTION_FULLSCREEN_ON:
    if (!_fullscreen) {
      _fullscreen = true;
      glutFullScreen();
    }
    break;

  case ACTION_FULLSCREEN_OFF:
    if (_fullscreen) {
      _fullscreen = false;
      glutReshapeWindow(_windowWidth, _windowHeight);
    }
    break;

  case ACTION_FULLSCREEN_TOGGLE:
    _fullscreen = !_fullscreen;
    if (_fullscreen)
      glutFullScreen();
    else
      glutReshapeWindow(_windowWidth, _windowHeight);
    break;

  case ACTION_PAN_CAMERA:       _camera->pan(_prevMouseX, _prevMouseY, _mouseX, _mouseY); break;
  case ACTION_ROLL_CAMERA:      _camera->roll(_prevMouseX, _prevMouseY, _mouseX, _mouseY); break;
  case ACTION_MOVE_CAMERA:      _camera->move(_prevMouseX, _prevMouseY, _mouseX, _mouseY); break;
  case ACTION_DOLLY_CAMERA:     _camera->move(_prevMouseX, _prevMouseY, _mouseX, _mouseY); break;
  case ACTION_DOLLY_CAMERA_IN:  _camera->dolly(0, 0, 0.5 * _width, 0); break;
  case ACTION_DOLLY_CAMERA_OUT: _camera->dolly(0, 0, -0.5 * _width, 0); break;
  case ACTION_ZOOM_CAMERA:      _camera->zoom(_prevMouseX, _prevMouseY, _mouseX, _mouseY); break;
  case ACTION_ZOOM_CAMERA_IN:   _camera->zoom(0, 0, 2 * _width, 0); break;
  case ACTION_ZOOM_CAMERA_OUT:  _camera->zoom(0, 0, -2 * _width, 0); break;

  case ACTION_IGNORE:
  default:
    break;
  }
  glutPostRedisplay();
}

} // namespace vgl

