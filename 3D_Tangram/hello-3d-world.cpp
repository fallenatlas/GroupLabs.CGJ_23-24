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
  GLint NormalMatrixId;
  GLint ColorId;
  mgl::SceneGraph* SceneGraph = nullptr;
  mgl::Mesh* SquareMesh = nullptr;
  mgl::Mesh* ParalelogramMesh = nullptr;
  mgl::Mesh* TriangleMesh = nullptr;

  double previousMousePositionX = 0;
  double previousMousePositionY = 0;

  void createMeshes();
  void createShaderPrograms();
  void createCameras();
  void createScene();
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
    std::string mesh_dir = "../assets/";
    //std::string mesh_dir = "../04-assets/models/";
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
    //std::string mesh_file = "monkey-torus-vtn-flat.obj";
    std::string square_mesh_file = "square_fixed.obj";
    std::string paralelogram_mesh_file = "paralelogram_fixed.obj";
    std::string triangle_mesh_file = "triangle_fixed.obj";
    std::string square_mesh_fullname = mesh_dir + square_mesh_file;
    std::string paralelogram_mesh_fullname = mesh_dir + paralelogram_mesh_file;
    std::string triangle_mesh_fullname = mesh_dir + triangle_mesh_file;

    SquareMesh = new mgl::Mesh();
    SquareMesh->joinIdenticalVertices();
    SquareMesh->create(square_mesh_fullname);

    ParalelogramMesh = new mgl::Mesh();
    ParalelogramMesh->joinIdenticalVertices();
    ParalelogramMesh->create(paralelogram_mesh_fullname);

    TriangleMesh = new mgl::Mesh();
    TriangleMesh->joinIdenticalVertices();
    TriangleMesh->create(triangle_mesh_fullname);
}

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
    Shaders = new mgl::ShaderProgram();
    Shaders->addShader(GL_VERTEX_SHADER, "cube-vs.glsl");
    Shaders->addShader(GL_FRAGMENT_SHADER, "cube-fs.glsl");

    Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
    if (SquareMesh->hasNormals() && TriangleMesh->hasNormals() && ParalelogramMesh->hasNormals()) {
        Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
    }
    if (SquareMesh->hasTexcoords() && TriangleMesh->hasTexcoords() && ParalelogramMesh->hasTexcoords()) {
        Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
    }
    if (SquareMesh->hasTangentsAndBitangents() && TriangleMesh->hasTangentsAndBitangents() && ParalelogramMesh->hasTangentsAndBitangents()) {
        Shaders->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);
    }

    Shaders->addUniform(mgl::MODEL_MATRIX);
    Shaders->addUniform(mgl::COLOR_UNIFORM);
    Shaders->addUniform(mgl::NORMAL_MATRIX);
    Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
    Shaders->create();

    ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
    NormalMatrixId = Shaders->Uniforms[mgl::NORMAL_MATRIX].index;
    ColorId = Shaders->Uniforms[mgl::COLOR_UNIFORM].index;
}

// model matrixes

// colors
const glm::vec4 purpleColor = { 0.451f, 0.247f, 0.800f, 1.0f };
const glm::vec4 blueColor = { 0.0f, 0.0f, 1.0f, 1.0f };
const glm::vec4 magentaColor = { 0.808f, 0.172f, 0.416f, 1.0f };
const glm::vec4 aquaColor = { 0.0f, 0.6f, 0.6f, 1.0f };
const glm::vec4 orangeColor = { 1.0f, 0.25f, 0.25f, 1.0f };
const glm::vec4 paralelogramColor = { 1.0f, 0.608f, 0.153f, 1.0f };
const glm::vec4 squareColor = { 0.0f, 0.6f, 0.0f, 1.0f };

void MyApp::createScene() {
    SceneGraph = new mgl::SceneGraph();

    // square
    mgl::SceneNode* squareNode = new mgl::SceneNode(ModelMatrixId, NormalMatrixId, ColorId);
    squareNode->setMesh(SquareMesh);
    glm::mat4 squareMM = glm::translate(glm::vec3(-7.0f, 2.0f, 0.004f + 1.416f * 1.416f)) * glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    squareNode->setModelMatrix(squareMM);
    squareNode->setNormalMatrix(glm::transpose(glm::inverse(squareMM)));
    squareNode->setColor(squareColor);
    squareNode->setShaderProgram(Shaders);
    SceneGraph->addNode(squareNode);

    // paralelogram
    mgl::SceneNode* paralelogramNode = new mgl::SceneNode(ModelMatrixId, NormalMatrixId, ColorId);
    paralelogramNode->setMesh(ParalelogramMesh);
    glm::mat4 paralelogramMM = glm::translate(glm::vec3(-7.23f, -0.005f + 2.0f * 1.416f * 1.416f, - 1.416f * 1.416f)) * glm::rotate(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    paralelogramNode->setModelMatrix(paralelogramMM);
    paralelogramNode->setNormalMatrix(glm::transpose(glm::inverse(paralelogramMM)));
    paralelogramNode->setColor(paralelogramColor);
    paralelogramNode->setShaderProgram(Shaders);
    SceneGraph->addNode(paralelogramNode);

    // small triangles
    // cyan
    mgl::SceneNode* cyanTriangleNode = new mgl::SceneNode(ModelMatrixId, NormalMatrixId, ColorId);
    cyanTriangleNode->setMesh(TriangleMesh);
    glm::mat4 cyanMM = glm::translate(glm::vec3(-7.01f, - 0.005f + 2.0f * 1.416f * 1.416f, 1.416f * 1.416f)) * glm::rotate(glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
    cyanTriangleNode->setModelMatrix(cyanMM);
    cyanTriangleNode->setNormalMatrix(glm::transpose(glm::inverse(cyanMM)));
    cyanTriangleNode->setColor(aquaColor);
    cyanTriangleNode->setShaderProgram(Shaders);
    SceneGraph->addNode(cyanTriangleNode);

    // orange
    mgl::SceneNode* orangeTriangleNode = new mgl::SceneNode(ModelMatrixId, NormalMatrixId, ColorId);
    orangeTriangleNode->setMesh(TriangleMesh);
    glm::mat4 orangeMM = glm::translate(glm::vec3(-6.998f, 1.416f * 1.416f, 0.015f -1.416f * 1.416f)) * glm::rotate(glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(glm::radians(-90.f), glm::vec3(0.0f, 1.0f, 0.0f));
    cyanTriangleNode->setModelMatrix(cyanMM);
    orangeTriangleNode->setModelMatrix(orangeMM);
    orangeTriangleNode->setNormalMatrix(glm::transpose(glm::inverse(orangeMM)));
    orangeTriangleNode->setColor(orangeColor);
    orangeTriangleNode->setShaderProgram(Shaders);
    SceneGraph->addNode(orangeTriangleNode);
    
    // medium triangles
    // purple
    mgl::SceneNode* purpleTriangleNode = new mgl::SceneNode(ModelMatrixId, NormalMatrixId, ColorId);
    purpleTriangleNode->setMesh(TriangleMesh);
    glm::mat4 purpleMM = glm::scale(glm::vec3(1.0f, 1.416f, 1.416f)) * glm::translate(glm::vec3(-7.01f, -0.005f + 2.0f * 1.416f, 1.416f)) * glm::rotate(glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    purpleTriangleNode->setModelMatrix(purpleMM);
    purpleTriangleNode->setNormalMatrix(glm::transpose(glm::inverse(purpleMM)));
    purpleTriangleNode->setColor(purpleColor);
    purpleTriangleNode->setShaderProgram(Shaders);
    SceneGraph->addNode(purpleTriangleNode);

    // big triangles
    // blue
    mgl::SceneNode* blueTriangleNode = new mgl::SceneNode(ModelMatrixId, NormalMatrixId, ColorId);
    blueTriangleNode->setMesh(TriangleMesh);
    //The numbers may be slighty off (like -7.23f in translate)
    glm::mat4 blueMM = glm::scale(glm::vec3(1.0f, 1.416f, 1.416f)) * glm::scale(glm::vec3(1.0f, 1.416f, 1.416f)) * glm::translate(glm::vec3(-7.23f, 1.0f, -1.0f * 1.416f * 1.416f + 0.01f)) * glm::rotate(glm::radians(-90.f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(glm::radians(-90.f), glm::vec3(0.0f, 1.0f, 0.0f));
    blueTriangleNode->setModelMatrix(blueMM);
    blueTriangleNode->setNormalMatrix(glm::transpose(glm::inverse(blueMM)));
    blueTriangleNode->setColor(blueColor);
    blueTriangleNode->setShaderProgram(Shaders);
    SceneGraph->addNode(blueTriangleNode);

    // magenta
    mgl::SceneNode* magentaTriangleNode = new mgl::SceneNode(ModelMatrixId, NormalMatrixId, ColorId);
    magentaTriangleNode->setMesh(TriangleMesh);
    glm::mat4 magentaMM = glm::scale(glm::vec3(1.0f, 1.416f, 1.416f)) * glm::scale(glm::vec3(1.0f, 1.416f, 1.416f)) * glm::translate(glm::vec3(-7.0f, 1.0f, 1.0f * 1.416f * 1.416f)) * glm::rotate(glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
    magentaTriangleNode->setModelMatrix(magentaMM);
    magentaTriangleNode->setNormalMatrix(glm::transpose(glm::inverse(magentaMM)));
    magentaTriangleNode->setColor(magentaColor);
    magentaTriangleNode->setShaderProgram(Shaders);
    SceneGraph->addNode(magentaTriangleNode);
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
    SceneGraph->renderScene();
    /*
    Shaders->bind();
    glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    Mesh->draw();
    Shaders->unbind();
    */
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
    createMeshes();
    createShaderPrograms();  // after mesh;
    createCameras();
    createScene();
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
