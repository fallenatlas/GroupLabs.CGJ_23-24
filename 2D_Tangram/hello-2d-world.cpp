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

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
 public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;

 private:
  const GLuint POSITION = 0, COLOR = 1;
  GLuint VaoId[3], VboId[3][2];
  mgl::ShaderProgram *Shaders;
  GLint MatrixId;
  GLint ColorId;

  void createShaderProgram();
  void createBufferObjects();
  void destroyBufferObjects();
  void drawScene();
};

/*
class drawableObject {
public:
    GLuint VaoId;
    mgl::ShaderProgram* Shaders;
    GLint MatrixId;
    GLint ColorId;
    glm::mat4 TranformationMatrix;
    Color color[3];

    void drawObject() {}
};

void drawableObject::drawObject() {
    glBindVertexArray(VaoId);
    Shaders->bind();

    glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
    glUniform3fv(ColorId, 1, Color);
    glDrawElements(GL_TRIANGLES, sizeof(TriangleIndices) / sizeof(GLubyte), GL_UNSIGNED_BYTE,
        reinterpret_cast<GLvoid*>(0));
}
*/


//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram() {
  Shaders = new mgl::ShaderProgram();
  Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
  Shaders->addAttribute(mgl::COLOR_ATTRIBUTE, COLOR);
  Shaders->addUniform("Matrix");
  Shaders->addUniform("Color");

  Shaders->create();

  MatrixId = Shaders->Uniforms["Matrix"].index;
  ColorId = Shaders->Uniforms["Color"].index;
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs

/*
typedef struct {
  GLfloat XYZW[4];
  GLfloat RGBA[4];
} Vertex;
*/

typedef struct {
    GLfloat XYZW[4];
} Vertex;

typedef struct {
    GLubyte order[3];
} Indice;

typedef struct {
    GLfloat RGBA[4];
} Color;

/*
const Vertex Vertices[] = {
    {{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.75f, 0.25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.50f, 0.75f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}};

const GLubyte Indices[] = {0, 1, 2};
*/

/*
const Vertex Vertices[] = {
    // purple
    {{0.0f, -0.25f, 0.0f, 1.0f}, {0.451f, 0.247f, 0.800f, 1.0f}},
    {{0.25f, -0.25f, 0.0f, 1.0f}, {0.451f, 0.247f, 0.800f, 1.0f}},
    {{0.0f, 0.0f, 0.0f, 1.0f}, {0.451f, 0.247f, 0.800f, 1.0f}},
    // yellow
    {{0.0f, -0.25f, 0.0f, 1.0f}, {1.0f, 0.608f, 0.153f, 1.0f}},
    {{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.608f, 0.153f, 1.0f}},
    {{-0.125f, 0.125f, 0.0f, 1.0f}, {1.0f, 0.608f, 0.153f, 1.0f}},
    {{-0.125f, -0.125f, 0.0f, 1.0f}, {1.0f, 0.608f, 0.153f, 1.0f}},
    // blue
    {{0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{0.354f, 0.354f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    {{0.0f, 0.354f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}
};
*/

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
    {-0.124f, 0.478f, 0.0f, 1.0f},
    {0.124f, 0.478f, 0.0f, 1.0f},
    {0.0f, 0.604f, 0.0f, 1.0f}
};

const GLubyte SquareIndices[] = {
    0, 2, 1,
    2, 3, 1
};

/*
const Vertex SquareVertices[] = {

};

const GLubyte SquareIndices[] = {

};
*/

/*
const GLubyte Indices[] = {
    0, 1, 2,
    3, 4, 6,
    4, 5, 6,
    7, 8, 9
};
*/

/*
void MyApp::createBufferObjects() {
  int index = 0;
  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);
  {
    glGenBuffers(2, VboId);

    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
    {
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(POSITION);
      glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<GLvoid *>(0));
      glEnableVertexAttribArray(COLOR);
      glVertexAttribPointer(
          COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
          reinterpret_cast<GLvoid *>(sizeof(Vertices[0].XYZW)));
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
                   GL_STATIC_DRAW);
    }
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  //glDeleteBuffers(2, VboId);
}*/

void MyApp::createBufferObjects() {
    glGenVertexArrays(3, VaoId);
    
    glBindVertexArray(VaoId[0]);
    {
        glGenBuffers(2, VboId[0]);

        glBindBuffer(GL_ARRAY_BUFFER, VboId[0][0]);
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(POSITION);
            glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<GLvoid*>(0));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[0][1]);
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TriangleIndices), TriangleIndices,
                GL_STATIC_DRAW);
        }
    }

    glBindVertexArray(VaoId[1]);
    {
        glGenBuffers(2, VboId[1]);

        glBindBuffer(GL_ARRAY_BUFFER, VboId[1][0]);
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(ParalelogramVertices), ParalelogramVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(POSITION);
            glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<GLvoid*>(0));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1][1]);
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ParalelogramIndices), ParalelogramIndices,
                GL_STATIC_DRAW);
        }
    }

    glBindVertexArray(VaoId[2]);
    {
        glGenBuffers(2, VboId[2]);

        glBindBuffer(GL_ARRAY_BUFFER, VboId[2][0]);
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(SquareVertices), SquareVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(POSITION);
            glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<GLvoid*>(0));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[2][1]);
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SquareIndices), SquareIndices,
                GL_STATIC_DRAW);
        }
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glDeleteBuffers(2, VboId);

}

void MyApp::destroyBufferObjects() {
  /*
  glBindVertexArray(VaoId);
  glDisableVertexAttribArray(POSITION);
  glDisableVertexAttribArray(COLOR);
  glDeleteVertexArrays(1, &VaoId);
  glBindVertexArray(0);
  */
}

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 I(1.0f);
const glm::mat4 M = glm::translate(glm::vec3(0.0f, 0.75f, 0.0f));
const glm::mat4 MBlue = glm::rotate(glm::scale(glm::translate(glm::vec3(0.354f, 0.354f, 0.0f)), glm::vec3(1.416f, 1.416f, 1.416f)), -glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 MMagenta = glm::rotate(glm::scale(glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.416f, 1.416f, 1.416f)), glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
const glm::mat4 MAqua = glm::scale(glm::translate(glm::vec3(-0.354f, 0.531f, 0.0f)), glm::vec3(0.708f, 0.708f, 0.708f));
const glm::mat4 MOrange = glm::rotate(glm::scale(glm::translate(glm::vec3(0.177f, 0.354f, 0.0f)), glm::vec3(0.708f, 0.708f, 0.708f)), glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));

//const glm::mat4 translate = glm::translate(glm::vec3(0.0f, 0.75f, 0.0f));

void MyApp::drawScene() {
  // Drawing directly in clip space

  GLfloat triangleColor[4] = { 0.451f, 0.247f, 0.800f, 1.0f};
  GLfloat blueColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f};
  GLfloat magentaColor[4] = { 0.808f, 0.172f, 0.416f, 1.0f};
  GLfloat aquaColor[4] = { 0.0f, 0.6f, 0.6f, 1.0f};
  GLfloat orangeColor[4] = { 1.0f, 0.25f, 0.25f, 1.0f};
  // 0, 1, 1
  GLfloat paralelogramColor[4] = { 1.0f, 0.608f, 0.153f, 1.0f};
  //green
  GLfloat squareColor[4] = { 0.0f, 0.6f, 0.0f, 1.0f};

  // purple triangle
  glBindVertexArray(VaoId[0]);
  Shaders->bind();

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  glUniform4fv(ColorId, 1, triangleColor);
  glDrawElements(GL_TRIANGLES, sizeof(TriangleIndices)/sizeof(GLubyte), GL_UNSIGNED_BYTE,
                 reinterpret_cast<GLvoid *>(0));

  // blue triagle
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(MBlue));
  glUniform4fv(ColorId, 1, blueColor);
  glDrawElements(GL_TRIANGLES, sizeof(TriangleIndices) / sizeof(GLubyte), GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));

  // magenta triagle
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(MMagenta));
  glUniform4fv(ColorId, 1, magentaColor);
  glDrawElements(GL_TRIANGLES, sizeof(TriangleIndices) / sizeof(GLubyte), GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));

  //Aqua triangle
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(MAqua));
  glUniform4fv(ColorId, 1, aquaColor);
  glDrawElements(GL_TRIANGLES, sizeof(TriangleIndices) / sizeof(GLubyte), GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));

  //Orange triangle
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(MOrange));
  glUniform4fv(ColorId, 1, orangeColor);
  glDrawElements(GL_TRIANGLES, sizeof(TriangleIndices) / sizeof(GLubyte), GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));

  // paralelogram
  glBindVertexArray(VaoId[1]);

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  glUniform4fv(ColorId, 1, paralelogramColor);
  glDrawElements(GL_TRIANGLES, sizeof(ParalelogramIndices) / sizeof(GLubyte), GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));

  // square
  glBindVertexArray(VaoId[2]);

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  glUniform4fv(ColorId, 1, squareColor);
  glDrawElements(GL_TRIANGLES, sizeof(SquareIndices) / sizeof(GLubyte), GL_UNSIGNED_BYTE,
      reinterpret_cast<GLvoid*>(0));


  //glDrawArrays(GL_TRIANGLES, 0, 3);

  //glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M));
  //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
  //               reinterpret_cast<GLvoid *>(0));
  //glDrawArrays(GL_TRIANGLES, 0, 3);

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
