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
- The usual templated 3D math classes:
  - Vec2, Vec3 and Vec4
  - Matrix3 and Matrix4
  - Quaternion
- Support for loading (but not saving) a number of 2d image formats:
  - BMP
  - PNG
  - JPG
  - TGA
  - TIF
  Note that you're expected to have libpng, libjpeg and libtiff already
  installed on your system somewhere.
- Support for a loading (but not saving) a number of 3d geometry formats:
  - OBJ
  - PLY
- Some camera classes:
  - An arcball-style perspective camera (the default).
  - An orthorgrahic camera.
  - A base class, intended for subclassing.
- A *very* simple renderer abstraction.
- Helper functions for dealing with OpenGL shaders.
- Some example programs which demonstrate various aspects of the library.
- A build system using CMake


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
  - cppunit   (optional; only needed for the unit tests)


Getting the code
================

  git clone http://github.com/vilya


Building it
===========

VGL uses CMake, so you have the option of generating Makefiles or project files
for various IDEs.

Building with make
------------------
I recommend doing an out-of-source build. If you're inside the top-level VGL
directory, this should do the trick:

  mkdir build
  cd build
  cmake ..
  make

If you want to see the command line it's passing to the compiler, just write:

  make VERBOSE=1

The build is set up to try and autodetect the location of the libraries it
needs; if you have them installed in non-standard locations, you'll need to
tell CMake where to find them:

  cmake -DCMAKE_LIBRARY_PATH=/path/to/your/library/dir ..

The generated makefile includes targets for running tests, installing the
library and so on:

  make test     Run all the tests
  make install  Install the library
  make help     List out all the targets provided by the makefile.

Building with XCode
-------------------
On OS X you can use CMake to generate an XCode project for VGL. If you're
inside the top-level VGL directory, do this:

  mkdir project
  cd project
  cmake -g Xcode ..

This will create VGL.xcodeproj inside the current directory, which you can open
up directly with XCode.


Running the examples
====================
If you're in the project root folder, you should be able to run the example
programs by typing (for example):

  ./build/arcball


Reporting bugs
==============
If you find any bugs, please add them to the project tracker on github:

  http://github.com/vilya/VGL/issues

