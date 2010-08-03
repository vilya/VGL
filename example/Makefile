DBGFLAGS := -g
OPTFLAGS := -O3

CXX := g++
CC  := gcc
LD  := g++

SRC := .
OBJ := ../build/example
DIST := ../dist
BIN := ../bin

ifeq ($(OSTYPE), linux-gnu)
DYLIB_PRE := lib
DYLIB_EXT := .so
LIBVGL_BIN  := $(DIST)/lib/$(DYLIB_PRE)vgl$(DYLIB_EXT)
CXXFLAGS  := -Wall -m64
LDFLAGS   := -m64 -Wl,--rpath,\$$ORIGIN
INCLUDE   := -I$(DIST)/include
LIBS      := -L$(DIST)/lib -lvgl
else
DYLIB_PRE := lib
DYLIB_EXT := .dylib
LIBVGL_BIN  := $(DIST)/lib/$(DYLIB_PRE)vgl$(DYLIB_EXT)
CXXFLAGS  := -Wall -isysroot /Developer/SDKs/MacOSX10.6.sdk -arch x86_64
LDFLAGS   := -framework OpenGL -framework GLUT \
	-Wl,-syslibroot,/Developer/SDKs/MacOSX10.6.sdk -arch x86_64 \
	-Wl,-rpath,@loader_path/
INCLUDE   := -I$(DIST)/include 
LIBS      := -L$(DIST)/lib -lvgl
endif

EXAMPLE_APPS  := \
	$(BIN)/basic \
	$(BIN)/example


.PRECIOUS: $(OBJ)/basic.o $(OBJ)/example.o


.PHONY: debug
debug:
	$(MAKE) CXXFLAGS="$(DBGFLAGS) $(CXXFLAGS)" all


.PHONY: release
release:
	$(MAKE) CXXFLAGS="$(OPTFLAGS) $(CXXFLAGS)" all


.PHONY: all
all: dirs $(EXAMPLE_APPS)


.PHONY: dirs
dirs:
	@mkdir -p $(OBJ)
	@mkdir -p $(BIN)


$(BIN)/%: $(OBJ)/%.o
	$(LD) $(LDFLAGS) -o  $@ $^ $(LIBS)


$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@


.PHONY: clean
clean:
	@echo "Run make clean from the library root instead please."

