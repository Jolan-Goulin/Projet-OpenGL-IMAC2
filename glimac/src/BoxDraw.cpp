
#include "glimac/BoxDraw.hpp"

const GLuint VERTEX_ATTR_POSITION = 0;
const GLuint VERTEX_ATTR_NORMAL = 1;
const GLuint VERTEX_ATTR_TEXCOORD = 2;


unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

Box::~Box(){
    glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void Box::initialise() { 
    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

    
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glVertexAttribPointer(
        VERTEX_ATTR_POSITION,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(GLfloat),
        (GLvoid *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Cubemap (Skybox)
    std::vector<std::string> faces;

    faces.push_back("./../../assets/textures/skybox/space/right.png");
    faces.push_back("./../../assets/textures/skybox/space/left.png");
    faces.push_back("./../../assets/textures/skybox/space/top.png");
    faces.push_back("./../../assets/textures/skybox/space/bottom.png");
    faces.push_back("./../../assets/textures/skybox/space/front.png");
    faces.push_back("./../../assets/textures/skybox/space/back.png");

    m_texture_skybox = loadCubemap(faces);

    std::vector<std::string> facesrainbow;

    // Cubemap (Box)
    facesrainbow.push_back("./../../assets/textures/rainbow.png");
    facesrainbow.push_back("./../../assets/textures/rainbow.png");
    facesrainbow.push_back("./../../assets/textures/rainbow.png");
    facesrainbow.push_back("./../../assets/textures/rainbow.png");
    facesrainbow.push_back("./../../assets/textures/rainbow.png");
    facesrainbow.push_back("./../../assets/textures/rainbow.png");

    m_texture_rainbow = loadCubemap(facesrainbow);

}

void Box::drawSkybox(TrackballCamera camera, GLuint uMVMatrix, GLuint uMVPMatrix, glm::mat4 ProjMatrix) {

        // Remove the translation section of the MVMatrix
        glm::mat4 MVMatrix = glm::mat4(glm::mat3(camera.getViewMatrix()));

        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix));

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_skybox);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Set depth function back to default
        glDepthFunc(GL_LESS);

        glEnable(GL_DEPTH_TEST);

        // Change depth function so depth test passes when values are equal to depth buffer's content
        glDepthFunc(GL_LEQUAL);

}

void Box::drawCase(int x, Personnage p, TrackballCamera camera, GLuint uMVMatrix, GLuint uMVPMatrix, glm::mat4 ProjMatrix){
    glm::mat4 MVMatrix = camera.getViewMatrix();
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_rainbow);
    for (int j=0; j<6; j++){
        for (int i=0; i<6; i++){
            // Box transformation 
            glm::mat4 boxMVMatrix = glm::translate(MVMatrix, glm::vec3(-5+2*(j+2*p.movedleft()), -3.0-p.movedup(), -5.0 +2*i + 2*x+p.distance()) ); // Translation * Rotation * Translation
            
            // Send matrices to the GPU            
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(boxMVMatrix));
            //glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(moonMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix));
            
            // Drawing call
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Box::drawCasePit(int x, Personnage p, TrackballCamera camera, GLuint uMVMatrix, GLuint uMVPMatrix, glm::mat4 ProjMatrix){
    glm::mat4 MVMatrix = camera.getViewMatrix();
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_rainbow);
    for (int j=0; j<6; j++){
        for (int i=0; i<2; i++){
            // Box transformation 
            glm::mat4 boxMVMatrix = glm::translate(MVMatrix, glm::vec3(-5+2*(j+2*p.movedleft()), -3.0-p.movedup(), -5.0 +2*i + 2*x+p.distance()) ); // Translation * Rotation * Translation
            
            // Send matrices to the GPU            
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(boxMVMatrix));
            //glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(moonMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix));
            
            // Drawing call
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        for (int i=4; i<6; i++){
            // Box transformation 
            glm::mat4 boxMVMatrix = glm::translate(MVMatrix, glm::vec3(-5+2*(j+2*p.movedleft()), -3.0-p.movedup(), -5.0 +2*i + 2*x+p.distance()) ); // Translation * Rotation * Translation
            
            // Send matrices to the GPU            
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(boxMVMatrix));
            //glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(moonMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix));
            
            // Drawing call
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Box::drawObstacle(int x, Personnage p, TrackballCamera camera, GLuint uMVMatrix, GLuint uMVPMatrix, glm::mat4 ProjMatrix){
    glm::mat4 MVMatrix = camera.getViewMatrix();
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_rainbow);
    for (int j=2; j<4; j++){
        for (int i=2; i<4; i++){
            // Box transformation 
            glm::mat4 boxMVMatrix = glm::translate(MVMatrix, glm::vec3(-5+2*(j+2*p.movedleft()), -p.movedup(), -5.0 +2*i + 2*x+p.distance()) ); // Translation * Rotation * Translation
            
            // Send matrices to the GPU            
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(boxMVMatrix));
            //glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(moonMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix));
            
            // Drawing call
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Box::drawMonkeySkybox( Personnage p, TrackballCamera camera, GLuint uMVMatrix, GLuint uMVPMatrix, glm::mat4 ProjMatrix){
    glm::mat4 MVMatrix = camera.getViewMatrix();
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_rainbow);

    glm::mat4 boxMVMatrix = glm::translate(MVMatrix, glm::vec3(2*(2*p.movedleft()), 0.2-p.movedup(), 5.0)); 
    // Send matrices to the GPU            
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(boxMVMatrix));
    //glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(moonMVMatrix))));
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix));
    
    // Drawing call
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}