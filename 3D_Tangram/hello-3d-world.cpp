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
  void windowCloseCallback(GLFWwindow *win) override;
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

  double previousMousePositionX = 0;
  double previousMousePositionY = 0;

  void createShaderProgram();
  void createBufferObjects();
  void destroyBufferObjects();
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


///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderProgram() {
  Shaders = new mgl::ShaderProgram();
  Shaders->addShader(GL_VERTEX_SHADER, "color-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "color-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
  Shaders->addAttribute(mgl::COLOR_ATTRIBUTE, COLOR);
  Shaders->addUniform(mgl::MODEL_MATRIX);
  Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);

  Shaders->create();

  ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
}

////////////////////////////////////////////////////////////////// VAO, VBO, EBO

typedef struct {
  GLfloat XYZW[4];
  GLfloat RGBA[4];
} Vertex;

const Vertex Vertices[] = {
    {{0.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.1f, 1.0f}},  // 0 - FRONT
    {{1.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.1f, 1.0f}},  // 1
    {{1.0f, 1.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.1f, 1.0f}},  // 2
                                                           // 2
    {{0.0f, 1.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.1f, 1.0f}},  // 3
                                                           // 0

    {{1.0f, 0.0f, 1.0f, 1.0f}, {0.1f, 0.9f, 0.1f, 1.0f}},  // 1 - RIGHT
    {{1.0f, 0.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.1f, 1.0f}},  // 5
    {{1.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.1f, 1.0f}},  // 6
                                                           // 6
    {{1.0f, 1.0f, 1.0f, 1.0f}, {0.1f, 0.9f, 0.1f, 1.0f}},  // 2
                                                           // 1

    {{1.0f, 1.0f, 1.0f, 1.0f}, {0.1f, 0.1f, 0.9f, 1.0f}},  // 2 - TOP
    {{1.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.1f, 0.9f, 1.0f}},  // 6
    {{0.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.1f, 0.9f, 1.0f}},  // 7
                                                           // 7
    {{0.0f, 1.0f, 1.0f, 1.0f}, {0.1f, 0.1f, 0.9f, 1.0f}},  // 3
                                                           // 2

    {{1.0f, 0.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.9f, 1.0f}},  // 5 - BACK
    {{0.0f, 0.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.9f, 1.0f}},  // 4
    {{0.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.9f, 1.0f}},  // 7
                                                           // 7
    {{1.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.9f, 1.0f}},  // 6
                                                           // 5

    {{0.0f, 0.0f, 0.0f, 1.0f}, {0.9f, 0.1f, 0.9f, 1.0f}},  // 4 - LEFT
    {{0.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.9f, 1.0f}},  // 0
    {{0.0f, 1.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.9f, 1.0f}},  // 3
                                                           // 3
    {{0.0f, 1.0f, 0.0f, 1.0f}, {0.9f, 0.1f, 0.9f, 1.0f}},  // 7
                                                           // 4

    {{0.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.9f, 0.1f, 1.0f}},  // 0 - BOTTOM
    {{0.0f, 0.0f, 0.0f, 1.0f}, {0.9f, 0.9f, 0.1f, 1.0f}},  // 4
    {{1.0f, 0.0f, 0.0f, 1.0f}, {0.9f, 0.9f, 0.1f, 1.0f}},  // 5
                                                           // 5
    {{1.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.9f, 0.1f, 1.0f}}   // 1
                                                           // 0
};

const unsigned int Indices[] = {
    0,  1,  2,  2,  3,  0,   // FRONT
    4,  5,  6,  6,  7,  4,   // RIGHT
    8,  9,  10, 10, 11, 8,   // TOP
    12, 13, 14, 14, 15, 12,  // BACK
    16, 17, 18, 18, 19, 16,  // LEFT
    20, 21, 22, 22, 23, 20   // BOTTOM
};

void MyApp::createBufferObjects() {
  GLuint boId[2];

  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);
  {
    glGenBuffers(2, boId);
    glBindBuffer(GL_ARRAY_BUFFER, boId[0]);
    {
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

      glEnableVertexAttribArray(POSITION);
      glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

      glEnableVertexAttribArray(COLOR);
      glVertexAttribPointer(
          COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
          reinterpret_cast<GLvoid *>(sizeof(Vertices[0].XYZW)));
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boId[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
                 GL_STATIC_DRAW);
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  //glDeleteBuffers(2, boId);

  OrbitCameras[0] = new mgl::OrbitCamera(UBO_BP, true, glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  OrbitCameras[1] = new mgl::OrbitCamera(UBO_BP, false, glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  activeIndex = 0;
}

void MyApp::destroyBufferObjects() {
  glBindVertexArray(VaoId);
  glDisableVertexAttribArray(POSITION);
  glDisableVertexAttribArray(COLOR);
  glDeleteVertexArrays(1, &VaoId);
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 ModelMatrix = glm::scale(glm::vec3(2.0f)) * glm::translate(glm::vec3(-0.5f, -0.5f, -0.5f));

void MyApp::drawScene(double elapsed) {
  OrbitCameras[activeIndex]->updateRotation(elapsed);

  glm::mat4(glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

  glBindVertexArray(VaoId);
  Shaders->bind();

  glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  Shaders->unbind();
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  createBufferObjects();
  createShaderProgram();
}

void MyApp::windowCloseCallback(GLFWwindow *win) { destroyBufferObjects(); }

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
