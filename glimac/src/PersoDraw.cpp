
#include "glimac/PersoDraw.hpp"

const GLuint VERTEX_ATTR_POSITION = 0;
const GLuint VERTEX_ATTR_NORMAL = 1;
const GLuint VERTEX_ATTR_TEXCOORD = 2;

GLuint* texturesInit(){
    
    // Load textures (absolute path to the texture files)
    std::unique_ptr<Image> planetEarthTexture = loadImage("./../../assets/textures/EarthMap.jpg");
    std::unique_ptr<Image> moonTexture = loadImage("./../../assets/textures/MoonMap.jpg");
    std::unique_ptr<Image> cloudTexture = loadImage("./../../assets/textures/CloudMap.jpg");
    std::unique_ptr<Image> rainbowTexture = loadImage("./../../assets/textures/rainbow.png");

    // display errors if textures fail to load
    try{
    if (planetEarthTexture == NULL) {
        std::cout << "Planet Earth texture unique ptr is null. Exit program." << std::endl;
        throw std::exception_ptr();
    }
    

    if (moonTexture == NULL) {
        std::cout << "Moon texture unique ptr is null. Exit program." << std::endl;
        throw std::exception_ptr();
    }

    if (cloudTexture == NULL) {
        std::cout << "Cloud texture unique ptr is null. Exit program." << std::endl;
        throw std::exception_ptr();
    }

    if (rainbowTexture == NULL) {
        std::cout << "rainbowTexture unique ptr is null. Exit program." << std::endl;
        throw std::exception_ptr();
    }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }

    // Number of textures used
    unsigned int texturesCount = 4;

    // Generate textures
    GLuint *textures = new GLuint[texturesCount];
    glGenTextures(texturesCount, textures);

    // Bind the first texture
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		planetEarthTexture->getWidth(),
		planetEarthTexture->getHeight(),
		0,
		GL_RGBA,
		GL_FLOAT,
		planetEarthTexture->getPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

    // Bind the second texture
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		moonTexture->getWidth(),
		moonTexture->getHeight(),
		0,
		GL_RGBA,
		GL_FLOAT,
		moonTexture->getPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

    // Bind the third texture
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		cloudTexture->getWidth(),
		cloudTexture->getHeight(),
		0,
		GL_RGBA,
		GL_FLOAT,
		cloudTexture->getPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Bind the forth texture
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		rainbowTexture->getWidth(),
		rainbowTexture->getHeight(),
		0,
		GL_RGBA,
		GL_FLOAT,
		rainbowTexture->getPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

    return textures;
}

PersoDraw::PersoDraw()
    : m_sphere(Sphere(1, 32, 16)), m_textures(texturesInit())
{

};

void PersoDraw::initialise(){
    // Create a sphere (using glimac class Sphere)
    m_sphere = Sphere(1, 32, 16);
    m_textures= texturesInit();

     // VBO creation
    glGenBuffers(1, &m_vbo);
    // VBO binding
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // Send data (related to the sphere) into the VBO
    glBufferData(GL_ARRAY_BUFFER,  m_sphere.getVertexCount() * sizeof(ShapeVertex), m_sphere.getDataPointer(), GL_STATIC_DRAW);
    // Unbind the VBO (to avoid errors)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO creation
    glGenVertexArrays(1, &m_vao);
    // VAO binding
    glBindVertexArray(m_vao);

    // Activation of vertex attributs
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORD);
    // Define arrays of attribute data
    glVertexAttribPointer(
        VERTEX_ATTR_POSITION,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ShapeVertex),
        (const GLvoid*) offsetof(ShapeVertex, position)
    );
    glVertexAttribPointer(
        VERTEX_ATTR_NORMAL,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ShapeVertex),
        (const GLvoid*) offsetof(ShapeVertex, normal)
    );
    glVertexAttribPointer(
        VERTEX_ATTR_TEXCOORD,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ShapeVertex),
        (const GLvoid*) offsetof(ShapeVertex, texCoords)
    );
    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO
    glBindVertexArray(0);

    // Get a certain amount of random transformation (rotation) axes to draw moons later
	for (unsigned int i = 0; i < 32; i++)
		m_randomTransforms.push_back(glm::sphericalRand(2.f));
}

void PersoDraw::drawEarth(TrackballCamera camera, GLuint uMVMatrix, GLuint uNormalMatrix, GLuint uMVPMatrix, glm::mat4 ProjMatrix, float time){
    
        // Get the ViewMatrix
        glm::mat4 MVMatrix = camera.getViewMatrix();
        // Planet Earth transformations ( don' rotate if character is dead)
            glm::mat4 earthMVMatrix = glm::rotate(MVMatrix, time, glm::vec3(0, 1, 0)); // Translation * Rotation
        
        // Send matrices to the GPU
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(earthMVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * earthMVMatrix));
            
        // Bind the VAO
        glBindVertexArray(m_vao);
        
        // Bind the planet earth texture on the GL_TEXTURE0 unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textures[0]);
        // Bind the cloud texture on the GL_TEXTURE1 unit
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textures[2]);
        
        // Drawing call
        glDrawArrays(GL_TRIANGLES, 0, m_sphere.getVertexCount());
        
        // Unbind the VAO
        glBindVertexArray(0);
        
        
        // Unbind of GL_TEXTURE1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
}
void PersoDraw::drawMoons(Personnage p, TrackballCamera camera, GLuint uMVMatrix, GLuint uNormalMatrix, GLuint uMVPMatrix, glm::mat4 ProjMatrix, float time){
    glm::mat4 MVMatrix = camera.getViewMatrix();
    m_randomTransforms.push_back(glm::sphericalRand(2.f));
    for (int i = 0; i < p.moons(); ++i) {
        // Moons transformation 
        glm::mat4 moonMVMatrix = glm::rotate(MVMatrix, (1 + m_randomTransforms[i][0]+m_randomTransforms[i][1]+m_randomTransforms[i][2]) * time, glm::vec3(0, 1, 0)); // Translation * Rotation
        moonMVMatrix = glm::translate(moonMVMatrix, m_randomTransforms[i]); // Translation * Rotation * Translation
        moonMVMatrix = glm::scale(moonMVMatrix, glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
        
        // Send matrices to the GPU            
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(moonMVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(moonMVMatrix))));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * moonMVMatrix));
        
        // Bind the VAO
        glBindVertexArray(m_vao);

        // Bind the moon texture on the GL_TEXTURE0 unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textures[1]);
        
        // Drawing call
        glDrawArrays(GL_TRIANGLES, 0, m_sphere.getVertexCount());

        // Unbind the VAO
        glBindVertexArray(0);
        
        // Unbind of GL_TEXTURE1
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void PersoDraw::drawPiece(float x, Personnage p, TrackballCamera camera, GLuint uMVMatrix, GLuint uNormalMatrix, GLuint uMVPMatrix, glm::mat4 ProjMatrix){
    glm::mat4 MVMatrix = camera.getViewMatrix();
    m_randomTransforms.push_back(glm::sphericalRand(2.f));
    // Moons transformation 
    glm::mat4 moonMVMatrix = glm::translate(MVMatrix, glm::vec3(4*p.movedleft(), -p.movedup(), 2*x+p.distance())); // Translation * Rotation * Translation


    // Send matrices to the GPU            
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(moonMVMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(moonMVMatrix))));
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * moonMVMatrix));

    // Bind the VAO
    glBindVertexArray(m_vao);
    // Bind the moon texture on the GL_TEXTURE0 unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    
    // Drawing call
    glDrawArrays(GL_TRIANGLES, 0, m_sphere.getVertexCount());

    // Unbind of GL_TEXTURE1
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}



PersoDraw::~PersoDraw(){
    glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
    delete m_textures;
}