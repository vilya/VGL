DBGFLAGS := -g
OPTFLAGS := -O3 -fopenmp
#OPTFLAGS := -O3 -msse3 -m3dnow -mfpmath=sse
#OPTFLAGS := -O3

CXX := g++
LD  := g++

ifeq ($(OSTYPE), linux-gnu)
CXXFLAGS  := -Wall -m64 -fPIC -shared
LDFLAGS   := -m64 -fopenmp -Wl,--rpath,\$$ORIGIN -fPIC -shared
INCLUDE   := 
LIBS      := -lglut -lpthread
DYLIB_PRE := lib
DYLIB_EXT := .so
else
CXXFLAGS  := -Wall -isysroot /Developer/SDKs/MacOSX10.6.sdk -arch x86_64 -shared
LDFLAGS   := -framework OpenGL -framework GLUT \
	-Wl,-syslibroot,/Developer/SDKs/MacOSX10.6.sdk -arch x86_64 -shared
INCLUDE   := 
LIBS      := 
DYLIB_PRE := lib
DYLIB_EXT := .dylib
endif

SRC := src
OBJ := build/obj
DIST := dist
BIN := bin
EXAMPLE_SRC := example
EXAMPLE_OBJ := build/example

LIBVGL_BIN  := $(DIST)/lib/$(DYLIB_PRE)vgl$(DYLIB_EXT)
LIBVGL_OBJS :=  \
                $(OBJ)/vgl_camera.o \
                $(OBJ)/vgl_image.o \
                $(OBJ)/vgl_math.o \
                $(OBJ)/vgl_viewer.o
LIBVGL_INCS :=  \
                $(DIST)/include/vgl.h \
                $(DIST)/include/vgl_camera.h \
                $(DIST)/include/vgl_image.h \
                $(DIST)/include/vgl_math.h \
                $(DIST)/include/vgl_renderer.h \
                $(DIST)/include/vgl_viewer.h

EXAMPLE_BIN   := $(BIN)/example
EXAMPLE_OBJS  := $(EXAMPLE_OBJ)/example.o


.PHONY: debug
debug:
	$(MAKE) CXXFLAGS="$(DBGFLAGS) $(CXXFLAGS)" all


.PHONY: release
release:
	$(MAKE) CXXFLAGS="$(OPTFLAGS) $(CXXFLAGS)" all


.PHONY: all
all: dirs build_libvgl build_example


.PHONY: dirs
dirs:
	@mkdir -p $(OBJ)
	@mkdir -p $(EXAMPLE_OBJ)
	@mkdir -p $(DIST)/include
	@mkdir -p $(DIST)/lib
	@mkdir -p $(BIN)


.PHONY: build_example
build_example: build_libvgl $(EXAMPLE_BIN)
	cp $(LIBVGL_BIN) $(BIN)


.PHONY: build_libvgl
build_libvgl: $(LIBVGL_BIN) $(LIBVGL_INCS)


$(LIBVGL_BIN): $(LIBVGL_OBJS)
	$(LD) $(LDFLAGS) -o  $@ $^ $(LIBS)


$(DIST)/include/%: $(SRC)/%
	cp $< $@


$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@


$(EXAMPLE_BIN): $(EXAMPLE_OBJS)
	$(LD) -m64 -fopenmp -Wl,--rpath,\$$ORIGIN -L$(DIST)/lib -o $@ $^ $(LIBS) -lvgl


$(EXAMPLE_OBJ)/%.o: $(EXAMPLE_SRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -I$(DIST)/include -c $< -o $@


.PHONY: clean
clean:
	rm -rf bin/* build/* dist/*

