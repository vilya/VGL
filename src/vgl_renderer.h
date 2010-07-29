#ifndef vgl_renderer_h
#define vgl_renderer_h

namespace vgl {

class Renderer {
public:
  virtual void setup() = 0;
  virtual void render() = 0;
};

} // namespace vgl

#endif // vgl_renderer_h

