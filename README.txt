vgl = Vil's Graphics Library! :-)

It's a framework to take the donkey work out of writing OpenGL apps, so that
it's easier to concentrate on the rendering and other interesting bits.

So far it includes:
- A basic windowing system based on GLUT. The aim is to make it easy to get a
  single window up where you can draw using OpenGL and respond to events.
- Support for loading (but not saving) a number of 2d image formats:
  - BMP
  - PNG
  - JPG
  - TGA
  Note that you're expected to have libpng and libjpeg already installed on
  your system somewhere.
- Support for a loading (but not saving) a number of 3d geometry formats:
  - OBJ
  - PLY
- A simple camera class (designed to be overridden if you want to support other
  types of camera).
- A *very* simple renderer abstraction.

