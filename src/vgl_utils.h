#ifndef vgl_utils_h
#define vgl_utils_h

#include <algorithm>

#define GL_GLEXT_PROTOTYPES 1
#ifdef linux
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

namespace vgl {

//
// TEMPLATE FUNCTIONS
//

template <typename Num>
Num clamp(Num k)
{
  return std::max(Num(0), std::min(Num(1), k));
}


template <typename Num>
Num sqr(Num k)
{
  return k * k;
}


//
// TYPES
//

enum StringAlignment {
  ALIGN_LEFT, ALIGN_RIGHT
};


//
// HELPER FUNCTIONS
//

// Resolve a filename relative to a base directory. Relative filenames get the
// base directory prepended; absolute filenames are unaffected.
//
// Note that the return value is a pointer to internal statically allocated
// storage. Don't free() or delete it! It also means that this function isn't
// thread-safe. But on the plus side, it doesn't leak memory either.
const char* resolveFilename(const char* baseDir, const char* filename);


//
// OPENGL HELPER FUNCTIONS
//

// Draw a text string at some position on screen. The font parameter must be
// one of GLUTs font ID constants (e.g. GLUT_BITMAP_8_BY_13). The x and y 
// parameters are the position relative to the current model and view
// transforms.
void drawBitmapString(float x, float y, void* font, char* str,
    StringAlignment alignment = ALIGN_LEFT);

// Load a shader from a file. The shaderType param specifies whether it's a
// vertex shader, fragment shader etc. The path param gives the file to load it
// from.
GLuint loadShader(GLenum shaderType, const char* path);

// Link a vertex shader and fragment shader together into a shader program.
GLuint linkShader(GLuint vertexShaderID, GLuint fragmentShaderID);

// Print out the contents of the shader info log. Useful for debugging when
// your shader won't compile. The obj param is the ID of the shader whose log
// you want to print.
void printShaderInfoLog(GLuint obj);

// Print out the contents of the program info load. Useful for debugging when
// your shader program won't link. The obj param is the ID of the program whose
// log you want to print.
void printProgramInfoLog(GLuint obj);

// Checks OpenGLs error state and prints an error message if there was a
// problem. You can optionally provide a message to print if everything was OK,
// too.
bool checkGLError(const char* errMsg, const char* okMsg = NULL);


} // namespace vgl

#endif // vgl_utils_h

