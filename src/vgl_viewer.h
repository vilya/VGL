#ifndef vgl_viewer_h
#define vgl_viewer_h

namespace vgl {

//
// Forward declarations
//

class Camera;
class Renderer;


//
// Types
//

class Viewer {
public:
  // Actions.
  enum {
    ACTION_IGNORE,            // Do nothing.
    ACTION_QUIT,              // Exit the program.
    
    ACTION_FULLSCREEN_ON,     // Turn fullscreen mode on, if not already in it.
    ACTION_FULLSCREEN_OFF,    // Turn fullscreen mode off, if it's currently on.
    ACTION_FULLSCREEN_TOGGLE, // Toggle fullscreen mode off or on.

    ACTION_RESET_CAMERA,      // Reset the camera to it's "home" position.

    ACTION_ZOOM_CAMERA,       // Zoom the camera
    ACTION_ZOOM_CAMERA_IN,    // Zoom the camera in by some predetermined amount
    ACTION_ZOOM_CAMERA_OUT,   // Zoom the camera out by some predetermined amount
    ACTION_PAN_CAMERA,        // Turn the camera without moving it from it's current position.
    ACTION_MOVE_CAMERA,       // Move the camera left right, up or down without turning it.
    ACTION_DOLLY_CAMERA,      // Move the camera in or out without turning it.
    ACTION_DOLLY_CAMERA_IN,   // Move the camera in by some predetermined amount without turning it.
    ACTION_DOLLY_CAMERA_OUT,  // Move the camera out by some predetermined amount without turning it.
    ACTION_ROLL_CAMERA        // Rotate the camera around it's current focal point.
  };

public:
  Viewer(const char* title, int width, int height, Camera* camera, Renderer* renderer);
  virtual ~Viewer();

  virtual void render();
  virtual void resize(int width, int height);

  virtual void keyPressed(unsigned char key, int x, int y);
  virtual void mousePressed(int button, int state, int x, int y);
  virtual void mouseDragged(int x, int y);

  void run();

protected:
  // Override this to map a key press to an action code. The mapping can take
  // any of the current state into account if you like.
  virtual int actionForKeyPress(unsigned int key, int x, int y);

  // Override this to map a mouse press to an action code. The mapping can take
  // any of the current state into account if you like.
  virtual int actionForMousePress(int button, int state, int x, int y);

  // Override this to map a mouse drag to an action code. The mapping can take
  // any of the current state into account if you like.
  virtual int actionForMouseDrag(int x, int y);

  // Override this to make the viewer respond to different types of action. The
  // replacement function should call this if it wants to enable the default 
  // action handling behaviours.
  virtual void actionHandler(int action);

private:
  int _windowWidth, _windowHeight;
  int _width, _height;
  int _mouseX, _mouseY;
  int _prevMouseX, _prevMouseY;
  int _state, _key, _button;
  bool _shiftDown, _altDown, _ctrlDown;
  bool _fullscreen;

  Camera* _camera;
  Renderer* _renderer;
};


} // namespace vgl

#endif // vgl_viewer_h

