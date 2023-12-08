//Scnene Graph Class

#include "./mglScenegraph.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "./mglShader.hpp"
#include "./mglMesh.hpp"

namespace mgl
{
	SceneGraph::SceneGraph() {
		this->nodes = std::vector<SceneNode*>();
	}

	SceneGraph::~SceneGraph() {
		/*
		for (auto node : nodes) {
			delete node;
		}
		delete camera;
		*/
	}

	void SceneGraph::setCamera(OrbitCamera* camera) {
		this->camera = camera;
	}

	OrbitCamera* SceneGraph::getCamera() {
		return camera;
	}

	void SceneGraph::addNode(SceneNode* node) {
		this->nodes.push_back(node);
	}

	std::vector <SceneNode*> SceneGraph::getNodes() {
		return this->nodes;
	}


	void SceneGraph::renderScene() {
		//Problably needs to be changed
		for (auto node : nodes) {
			//node->getShaderProgram()->bind();

			//glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(node->getModelMatrix()));

			node->draw();

			//node->getShaderProgram()->unbind();
		}
		
	}

	void SceneGraph::drawNode(SceneNode* node) {
		if (node->getMesh()) {
			node->draw();
		}
	}

	///////////////////////////////////////////////////////////////////////// SceneNode
	SceneNode::SceneNode(GLint modelMatrixId, GLint normalMatrixId, GLint colorId) {
		ModelMatrix = glm::mat4(1.0f);
		ModelMatrixId = modelMatrixId;
		NormalMatrixId = normalMatrixId;
		ColorId = colorId;
		mesh = nullptr;
		shaderProgram = nullptr;
	}

	SceneNode::~SceneNode() {
		/*
		delete mesh;
		delete shaderProgram;
		*/
	}

	void SceneNode::setModelMatrix(glm::mat4 modelmatrix) {
		ModelMatrix = modelmatrix;
	}

	glm::mat4 SceneNode::getModelMatrix() {
		return ModelMatrix;
	}

	void SceneNode::setNormalMatrix(glm::mat4 normalMatrix) {
		NormalMatrix = normalMatrix;
	}

	glm::mat4 SceneNode::getNormalMatrix() {
		return NormalMatrix;
	}

	void SceneNode::setMesh(Mesh* mesh) {
		this->mesh = mesh;
	}

	Mesh* SceneNode::getMesh() {
		return mesh;
	}

	void SceneNode::setColor(glm::vec4 color) {
		Color = color;
	}

	glm::vec4 SceneNode::getColor() {
		return Color;
	}

	void SceneNode::setShaderProgram(ShaderProgram* shaderProgram) {
		this->shaderProgram = shaderProgram;
	}

	ShaderProgram* SceneNode::getShaderProgram() {
		return shaderProgram;
	}

	void SceneNode::draw() {
		if (mesh) {
			shaderProgram->bind();
			glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
			glUniformMatrix4fv(NormalMatrixId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
			glUniform4fv(ColorId, 1, glm::value_ptr(Color));
			mesh->draw();
			shaderProgram->unbind();
		}

		// draw children
	}
	
}