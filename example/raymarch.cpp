#include "vgl.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <vector>
#include <sys/errno.h>


//
// Global variables
//

static size_t bboxTestCount = 0;
static size_t bboxRejectionCount = 0;
static size_t bboxRejectionDepth = 0;
static size_t bvhNodesTraversed = 0;


//
// Types
//

struct Grid {
  vgl::Vec3ui dim;
  vgl::Vec3f lowCorner, highCorner, cellSize;

  Grid() :
    dim(), lowCorner(), highCorner() {}

  Grid(unsigned int iWidth, unsigned int iHeight, unsigned int iDepth,
      vgl::Vec3f iLowCorner, vgl::Vec3f iHighCorner);
};


class Marcher {
public:
  Marcher(const vgl::Ray3f& r, const Grid& g);

  void moveNext();
  bool hasNext() const;

  const vgl::Vec3i& index() const { return _index; }
  const vgl::Vec3f& position() const { return _pos; }

private:
  bool initialHit();

private:
  vgl::Ray3f _ray;
  Grid _grid;

  vgl::Vec3f _pos;

  vgl::Vec3i _index;
  vgl::Vec3i _delta;

  vgl::Vec3f _timeDelta;
  vgl::Vec3f _nextPlane;
};


class RayMarchRenderer : public vgl::Renderer {
public:
  RayMarchRenderer(const Grid& g, const vgl::Ray3f& r) :
    _gridList(0), _grid(g), _ray(r) {}

  void toggleDrawGrid() { _showGrid = !_showGrid; }
  void toggleDrawRay() { _showRay = !_showRay; }
  void toggleDrawIntersections() { _showIntersections = !_showIntersections; }
  void toggleDrawIntersectedCells() { _showIntersectedCells = !_showIntersectedCells; }

  virtual void setup();
  virtual void render();

private:
  void renderGrid();
  void renderRay() const;
  void renderIntersections() const;
  void renderIntersectedCells() const;

  void renderIntersection(const vgl::Vec3f& pos, float size) const;
  void renderIntersectedCell(const vgl::Vec3i& index, float size) const;

private:
  int _gridList;
  Grid _grid;
  vgl::Ray3f _ray;

  bool _showGrid;
  bool _showRay;
  bool _showIntersections;
  bool _showIntersectedCells;
};


class RayMarchViewer : public vgl::Viewer {
public:
  // Extra UI actions.
  enum {
    ACTION_FIRST = 100,

    ACTION_TOGGLE_DRAW_GRID,
    ACTION_TOGGLE_DRAW_RAY,
    ACTION_TOGGLE_DRAW_INTERSECTIONS,
    ACTION_TOGGLE_DRAW_INTERSECTED_CELLS
  };

public:
  RayMarchViewer(RayMarchRenderer* renderer, vgl::Camera* camera = NULL);

protected:
  virtual int actionForKeyPress(unsigned int key, int x, int y);
  virtual void actionHandler(int action);
};


//
// Grid methods
//

Grid::Grid(unsigned int iWidth, unsigned int iHeight, unsigned int iDepth,
    vgl::Vec3f iLowCorner, vgl::Vec3f iHighCorner) :
  dim(iWidth, iHeight, iDepth), lowCorner(iLowCorner), highCorner(iHighCorner)
{
  cellSize = (highCorner - lowCorner) / vgl::Vec3f(dim[0], dim[1], dim[2]);
}


//
// Marcher methods
//

Marcher::Marcher(const vgl::Ray3f& r, const Grid& g) :
  _ray(r), _grid(g), _pos(r.o), _index(), _delta(), _timeDelta(), _nextPlane()
{
  bool inside = true;
  for (int i = 0; i < 3; ++i) {
    _delta[i] = (_ray.d[i] > 0) ? 1 : (_ray.d[i] < 0) ? -1 : 0;
    _index[i] = (int)floorf((_ray.o[i] - _grid.lowCorner[i]) / _grid.cellSize[i]);
    if (_index[i] < 0 || (unsigned int)_index[i] >= _grid.dim[i]) {
      inside = false;
      if (vgl::sign(_delta[i]) == vgl::sign(_index[i]))
        return;
    }
  }

  if (!inside) {
    if (!initialHit())
      return;
  }

  for (int i = 0; i < 3; ++i) {
    float gridCoord = (_pos[i] - _grid.lowCorner[i]) / _grid.cellSize[i];
    if (_ray.d[i] < 0)
      gridCoord = floorf(gridCoord);
    else
      gridCoord = ceilf(gridCoord);

    _nextPlane[i] = _grid.lowCorner[i] + _grid.cellSize[i] * gridCoord;
    _timeDelta[i] = (_nextPlane[i] - _pos[i]) / _ray.d[i];
  }
}


void Marcher::moveNext()
{
  if (!hasNext())
    return;

  unsigned int axis = minIndex(_timeDelta);
  
  _index[axis] += _delta[axis];
  if (_index[axis] < 0 || (unsigned int)_index[axis] >= _grid.dim[axis])
    return;

  float timeDelta = _timeDelta[axis];
  for (int i = 0; i < 3; ++i)
    _timeDelta[i] -= timeDelta;

  _nextPlane[axis] += (_grid.cellSize[axis] * _delta[axis]);
  _timeDelta[axis] = (_grid.cellSize[axis] * _delta[axis]) / _ray.d[axis];
  _pos = _pos + _ray.d * timeDelta;
}


/*
void Marcher::moveNext()
{
  if (!hasNext())
    return;

  float minDt = 1e20;
  int minIndex = -1;

  for (int i = 0; i < 3; ++i) {
    float gridCoord = (_pos[i] - _grid.lowCorner[i]) / _grid.cellSize[i];
    if (_ray.d[i] < 0)
      gridCoord = floorf(gridCoord);
    else
      gridCoord = ceilf(gridCoord);

    float nextPlane = _grid.lowCorner[i] + _grid.cellSize[i] * gridCoord;

    float dt = (nextPlane - _pos[i]) / _ray.d[i];
    if (dt > 0 && dt < minDt) {
      minDt = dt;
      minIndex = i;
    }
  }

  if (minIndex >= 0) {
    _index[minIndex] += _delta[minIndex];
    _pos = _pos + _ray.d * minDt;
  }
  else {
    _index[0] = _index[1] = _index[2] = -1;
  }
}
*/


bool Marcher::hasNext() const
{
  for (int i = 0; i < 3; ++i) {
    if (_index[i] < 0 || (unsigned int)_index[i] >= _grid.dim[i])
      return false;
  }
  return true;
}


bool Marcher::initialHit()
{
  float tNear = 1e-10;
  float tFar = 1e20;

  for (int i = 0; i < 3; ++i) {
    if (fabsf(_ray.d[i]) < 1e-6) {
      if (_ray.o[i] < _grid.lowCorner[i] || _ray.o[i] > _grid.highCorner[i]) {
        _index[0] = _index[1] = _index[2] = -1;
        return false;
      }
    } else {
      float t1 = (_grid.lowCorner[i] - _ray.o[i]) / _ray.d[i];
      float t2 = (_grid.highCorner[i] - _ray.o[i]) / _ray.d[i];
      if (t1 > t2)
        std::swap(t1, t2);
      tNear = fmaxf(tNear, t1);
      tFar = fminf(tFar, t2);
      if (tNear > tFar) {
        _index[0] = _index[1] = _index[2] = -1;
        return false;
      }
    }
  }

  _pos = _ray.o + tNear * _ray.d;
  for (int i = 0; i < 3; ++i)
    _index[i] = floorf( (_pos[i] - _grid.lowCorner[i]) / _grid.cellSize[i] );

  return true;
}


//
// RayMarchRenderer methods
//

void RayMarchRenderer::setup()
{
    _gridList = glGenLists(1);

    vgl::Vec3f size = (_grid.highCorner - _grid.lowCorner)
                    / vgl::Vec3f(_grid.dim[0], _grid.dim[1], _grid.dim[2]);

    glNewList(_gridList, GL_COMPILE);
    glColor4f(0.6, 0.6, 0.6, 0.5);
    glBegin(GL_LINES);
    // Horizontal lines
    for (unsigned int j = 0; j <= _grid.dim.y; ++j) {
      float y = _grid.lowCorner.y + j * size.y;
      float x1 = _grid.lowCorner.x;
      float x2 = _grid.highCorner.x;
      for (unsigned int k = 0; k <= _grid.dim.z; ++k) {
        float z = _grid.lowCorner.z + k * size.z;
        glVertex3f(x1, y, z);
        glVertex3f(x2, y, z);
      }
    }
    // Vertical lines
    for (unsigned int i = 0; i <= _grid.dim.x; ++i) {
      float x = _grid.lowCorner.x + i * size.x;
      float y1 = _grid.lowCorner.y;
      float y2 = _grid.highCorner.y;
      for (unsigned int k = 0; k <= _grid.dim.z; ++k) {
        float z = _grid.lowCorner.z + k * size.z;
        glVertex3f(x, y1, z);
        glVertex3f(x, y2, z);
      }
    }
    // Pokey-outey lines
    for (unsigned int i = 0; i <= _grid.dim.x; ++i) {
      float x = _grid.lowCorner.x + i * size.x;
      float z1 = _grid.lowCorner.z;
      float z2 = _grid.highCorner.z;
      for (unsigned int j = 0; j <= _grid.dim.y; ++j) {
        float y = _grid.lowCorner.y + j * size.y;
        glVertex3f(x, y, z1);
        glVertex3f(x, y, z2);
      }
    }
    glEnd();
    glEndList();
}


void RayMarchRenderer::render()
{
  if (_showGrid)
    renderGrid();
  if (_showRay)
    renderRay();
  if (_showIntersections)
    renderIntersections();
  if (_showIntersectedCells)
    renderIntersectedCells();
}


void RayMarchRenderer::renderGrid()
{
  glCallList(_gridList);
}


void RayMarchRenderer::renderRay() const
{
  glColor3f(1, 0, 0);
  glBegin(GL_LINES);
  glVertex3fv(_ray.o.data);
  glVertex3fv( (_ray.o + _ray.d).data );
  glEnd();
}


void RayMarchRenderer::renderIntersections() const
{
  glColor4f(1, 0.7, 0.7, 0.5);
  Marcher m(_ray, _grid);
  while (m.hasNext()) {
    renderIntersection(m.position(), 0.1);
    m.moveNext();
  }
}


void RayMarchRenderer::renderIntersectedCells() const
{
  glColor4f(0.3, 0.3, 0.7, 0.2);
  Marcher m(_ray, _grid);
  while (m.hasNext()) {
    renderIntersectedCell(m.index(), 0.99);
    m.moveNext();
  }
}


void RayMarchRenderer::renderIntersection(const vgl::Vec3f& pos, float size) const
{
  glBegin(GL_LINES);
  glVertex3fv( (pos - vgl::Vec3f(size, 0, 0)).data );
  glVertex3fv( (pos + vgl::Vec3f(size, 0, 0)).data );
  glVertex3fv( (pos - vgl::Vec3f(0, size, 0)).data );
  glVertex3fv( (pos + vgl::Vec3f(0, size, 0)).data );
  glVertex3fv( (pos - vgl::Vec3f(0, 0, size)).data );
  glVertex3fv( (pos + vgl::Vec3f(0, 0, size)).data );
  glEnd();
}


void RayMarchRenderer::renderIntersectedCell(const vgl::Vec3i& index, float size) const
{
  const float offset = (1.0 - size) / 2.0;

  vgl::Vec3f low;
  for (int i = 0; i < 3; ++i)
    low[i] = _grid.lowCorner[i] + (index[i] + offset) * _grid.cellSize[i];

  glPushMatrix();
  glTranslatef(low.x, low.y, low.z);
  glScalef(_grid.cellSize.x, _grid.cellSize.y, _grid.cellSize.z);
  glTranslatef(size / 2.0, size / 2.0, size / 2.0);
  glutSolidCube(size);
  glPopMatrix();
}


//
// GUI
//

RayMarchViewer::RayMarchViewer(RayMarchRenderer* renderer, vgl::Camera* camera) :
  vgl::Viewer("RayMarch", 1024, 768, renderer, camera)
{}


int RayMarchViewer::actionForKeyPress(unsigned int key, int x, int y)
{
  switch (key) {
    case 'g': return ACTION_TOGGLE_DRAW_GRID;
    case 'r': return ACTION_TOGGLE_DRAW_RAY;
    case 'i': return ACTION_TOGGLE_DRAW_INTERSECTIONS;
    case 'c': return ACTION_TOGGLE_DRAW_INTERSECTED_CELLS;
    default: return vgl::Viewer::actionForKeyPress(key, x, y);
  }
}


void RayMarchViewer::actionHandler(int action)
{
  RayMarchRenderer* renderer = dynamic_cast<RayMarchRenderer*>(_renderer);
  switch (action) {
    case ACTION_TOGGLE_DRAW_GRID:
      renderer->toggleDrawGrid();
      break;
    case ACTION_TOGGLE_DRAW_RAY:
      renderer->toggleDrawRay();
      break;
    case ACTION_TOGGLE_DRAW_INTERSECTIONS:
      renderer->toggleDrawIntersections();
      break;
    case ACTION_TOGGLE_DRAW_INTERSECTED_CELLS:
      renderer->toggleDrawIntersectedCells();
      break;
    default:
      vgl::Viewer::actionHandler(action);
      break;
  }
  glutPostRedisplay();
}


//
// MAIN
//

void usage(char* progname)
{
  fprintf(stderr,
"Usage: %s\n"
"    [-w|--width NUM] [-h|--height NUM] [-x NUM] [-y NUM] [-z NUM]\n"
"    [-s|--size NUM] [--help]\n",
          progname);
}


int main(int argc, char** argv)
{
  size_t width = 800;
  size_t height = 600;

  unsigned int x = 8;
  unsigned int y = 8;
  unsigned int z = 8;

  int arg = 1;
  while (arg < argc) {
    if (strcmp(argv[arg], "-w") == 0 || strcmp(argv[arg], "--width") == 0)
      width = atoi(argv[++arg]);
    else if (strcmp(argv[arg], "-h") == 0 || strcmp(argv[arg], "--height") == 0)
      height = atoi(argv[++arg]);
    else if (strcmp(argv[arg], "-x") == 0)
      x = atoi(argv[++arg]);
    else if (strcmp(argv[arg], "-y") == 0)
      y = atoi(argv[++arg]);
    else if (strcmp(argv[arg], "-z") == 0)
      z = atoi(argv[++arg]);
    else if (strcmp(argv[arg], "-s") == 0 || strcmp(argv[arg], "--size") == 0)
      x = y = z = atoi(argv[++arg]);
    else if (strcmp(argv[arg], "--help") == 0) {
      usage(argv[0]);
      return 0;
    }
    else {
      usage(argv[0]);
      return -1;
    }
    ++arg;
  }

  Grid g(x, y, z, vgl::Vec3f(-2, -2, -2), vgl::Vec3f(2, 2, 2));
  vgl::Ray3f r(vgl::Vec3f(-3, -3, 3), vgl::Vec3f(4, 5, -6));
  RayMarchRenderer renderer(g, r);
  RayMarchViewer gui(&renderer);
  gui.run();
  return 0;
}

