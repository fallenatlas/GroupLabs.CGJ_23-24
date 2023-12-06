//Scnene Graph Class

#include <glm/gtc/type_ptr.hpp>
#include "./mglScenegraph.hpp"

namespace mgl
{
	SceneGraph::SceneGraph(SceneNode* root, Camera* camera) {
		this->root = root;
		this->camera = camera;
	}

	SceneGraph::~SceneGraph() {
		delete root;
		delete camera;
	}

	void SceneGraph::setCamera(Camera* camera) {
		this->camera = camera;
	}

	Camera* SceneGraph::getCamera() {
		return camera;
	}

	void SceneGraph::setRoot(SceneNode* root) {
		this->root = root;
	}

	SceneNode* SceneGraph::getRoot() {
		return root;
	}

	void SceneGraph::updateScene() {
		//update camera
		root->update();
	}

	void SceneGraph::renderScene(GLint ModelMatrixId, glm::mat4 ModelMatrix) {
		//Problably needs to be changed
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		root->getShaderProgram()->bind();

		
		glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		drawNode(root);

		root->getShaderProgram()->unbind();
	}

	void SceneGraph::drawNode(SceneNode* node) {
		//Problably needs to be changed
		if(node->getMesh()) {
			node->getMesh()->draw();
		}

		for(auto child : node->getChildren()) {
			drawNode(child);
		}
	}

	///////////////////////////////////////////////////////////////////////// SceneNode
	SceneNode::SceneNode() {
		mesh = nullptr;
		parent = nullptr;
	
	}

	SceneNode::SceneNode(Mesh* mesh, glm::mat4 ModelMatrix) {
		this->mesh = mesh;
		setModelMatrix(ModelMatrix);
		parent = nullptr;

	}

	SceneNode::~SceneNode() {
		for (auto child : children) {
			delete child;
		}
	}

	void SceneNode::removeParent() {
		parent = nullptr;
	}

	void SceneNode::addParent(SceneNode* parent) {
		this->parent = parent;
	}

	void SceneNode::addChild(SceneNode* child) {
		children.push_back(child);
		child->addParent(this);
	}

	std::vector<SceneNode*> SceneNode::getChildren() {
		return children;
	}

	void SceneNode::removeChild(SceneNode* child) {
		for (auto it = children.begin(); it != children.end(); ++it) {
			if (*it == child) {
				children.erase(it);
				break;
			}
		}
	}

	void SceneNode::setModelMatrix(const glm::mat4& modelmatrix) {
		ModelMatrix = modelmatrix;
	}

	glm::mat4& SceneNode::getModelMatrix() {
		return ModelMatrix;
	}

	void SceneNode::setMesh(Mesh* mesh) {
		this->mesh = mesh;
	}

	Mesh* SceneNode::getMesh() {
		return mesh;
	}

	void SceneNode::setShaderProgram(ShaderProgram* shaderProgram) {
		this->shaderprogram = shaderProgram;
	}

	ShaderProgram* SceneNode::getShaderProgram() {
		return shaderprogram;
	}

	void SceneNode::draw() {
		if (mesh) {
			mesh->draw();
		}
	}

	void SceneNode::update() {
		if (parent) {
			ModelMatrix = parent->getModelMatrix() * ModelMatrix;
		}

		for (auto child : children) {
			child->update();
		}
	}
}