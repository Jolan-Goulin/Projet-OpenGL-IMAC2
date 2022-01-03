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

using namespace glimac;

struct EarthProgram {
    Program m_Program;
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram(){};
    ~EarthProgram(){};
    EarthProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
    }
};

struct MoonProgram {
    Program m_Program;
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    MoonProgram(){};
    ~MoonProgram(){};
    MoonProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/tex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }
};

GLuint* texturesInit();

class PersoDraw{
    private :
        GLuint m_vbo = 0;
        GLuint m_vao = 0;
        GLuint* m_textures;
        Sphere m_sphere;
        std::vector<glm::vec3> m_randomTransforms;

    public :
        void initialise();
        void drawEarth(TrackballCamera camera, GLuint uMVMatrix, GLuint uNormalMatrix, GLuint uMVPMatrix, glm::mat4 ProjMatrix, float time);
        void drawMoons(Personnage p, TrackballCamera camera, GLuint uMVMatrix, GLuint uNormalMatrix, GLuint uMVPMatrix, glm::mat4 ProjMatrix, float time);
        void drawPiece(float x, Personnage p, TrackballCamera camera, GLuint uMVMatrix, GLuint uNormalMatrix, GLuint uMVPMatrix, glm::mat4 ProjMatrix);
        PersoDraw();
        ~PersoDraw();
};