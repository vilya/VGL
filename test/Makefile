DBGFLAGS := -g
OPTFLAGS := -O3

CXX := g++
CC  := gcc
LD  := g++

SRC := .
OBJ := ../build/test
DIST := ../dist
BIN := ../bin

ifeq ($(OSTYPE), linux-gnu)
DYLIB_PRE := lib
DYLIB_EXT := .so
LIBVGL    := $(DYLIB_PRE)vgl$(DYLIB_EXT)
LIBVGL_BIN  := $(DIST)/lib/$(LIBVGL)
CXXFLAGS  := -Wall -m64
LDFLAGS   := -m64 -Wl,--rpath,\$$ORIGIN
INCLUDE   := -I$(DIST)/include
LIBS      := -L$(DIST)/lib -lvgl -lcppunit
else
DYLIB_PRE := lib
DYLIB_EXT := .dylib
LIBVGL    := $(DYLIB_PRE)vgl$(DYLIB_EXT)
LIBVGL_BIN  := $(DIST)/lib/$(LIBVGL)
CXXFLAGS  := -Wall -isysroot /Developer/SDKs/MacOSX10.6.sdk -arch x86_64
LDFLAGS   := -framework OpenGL -framework GLUT \
	-Wl,-syslibroot,/Developer/SDKs/MacOSX10.6.sdk -arch x86_64 \
	-Wl,-rpath,@loader_path/
INCLUDE   := -I/opt/local/include -I$(DIST)/include
LIBS      := -L/opt/local/lib -L$(DIST)/lib -lvgl -lcppunit
endif


TEST_OBJS  := \
	$(OBJ)/test_quaternion.o

TEST_APPS  := $(TEST_OBJS:$(OBJ)/%.o=$(BIN)/%)


.PRECIOUS: $(TEST_OBJS)


.PHONY: debug
debug:
	$(MAKE) CXXFLAGS="$(DBGFLAGS) $(CXXFLAGS)" all


.PHONY: release
release:
	$(MAKE) CXXFLAGS="$(OPTFLAGS) $(CXXFLAGS)" all


.PHONY: all
all: dirs $(TEST_APPS)


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

