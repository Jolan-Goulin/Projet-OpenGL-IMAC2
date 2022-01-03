# pragma once 

#include <GL/glew.h>
#include <iostream>
#include <stb/stb_image.hpp>

#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Image.hpp>

#include "glimac/GameObjects.hpp"
#include "glimac/Map.hpp"
#include <glimac/TrackballCamera.hpp>
#include <glimac/SDLWindowManager.hpp>

namespace glimac{

struct SkyBoxProgram{
    Program m_Program;
    GLint uMVMatrix;
    GLint uMVPMatrix;

    SkyBoxProgram(){};
    ~SkyBoxProgram(){};
    SkyBoxProgram(const FilePath &applicationPath) : m_Program(loadProgram(applicationPath.dirPath() + "shaders/skybox.vs.glsl",
                                                                           applicationPath.dirPath() + "shaders/skybox.fs.glsl"))
    {
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
    }
};

unsigned int loadCubemap(std::vector<std::string> faces);

class Box{
    private:
        GLuint m_vbo;
        GLuint m_vao;
        GLuint m_texture_skybox;
        GLuint m_texture_rainbow;
    public :
        void initialise();
        void drawSkybox(const TrackballCamera &camera, const GLuint uMVMatrix, const GLuint uMVPMatrix, const glm::mat4 &ProjMatrix) const;
        void drawCase(const int &x, const Personnage &p, const TrackballCamera &camera, const GLuint uMVMatrix, const GLuint uMVPMatrix, const glm::mat4 &ProjMatrix) const;
        void drawCasePit(const int &x, const Personnage &p, const TrackballCamera &camera, const GLuint uMVMatrix, const GLuint uMVPMatrix, const glm::mat4 &ProjMatrix) const;
        void drawObstacle(const int &x, const Personnage &p, const TrackballCamera &camera, const GLuint uMVMatrix, const GLuint uMVPMatrix, const glm::mat4 &ProjMatrix) const;
        void drawMonkeySkybox(const Personnage &p, const TrackballCamera &camera, const GLuint uMVMatrix, const GLuint uMVPMatrix, const glm::mat4 &ProjMatrix) const;
        ~Box();
        Box(){};
};
}
