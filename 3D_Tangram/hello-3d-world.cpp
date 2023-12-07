////////////////////////////////////////////////////////////////////////////////
//
// Adding a camera and moving to the 3rd dimension.
// A "Hello 3D World" of Modern OpenGL.
//
// Copyright (c) 2013-23 by Carlos Martinho
//
// INTRODUCES:
// VIEW PIPELINE, UNIFORM BUFFER OBJECTS, mglCamera.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "../mgl/mgl.hpp"

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
 public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  //void windowCloseCallback(GLFWwindow *win) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;
  void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) override;
  void cursorCallback(GLFWwindow* win, double xpos, double ypos) override;
  void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) override;
  void scrollCallback(GLFWwindow* win, double xoffset, double yoffset) override;

 private:
  const GLuint POSITION = 0, COLOR = 1, UBO_BP = 0;
  GLuint VaoId;

  mgl::ShaderProgram *Shaders = nullptr;
  mgl::OrbitCamera* OrbitCameras[2] = { nullptr };
  int activeIndex = 0;
  GLint ModelMatrixId;
  mgl::Mesh* Mesh = nullptr;

  double previousMousePositionX = 0;
  double previousMousePositionY = 0;

  void createMeshes();
  void createShaderPrograms();
  void createCameras();
  void drawScene(double elapsed);
  void switchCamera();
};

void MyApp::cursorCallback(GLFWwindow* win, double xpos, double ypos) {
    if (mgl::KeyState::getInstance().isMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {

        double xMoved = xpos - previousMousePositionX;
        double yMoved = ypos - previousMousePositionY;
        
        double anglex = xMoved * 20;
        double angley = yMoved * 20;
        float yawAngle = glm::radians(anglex);
        float pitchAngle = glm::radians(angley);

        OrbitCameras[activeIndex]->addYaw(yawAngle);
        OrbitCameras[activeIndex]->addPitch(pitchAngle);
    }
    previousMousePositionX = xpos;
    previousMousePositionY = ypos;
}

void MyApp::mouseButtonCallback(GLFWwindow* win, int button, int action,
    int mods) {
    mgl::KeyState::getInstance().updateMouseButtonState(button, action);
}

void MyApp::scrollCallback(GLFWwindow* win, double xoffset, double yoffset) {
    OrbitCameras[activeIndex]->addZoom(yoffset);
}

void MyApp::keyCallback(GLFWwindow* win, int key, int scancode, int action,
    int mods) {
    mgl::KeyState::getInstance().updateKeyState(key, action);

    if (mgl::KeyState::getInstance().isKeyPressed(GLFW_KEY_C)) {
        switchCamera();
    }
    else if (mgl::KeyState::getInstance().isKeyPressed(GLFW_KEY_P)) {
        OrbitCameras[activeIndex]->switchProjection();
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
        windowCloseCallback(win);
    }
}

void MyApp::switchCamera() {
    OrbitCameras[activeIndex]->setActive(false);
    activeIndex = activeIndex == 0 ? 1 : 0;
    OrbitCameras[activeIndex]->setActive(true);
}


///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes() {
    std::string mesh_dir = "../04-assets/models/";
    // std::string mesh_file = "cube-v.obj";
    // std::string mesh_file = "cube-vn.obj";
    // std::string mesh_file = "cube-vtn.obj";
    // std::string mesh_file = "cube-vtn-2.obj";
    // std::string mesh_file = "torus-vtn-flat.obj";
    // std::string mesh_file = "torus-vtn-smooth.obj";
    // std::string mesh_file = "suzanne-vtn-flat.obj";
    // std::string mesh_file = "suzanne-vtn-smooth.obj";
    // std::string mesh_file = "teapot-vn-flat.obj";
    // std::string mesh_file = "teapot-vn-smooth.obj";
    // std::string mesh_file = "bunny-vn-flat.obj";
    // std::string mesh_file = "bunny-vn-smooth.obj";
    std::string mesh_file = "monkey-torus-vtn-flat.obj";
    std::string mesh_fullname = mesh_dir + mesh_file;

    Mesh = new mgl::Mesh();
    Mesh->joinIdenticalVertices();
    Mesh->create(mesh_fullname);
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
    Shaders = new mgl::ShaderProgram();
    Shaders->addShader(GL_VERTEX_SHADER, "cube-vs.glsl");
    Shaders->addShader(GL_FRAGMENT_SHADER, "cube-fs.glsl");

    Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
    if (Mesh->hasNormals()) {
        Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
    }
    if (Mesh->hasTexcoords()) {
        Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
    }
    if (Mesh->hasTangentsAndBitangents()) {
        Shaders->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);
    }

    Shaders->addUniform(mgl::MODEL_MATRIX);
    Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
    Shaders->create();

    ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
}

////////////////////////////////////////////////////////////////////////// CAMERA

void MyApp::createCameras() {
    OrbitCameras[0] = new mgl::OrbitCamera(UBO_BP, true, glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    OrbitCameras[1] = new mgl::OrbitCamera(UBO_BP, false, glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    activeIndex = 0;
}

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 ModelMatrix(1.0f);

void MyApp::drawScene(double elapsed) {
    OrbitCameras[activeIndex]->updateRotation(elapsed);
    Shaders->bind();
    glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    Mesh->draw();
    Shaders->unbind();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
    createMeshes();
    createShaderPrograms();  // after mesh;
    createCameras();
}

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
  OrbitCameras[0]->updatePerspectiveProjectionMatrix(winx, winy);
  OrbitCameras[1]->updatePerspectiveProjectionMatrix(winx, winy);
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(elapsed); }

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new MyApp());
  engine.setOpenGL(4, 6);
  engine.setWindow(800, 600, "Hello Modern 3D World", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
