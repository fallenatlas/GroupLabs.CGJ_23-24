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

#include "./mglCamera.hpp"
#include "./mglShader.hpp"
#include "./mglMesh.hpp"

namespace mgl {

class SceneGraph;
class SceneNode;

////////////////////////////////////////////////////////////////////// SceneGraph

class SceneGraph {
private:
	Camera* camera;
	std::vector <SceneNode*> nodes;

public:
	SceneGraph(Camera* camera);
	virtual ~SceneGraph();

	void setCamera(Camera* camera);
	Camera* getCamera();

	void addNode(SceneNode* node);
	std::vector <SceneNode*> getNodes();

	void renderScene(GLint ModelMatrixId, glm::mat4 ModelMatrix);

	void drawNode(SceneNode* node);
};

////////////////////////////////////////////////////////////////////// SceneNode

class SceneNode {
private:
	glm::mat4 ModelMatrix;
	Mesh* mesh;
	ShaderProgram* shaderProgram;

public:
	SceneNode();
	virtual ~SceneNode();

	void setModelMatrix(glm::mat4 ModelMatrix);
	glm::mat4 getModelMatrix();

	void setMesh(Mesh* mesh);
	Mesh* getMesh();

	void setShaderProgram(ShaderProgram* shaderProgram);
	ShaderProgram* getShaderProgram();

	void draw();
};

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_SCENEGRAPH_HPP */
