VGL = Vil's Graphics Library! :-)

It's a library to take the donkey work out of writing OpenGL apps, so that it's
easier to concentrate on the rendering and other interesting bits.

The goal is to be agnostic about the choice of scene data structures, rendering
algorithms and user interaction methods while providing useful default
implementations for each of these.

A corollary to this is that it should be possible to override any behaviour
from the library. There's always going to be *something* that any given
program will need to do differently...

So far it includes:
- A very basic windowing system based on GLUT.
  - Single window, set up for drawing into with OpenGL.
  - Customisable (by overriding) mapping of input events to GUI actions.
  - Default behaviour which maps input events to camera controls.
- The usual 3D math classes.
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
- An example program which draws a lit teapot.
- An example program which loads and displays a model file.


License
=======
This code is licensed under the GPL v3. A full copy of the license terms can be
found in the accompanying LICENSE.txt file, or on the Free Software
Foundation's website at http://www.gnu.org/licenses/gpl.html


Pre-requisites
==============
- Linux or Mac OS X (tested on Ubuntu 9.10 and OS X 10.6.4)
- A graphics card which supports OpenGL 2.1 or higher.
- The following libraries installed on your system:
  - GLUT
  - libpng
  - libjpeg6b
  - libtiff
  - cppunit   (for the unit tests)


Getting the code
================

  git clone http://github.com/vilya


Compiling it
============

  make

...should do the trick. You may need to adjust the include paths and library
paths if you've got your libraries installed in custom locations.

After a successful make, the library and headers will be inside the dist
directory; the example programs (and a second copy of the library) will be
inside the bin directory along with the test programs.


Running the examples
====================
If you're in the project root folder, you should be able to run the example
programs by typing (for example):

  ./bin/example


Reporting bugs
==============
If you find any bugs, please add them to the project tracker on github:

  http://github.com/vilya/VGL/issues

