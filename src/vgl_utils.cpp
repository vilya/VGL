#include "vgl_utils.h"

#include <cstdio>

namespace vgl {

//
// FUNCTIONS
//

void drawBitmapString(float x, float y, void* font, char* str, StringAlignment alignment)
{
  float xPos = x;
  if (alignment == ALIGN_LEFT) {
    for (char* ch = str; *ch != '\0'; ++ch) {
      glRasterPos2f(xPos, y);
      switch (*ch) {
        case '\n':
          xPos = x;
          y -= 15;
          break;
        default:
          glutBitmapCharacter(font, *ch);
          xPos += glutBitmapWidth(font, *ch);
          break;
      }
    }
  }
  else {
    while (*str != '\0') {
      char* ch = str;
      for (ch = str; *ch != '\0' && *ch != '\n'; ++ch)
        xPos -= glutBitmapWidth(font, *ch);
      for (; str < ch; ++str) {
        glRasterPos2f(xPos, y);
        glutBitmapCharacter(font, *str);
        xPos += glutBitmapWidth(font, *str);
      }
      if (*str == '\n') {
        ++str;
        xPos = x;
        y -= 15;
      }
    }
  }
}


GLuint loadShader(GLenum shaderType, const char* path)
{
  GLuint shaderID;
  char* text = NULL;

  FILE* shaderFile = fopen(path, "r");
  if (shaderFile == NULL) {
    fprintf(stderr, "Unable to load shader: couldn't open file %s\n", path);
    return 0;
  }

  // Get the length of the file.
  fseek(shaderFile, 0, SEEK_END);
  size_t length = ftell(shaderFile);
  fseek(shaderFile, 0, SEEK_SET);

  text = new char[length + 1];
  fread(text, sizeof(const char), length, shaderFile);
  text[length] = '\0';

  fclose(shaderFile);

  shaderID = glCreateShader(shaderType);
  glShaderSource(shaderID, 1, (const GLchar**)&text, NULL);
  glCompileShader(shaderID);

  GLint status;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    fprintf(stderr, "Shader %s failed to compile:\n", path);
    printShaderInfoLog(shaderID);
    shaderID = 0;
  }

  delete[] text;
  return shaderID;
}


GLuint linkShader(GLuint vertexShaderID, GLuint fragmentShaderID)
{
  GLuint programID = glCreateProgram();
  glAttachShader(programID, vertexShaderID);
  glAttachShader(programID, fragmentShaderID);
  glLinkProgram(programID);

  GLint status;
  glGetProgramiv(programID, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    fprintf(stderr, "Shader program failed to link.\n");
    printProgramInfoLog(programID);
    programID = 0;
  }

  return programID;
}


void printShaderInfoLog(GLuint obj)
{
  int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 0) {
	  infoLog = new char[infologLength];
	  glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		fprintf(stderr, "%s\n", infoLog);
    delete[] infoLog;
	}
}

void printProgramInfoLog(GLuint obj)
{
  int infologLength = 0;
  int charsWritten  = 0;
  char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
  if (infologLength > 0) {
    infoLog = new char[infologLength];
    glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		fprintf(stderr, "%s\n", infoLog);
    delete[] infoLog;
  }
}


bool checkGLError(const char *errMsg, const char *okMsg)
{
  GLenum err = glGetError();
  if (err != GL_NO_ERROR)
    fprintf(stderr, "%s: %s (%d)\n", errMsg, gluErrorString(err), err);
  else if (okMsg != NULL)
    fprintf(stderr, "%s\n", okMsg);
  return (err != GL_NO_ERROR);
}

} // namespace vgl

