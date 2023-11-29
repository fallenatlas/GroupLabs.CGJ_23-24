////////////////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Clip Space.
// A "Hello 2D World" of Modern OpenGL.
//
// Copyright (c) 2013-23 by Carlos Martinho
//
// INTRODUCES:
// GL PIPELINE, mglShader.hpp, mglConventions.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "../mgl/mgl.hpp"

//////////////////////////////////////////////////////////////////// VERTICE AND SHAPE DATA

typedef struct {
    GLfloat XYZW[4];
} Vertex;

typedef struct {
    GLubyte order[3];
} Indice;

const Vertex TriangleVertices[] = {
    {0.0f, -0.25f, 0.0f, 1.0f},
    {0.25f, -0.25f, 0.0f, 1.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}
};

const GLubyte TriangleIndices[] = {
    0, 1, 2
};

const Vertex ParalelogramVertices[] = {
    {0.0f, -0.25f, 0.0f, 1.0f},
    {0.0f, 0.0f, 0.0f, 1.0f},
    {-0.125f, 0.125f, 0.0f, 1.0f},
    {-0.125f, -0.125f, 0.0f, 1.0f}
};

const GLubyte ParalelogramIndices[] = {
    0, 1, 3,
    1, 2, 3
};

const Vertex SquareVertices[] = {
    {0.0f, 0.354f, 0.0f, 1.0f},
    {-0.125f, 0.479f, 0.0f, 1.0f},
    {0.125f, 0.479f, 0.0f, 1.0f},
    {0.0f, 0.604f, 0.0f, 1.0f}
};

const GLubyte SquareIndices[] = {
    0, 2, 1,
    2, 3, 1
};

const GLuint POSITION = 0;

////////////////////////////////////////////////////////////////////////// DRAWABLEOBJECT

class DrawableObject {
public:
    GLuint VaoId, VboId[2];
    GLsizei NumberOfIndices;

    DrawableObject(const Vertex Vertices[], GLsizeiptr SizeOfVertices, const GLubyte Indices[], GLsizeiptr SizeOfIndices);
    void destroyBufferObject();
    void drawObject(mgl::ShaderProgram* Shaders, GLint MatrixId, GLint ColorId, glm::mat4 M, const GLfloat Color[4]);
};

DrawableObject::DrawableObject(const Vertex Vertices[], GLsizeiptr SizeOfVertices, const GLubyte Indices[], GLsizeiptr SizeOfIndices) {
    glGenVertexArrays(1, &VaoId);

    glBindVertexArray(VaoId);
    {
        glGenBuffers(2, VboId);

        glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
        {
            glBufferData(GL_ARRAY_BUFFER, SizeOfVertices, Vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(POSITION);
            glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<GLvoid*>(0));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeOfIndices, Indices,
                GL_STATIC_DRAW);
        }

    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    NumberOfIndices = SizeOfIndices / sizeof(GLubyte);
}

void DrawableObject::destroyBufferObject() {
    glBindVertexArray(VaoId);
    glDisableVertexAttribArray(POSITION);
    glDeleteVertexArrays(1, &VaoId);
    glBindVertexArray(0);
    glDeleteBuffers(2, VboId);
}

void DrawableObject::drawObject(mgl::ShaderProgram* Shaders, GLint MatrixId, GLint ColorId, glm::mat4 M, const GLfloat Color[4]) {
    glBindVertexArray(VaoId);
    Shaders->bind();

    glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M));
    glUniform4fv(ColorId, 1, Color);
    glDrawElements(GL_TRIANGLES, NumberOfIndices, GL_UNSIGNED_BYTE,
        reinterpret_cast<GLvoid*>(0));

    Shaders->unbind();
    glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
    void initCallback(GLFWwindow* win) override;
    void displayCallback(GLFWwindow* win, double elapsed) override;
    void windowCloseCallback(GLFWwindow* win) override;
    void windowSizeCallback(GLFWwindow* win, int width, int height) override;

private:
    mgl::ShaderProgram* Shaders;
    GLint MatrixId;
    GLint ColorId;

    DrawableObject *TriangleShape;
    DrawableObject *ParalelogramShape;
    DrawableObject *SquareShape;

    void createShaderProgram();
    void createBufferObjects();
    void destroyBufferObjects();
    void drawScene();
};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram() {
    Shaders = new mgl::ShaderProgram();
    Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
    Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

    Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
    Shaders->addUniform("Matrix");
    Shaders->addUniform("Color");

    Shaders->create();

    MatrixId = Shaders->Uniforms["Matrix"].index;
    ColorId = Shaders->Uniforms["Color"].index;
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs

void MyApp::createBufferObjects() {
    TriangleShape = new DrawableObject(TriangleVertices, sizeof(TriangleVertices), TriangleIndices, sizeof(TriangleIndices));
    ParalelogramShape = new DrawableObject(ParalelogramVertices, sizeof(ParalelogramVertices), ParalelogramIndices, sizeof(ParalelogramIndices));
    SquareShape = new DrawableObject(SquareVertices, sizeof(SquareVertices), SquareIndices, sizeof(SquareIndices));
}

void MyApp::destroyBufferObjects() {
    TriangleShape->destroyBufferObject();
    ParalelogramShape->destroyBufferObject();
    SquareShape->destroyBufferObject();
}

////////////////////////////////////////////////////////////////////////// SCENE

// tranformation matrices
const glm::mat4 I(1.0f);
const glm::mat4 MBlue = glm::rotate(glm::scale(glm::translate(glm::vec3(0.354f, 0.354f, 0.0f)), glm::vec3(1.416f, 1.416f, 1.416f)), -glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 MMagenta = glm::rotate(glm::scale(I, glm::vec3(1.416f, 1.416f, 1.416f)), glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 MAqua = glm::scale(glm::translate(glm::vec3(-0.354f, 0.531f, 0.0f)), glm::vec3(0.708f, 0.708f, 0.708f));
const glm::mat4 MOrange = glm::rotate(glm::scale(glm::translate(glm::vec3(0.177f, 0.354f, 0.0f)), glm::vec3(0.708f, 0.708f, 0.708f)), glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));

// colors
const GLfloat purpleColor[4] = { 0.451f, 0.247f, 0.800f, 1.0f };
const GLfloat blueColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
const GLfloat magentaColor[4] = { 0.808f, 0.172f, 0.416f, 1.0f };
const GLfloat aquaColor[4] = { 0.0f, 0.6f, 0.6f, 1.0f };
const GLfloat orangeColor[4] = { 1.0f, 0.25f, 0.25f, 1.0f };
const GLfloat paralelogramColor[4] = { 1.0f, 0.608f, 0.153f, 1.0f };
const GLfloat squareColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f };

void MyApp::drawScene() {
  // Drawing directly in clip space

  // triangles
  TriangleShape->drawObject(Shaders, MatrixId, ColorId, I, purpleColor);
  TriangleShape->drawObject(Shaders, MatrixId, ColorId, MBlue, blueColor);
  TriangleShape->drawObject(Shaders, MatrixId, ColorId, MMagenta, magentaColor);
  TriangleShape->drawObject(Shaders, MatrixId, ColorId, MAqua, aquaColor);
  TriangleShape->drawObject(Shaders, MatrixId, ColorId, MOrange, orangeColor);

  // paralelogram
  ParalelogramShape->drawObject(Shaders, MatrixId, ColorId, I, paralelogramColor);
  
  // square
  SquareShape->drawObject(Shaders, MatrixId, ColorId, I, squareColor);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  createBufferObjects();
  createShaderProgram();
}

void MyApp::windowCloseCallback(GLFWwindow *win) { destroyBufferObjects(); }

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(); }

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new MyApp());
  engine.setOpenGL(4, 6);
  engine.setWindow(600, 600, "Hello Modern 2D World", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END
