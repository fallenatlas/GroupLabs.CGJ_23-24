//Scnene Graph Class

#include "./mglScenegraph.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "./mglCamera.hpp"
#include "./mglShader.hpp"
#include "./mglMesh.hpp"

namespace mgl
{
	SceneGraph::SceneGraph(Camera* camera) {
		this->nodes = std::vector<SceneNode*>();
		this->camera = camera;
	}

	SceneGraph::~SceneGraph() {
		//delete root;
		delete camera;
	}

	void SceneGraph::setCamera(Camera* camera) {
		this->camera = camera;
	}

	Camera* SceneGraph::getCamera() {
		return camera;
	}

	void SceneGraph::addNode(SceneNode* node) {
		this->nodes.push_back(node);
	}

	std::vector <SceneNode*> SceneGraph::getNodes() {
		return this->nodes;
	}

	void SceneGraph::updateScene() {
		//update camera
		//root->update();
	}

	void SceneGraph::renderScene(GLint ModelMatrixId, glm::mat4 ModelMatrix) {
		//Problably needs to be changed
		for (auto node : nodes) {
			node->getShaderProgram()->bind();

			glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			drawNode(node);

			node->getShaderProgram()->unbind();
		}
		
	}

	void SceneGraph::drawNode(SceneNode* node) {
		if (node->getMesh()) {
			node->draw();
		}
	}

	///////////////////////////////////////////////////////////////////////// SceneNode
	SceneNode::SceneNode() {
		mesh = nullptr;
		shaderProgram = nullptr;
	}

	SceneNode::~SceneNode() {
		//TODO: delete mesh and shaderprogram

	}

	void SceneNode::setModelMatrix(glm::mat4 modelmatrix) {
		ModelMatrix = modelmatrix;
	}

	glm::mat4 SceneNode::getModelMatrix() {
		return ModelMatrix;
	}

	void SceneNode::setMesh(Mesh* mesh) {
		this->mesh = mesh;
	}

	Mesh* SceneNode::getMesh() {
		return mesh;
	}

	void SceneNode::setShaderProgram(ShaderProgram* shaderProgram) {
		this->shaderProgram = shaderProgram;
	}

	ShaderProgram* SceneNode::getShaderProgram() {
		return shaderProgram;
	}

	void SceneNode::draw() {
		if (mesh) {
			mesh->draw();
		}
	}
	
}