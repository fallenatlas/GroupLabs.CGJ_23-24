////////////////////////////////////////////////////////////////////////////////
//
// Scene Management Class
//
// Copyright (c)2022-23 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_SCENEGRAPH_HPP
#define MGL_SCENEGRAPH_HPP

#include <glm/glm.hpp>
#include <vector>
#include "../mgl/mgl.hpp"

namespace mgl {


class SceneGraph;
class SceneNode;
class IDrawable;

////////////////////////////////////////////////////////////////////// SceneGraph
class SceneGraph {
 private:
  Camera *camera;
  SceneNode *root;

 public:
  SceneGraph(SceneNode* root, Camera* camera);
  virtual ~SceneGraph();

  void setCamera(Camera *camera);
  Camera *getCamera();

  void setRoot(SceneNode *root);
  SceneNode *getRoot();

  void updateScene();
  void renderScene(GLint ModelMatrixId, glm::mat4 ModelMatrix);

  void drawNode(SceneNode *node);
};

/////////////////////////////////////////////////////////////////////// SceneNode
class SceneNode : public mgl::IDrawable {
 private:
  SceneNode* parent;
  std::vector<SceneNode *> children;
  glm::mat4 ModelMatrix;
  Mesh* mesh;
  ShaderProgram *shaderprogram;

  

 public:
  SceneNode();
  SceneNode(Mesh* mesh, glm::mat4 ModelMatrix);
  virtual ~SceneNode();

  void addParent(SceneNode* parent);
  void removeParent();

  void addChild(SceneNode *child);
  std::vector<SceneNode*> getChildren();
  void removeChild(SceneNode *child);

  void setModelMatrix(const glm::mat4 &modelmatrix);
  glm::mat4& getModelMatrix();

  void setMesh(Mesh *mesh);
  Mesh* getMesh();

  void setShaderProgram(ShaderProgram *shaderprogram);
  ShaderProgram* getShaderProgram();

  void draw(void);
  void update(void);
};


////////////////////////////////////////////////////////////////////// IDrawable

class IDrawable {
 public:
  virtual void draw(void) = 0;
  virtual void update(void) = 0;
};

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_SCENEGRAPH_HPP */
