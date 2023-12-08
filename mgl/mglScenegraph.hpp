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

#include "./mglOrbitCamera.hpp"
#include "./mglShader.hpp"
#include "./mglMesh.hpp"

namespace mgl {

class SceneGraph;
class SceneNode;

////////////////////////////////////////////////////////////////////// SceneGraph

class SceneGraph {
private:
	OrbitCamera* camera;
	std::vector <SceneNode*> nodes;

public:
	SceneGraph();
	virtual ~SceneGraph();

	void setCamera(OrbitCamera* camera);
	OrbitCamera* getCamera();

	void addNode(SceneNode* node);
	std::vector <SceneNode*> getNodes();

	void renderScene();

	void drawNode(SceneNode* node);
};

////////////////////////////////////////////////////////////////////// SceneNode

class SceneNode {
private:
	glm::mat4 ModelMatrix;
	Mesh* mesh;
	ShaderProgram* shaderProgram;
	GLint ModelMatrixId;
	GLint ColorId;
	GLint NormalMatrixId;
	glm::mat4 NormalMatrix;
	glm::vec4 Color;
	// parent
	// vector childs

public:
	SceneNode(GLint ModelMatrixId, GLint normalMatrixId, GLint colorId);
	virtual ~SceneNode();

	void setModelMatrix(glm::mat4 ModelMatrix);
	glm::mat4 getModelMatrix();

	void setNormalMatrix(glm::mat4 NormalMatrix);
	glm::mat4 getNormalMatrix();

	void setMesh(Mesh* mesh);
	Mesh* getMesh();

	void setColor(glm::vec4 color);
	glm::vec4 getColor();

	void setShaderProgram(ShaderProgram* shaderProgram);
	ShaderProgram* getShaderProgram();

	void draw();
};

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_SCENEGRAPH_HPP */
