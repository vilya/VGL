#include "vgl.h"

//
// Classes
//

class DirectionalLight
{
public:
  DirectionalLight(const Eigen::Vector3f& pos, const Eigen::Vector3f& target,
                   const Eigen::Vector3f& up);

  void setupProjectionMatrix(int shadowMapWidth, int shadowMapHeight);
  void setupModelViewMatrix();
  void setupLight(float brightness);

  Eigen::Matrix4f getShadowMatrix(int shadowMapWidth, int shadowMapHeight);

  const Eigen::Vector3f& getPos() const;
  const Eigen::Vector3f& getTarget() const;
  const Eigen::Vector3f& getUp() const;

private:
  Eigen::Vector3f _pos, _target, _up;
  Eigen::Matrix4f _projMatrix, _modelViewMatrix;
};


class ShadowMapRenderer : public vgl::Renderer
{
public:
  ShadowMapRenderer(DirectionalLight* light, vgl::Camera* camera);

  virtual void setup();
  virtual void render();

protected:
  void saveShadowTexture();
  void setupShadowMap();
  void drawSceneFromLight();
  void drawSceneFromCamera(float brightness, int viewport[]);
  void drawScene();

private:
  int _shadowMapWidth;
  int _shadowMapHeight;
  GLuint _shadowTex;

  DirectionalLight* _light;
  vgl::Camera* _camera;

  GLuint _testTex;
};


class ShadowMapViewer : public vgl::Viewer
{
public:
  ShadowMapViewer(ShadowMapRenderer* shadowMapRenderer);

  virtual void render();
};


//
// DirectionalLight methods
//

DirectionalLight::DirectionalLight(const Eigen::Vector3f& pos,
                                   const Eigen::Vector3f& target,
                                   const Eigen::Vector3f& up) :
    _pos(pos),
    _target(target),
    _up(up)
{
}


void DirectionalLight::setupProjectionMatrix(int shadowMapWidth,
                                             int shadowMapHeight)
{
  float distance = (_target - _pos).norm();
  gluPerspective(30, float(shadowMapWidth) / float(shadowMapHeight),
      distance * 0.1, distance * 2.0);
}


void DirectionalLight::setupModelViewMatrix()
{
  gluLookAt(_pos.x(), _pos.y(), _pos.z(), _target.x(), _target.y(), _target.z(),
            _up.x(), _up.y(), _up.z());
}


void DirectionalLight::setupLight(float brightness)
{
  const Eigen::Vector3f kColor(brightness, brightness, brightness);
  const Eigen::Vector3f kBlack(0, 0, 0);

  glLightfv(GL_LIGHT1, GL_POSITION, _pos.data());
  glLightfv(GL_LIGHT1, GL_AMBIENT, kColor.data());
  glLightfv(GL_LIGHT1, GL_DIFFUSE, kColor.data());
  glLightfv(GL_LIGHT1, GL_SPECULAR, kBlack.data());
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);
}


Eigen::Matrix4f DirectionalLight::getShadowMatrix(int shadowMapWidth,
                                                int shadowMapHeight)
{
  const float kBias[] = {
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
  };

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadMatrixf(kBias);
  setupProjectionMatrix(shadowMapWidth, shadowMapHeight);
  setupModelViewMatrix();

  Eigen::Matrix4f shadowMatrix;
  glGetFloatv(GL_MODELVIEW_MATRIX, shadowMatrix.data());
  glPopMatrix();

  shadowMatrix.transpose();
  return shadowMatrix;
}


//
// ShadowMapRenderer functions
//

ShadowMapRenderer::ShadowMapRenderer(DirectionalLight* light, vgl::Camera* camera) :
    vgl::Renderer(),
    _shadowMapWidth(512),
    _shadowMapHeight(512),
    _shadowTex(0),
    _light(light),
    _camera(camera),
    _testTex(0)
{
}


void ShadowMapRenderer::setup()
{
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glCullFace(GL_BACK);
  glShadeModel(GL_SMOOTH);
  glDepthFunc(GL_LEQUAL);

  const Eigen::Vector3f kWhite(1, 1, 1);

  glClearColor(0, 0, 0, 0);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, kWhite.data());
  glMaterialf(GL_FRONT, GL_SHININESS, 16.0);

  // Create the shadowmap texture.
  glGenTextures(1, &_shadowTex);
  glBindTexture(GL_TEXTURE_2D, _shadowTex);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE,
               _shadowMapWidth, _shadowMapHeight, 0, GL_LUMINANCE, GL_FLOAT, NULL);
}


void ShadowMapRenderer::render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Store the current viewport settings.
  int viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  int width = viewport[2];
  int height = viewport[3];

  // Draw the scene from the light's point of view.
  drawSceneFromLight();

  // Save the rendered scene in the shadow map.
  saveShadowTexture();

  // Draw the scene from the cameras point of view with a dim light for the shadowed areas.
  drawSceneFromCamera(0.2f, viewport);

  // Setup the shadow map.
  setupShadowMap();

  // Draw the scene from the cameras point of view with a bright light for the unshadowed areas.
  drawSceneFromCamera(1.0f, viewport);

  // Cleanup
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glDisable(GL_TEXTURE_GEN_Q);
  glDisable(GL_LIGHTING);
  glDisable(GL_ALPHA_TEST);
}


void ShadowMapRenderer::saveShadowTexture()
{
  glBindTexture(GL_TEXTURE_2D, _shadowTex);
  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, _shadowMapWidth, _shadowMapHeight, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void ShadowMapRenderer::setupShadowMap()
{
  Eigen::Matrix4f shadowMatrix = _light->getShadowMatrix(_shadowMapWidth, _shadowMapHeight);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGenfv(GL_S, GL_EYE_PLANE, shadowMatrix.row(0).data());
  glEnable(GL_TEXTURE_GEN_S);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGenfv(GL_T, GL_EYE_PLANE, shadowMatrix.row(1).data());
  glEnable(GL_TEXTURE_GEN_T);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGenfv(GL_R, GL_EYE_PLANE, shadowMatrix.row(2).data());
  glEnable(GL_TEXTURE_GEN_R);
  glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGenfv(GL_Q, GL_EYE_PLANE, shadowMatrix.row(3).data());
  glEnable(GL_TEXTURE_GEN_Q);
  glBindTexture(GL_TEXTURE_2D, _shadowTex);
  glEnable(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
  glAlphaFunc(GL_GEQUAL, 0.99f);
  glEnable(GL_ALPHA_TEST);
}


void ShadowMapRenderer::drawSceneFromLight()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  _light->setupProjectionMatrix(_shadowMapWidth, _shadowMapHeight);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  _light->setupModelViewMatrix();
  glViewport(0, 0, _shadowMapWidth, _shadowMapHeight);
  glCullFace(GL_FRONT);
  glShadeModel(GL_FLAT);
  glColorMask(0, 0, 0, 0);
  drawScene();
}


void ShadowMapRenderer::drawSceneFromCamera(float brightness, int viewport[])
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  _camera->setupProjectionMatrix();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  _camera->setupModelViewMatrix();
  glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
  glCullFace(GL_BACK);
  glShadeModel(GL_SMOOTH);
  glColorMask(1, 1, 1, 1);
  _light->setupLight(brightness);
  drawScene();
}


void ShadowMapRenderer::drawScene()
{
  glFrontFace(GL_CW); // The triangle winding order on the teapot seems to be clockwise...
  glColor3f(1, 0, 0);
  glPushMatrix();
  glTranslatef(0, 0.5, 0);
  glutSolidTeapot(0.5);
  glPopMatrix();

  glFrontFace(GL_CCW);
  glColor3f(0, 0, 1);
  glPushMatrix();
  glScalef(1, 0.1, 1);
  glTranslatef(0, -0.5, 0);
  glutSolidCube(1.0);
  glPopMatrix();
}


//
// ShadowMapViewer functions
//

ShadowMapViewer::ShadowMapViewer(ShadowMapRenderer* renderer) :
    vgl::Viewer("Shadow map test", 1024, 768, renderer)
{
}


void ShadowMapViewer::render()
{
  _renderer->render();
  glutSwapBuffers();
}


//
// Functions
//

int main(int argc, char**argv)
{
  const int kWidth = 1024;
  const int kHeight = 768;

  vgl::ArcballCamera camera(Eigen::Vector3f(0, 0, 5),
                            Eigen::Vector3f(0, 0, 0),
                            Eigen::Vector3f(0, 1, 0),
                            -1, 1, -1, 1,
                            30, kWidth, kHeight);

  DirectionalLight light(Eigen::Vector3f(2, 5, 1),
                         Eigen::Vector3f(0, 0, 0),
                         Eigen::Vector3f(1, 0, 1));

  ShadowMapRenderer renderer(&light, &camera);
  vgl::Viewer viewer("Stencil buffer test", kWidth, kHeight, &renderer, &camera);
  viewer.run();
  return 0;
}
