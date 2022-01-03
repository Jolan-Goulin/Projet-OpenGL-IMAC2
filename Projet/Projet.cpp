
#include <GL/glew.h>
#include <iostream>
#include <stb/stb_image.hpp>

#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Image.hpp>


#include "glimac/GameObjects.hpp"
#include "glimac/Map.hpp"
#include "glimac/PersoDraw.hpp"
#include "glimac/BoxDraw.hpp"
#include "glimac/Ttf.hpp"



using namespace glimac;


// create basic game variables
Personnage p;
std::vector<int> map(10, 0);
int map_start = 0;
float time_rotation = 0;



int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "To the moons !");
    //SDL_TTF_init();

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;
    
    // Shaders
    FilePath applicationPath(argv[0]);
    EarthProgram earthProgram(applicationPath);
    MoonProgram moonProgram(applicationPath);
    SkyBoxProgram skyBoxProgram(applicationPath);

    // Activate GPU's depth test
    glEnable(GL_DEPTH_TEST);

    // create needed matrix
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), 800 / 600.f, 0.1f, 100.f);
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    
    // Create a trackball camera 
    TrackballCamera camera;

    // Create structures used to draw 
    PersoDraw perso_draw;
    Box box;
    // Initialise drawing structures
    perso_draw.initialise();
    box.initialise();

    
    



    // Application loop:
	bool done = false;
	while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
			switch (e.type) {
				case SDL_QUIT:
					done = true;
					break;
                case SDL_MOUSEBUTTONUP:
                    // Check if user tried to zoom with mouse wheel ( what a bold user)
                    if (e.button.button == SDL_BUTTON_WHEELUP){
                        std::cout << "Zoom in" << std::endl;
                        camera.moveFront(0.5f);
                    }
                    else if (e.button.button == SDL_BUTTON_WHEELDOWN){
                        std::cout << "Zoom out" << std::endl;
                        camera.moveFront(-0.5f);
                    }
                break;
                // Check if user tried to imput a command with keyboard and execute it
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym) {
                        case SDLK_UP:
                            std::cout << "Zoom in" << std::endl;
                            camera.moveFront(1.f);
                            break;
                        case SDLK_DOWN:
                            std::cout << "Zoom out" << std::endl;
                            camera.moveFront(-1.f);
                            break;
                            case SDLK_d:
                            std::cout << "moving right" << std::endl;
                            p.movingRight();
                            break;
                        case SDLK_q:
                            std::cout << "moving left" << std::endl;
                            p.movingLeft();
                            break;
                        case SDLK_z:
                            std::cout << "saut" << std::endl;
                            p.saut();
                            break;
                        case SDLK_a:
                            std::cout << "mode change" << std::endl;
                            camera.changeMode();
                            break;
                        case SDLK_r:
                            std::cout << "respawn" << std::endl;
                            p.resurect();
                            break;
                        case SDLK_e:
                            std::cout << "restart" << std::endl;
                            initialise(map_start, map, p);
                            break;
                        case SDLK_p:
                            std::cout << "pause" << std::endl;
                            p.pause();
                            break;
                        case SDLK_l:
                            std::cout << "lock camera" << std::endl;
                            camera.lock();
                            break;
                        default:
                            break;
                        }
                break;
                // Check if the user drag the screen with his mouse and move camera according to his mouvements 
				case SDL_MOUSEMOTION:
                    if (windowManager.isMouseButtonPressed(SDL_BUTTON_LEFT)) {
                        if (e.motion.xrel != 0) {
                            camera.rotateUp(e.motion.xrel / 1.5f);
                        }
                        if (e.motion.yrel != 0) {
                            camera.rotateLeft(e.motion.yrel / 1.5f);

                        }
                        break;
					}
				default:
					break;
			}
		}
        
        // if the character is alive, draw the game
        if(p.isAlive()){
            // if needed update the map by deleting last case and creating a new one
            if(std::abs(map_start*2) +12 < p.distance()){
                map_start -=6;
                update(map);
            }
            // check if the character is in the map and collision with pieces, obstacles and pits
            isIn(map_start, map,  p);
            //make the character move according to the imputed deplacements
            p.avancer();
            //actualise time used for ratation only if character is alive (dead characters don't rotate) and if game ain't paused
            if(!p.paused()){
                time_rotation = windowManager.getTime();
            }
        }

            // affichage
            // Clean the depth buffer on each loop
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Prevent that your skybox draws depth values, and the skybox will never be in front of anything that will be drawn later
            glDisable(GL_DEPTH_TEST);
            // Skybox
            // get the shader
            skyBoxProgram.m_Program.use();
            // draw the skybox
            box.drawSkybox(camera, skyBoxProgram.uMVMatrix, skyBoxProgram.uMVPMatrix, ProjMatrix);
            
            // if the chamera is not in fps mode draw the character
            if (camera.modefps() == false){
                // Tell OpenGL to use the earthProgram
                earthProgram.m_Program.use();
                // Use to know on which unit the planet earth texture can be read by OpenGL
                glUniform1i(earthProgram.uEarthTexture, 0);
                // Use to know on which unit the cloud texture can be read by OpenGL
                glUniform1i(earthProgram.uCloudTexture, 1);
                // draw the character
                perso_draw.drawEarth(camera, earthProgram.uMVMatrix, earthProgram.uNormalMatrix, earthProgram.uMVPMatrix, ProjMatrix, time_rotation);
                // Tell OpenGL to use the moonProgram 
                moonProgram.m_Program.use();
                // draw the moons around the character
                perso_draw.drawMoons(p, camera, moonProgram.uMVMatrix, moonProgram.uNormalMatrix, moonProgram.uMVPMatrix, ProjMatrix, time_rotation);
                
            }
            // draw the cases, obstacles and pieces (and dont draw the pits since it's pits)
            // loop on the cases
            for (int i=0;i <10; i++){
                // get case's position
                int case_position = -6*i + map_start;
                // initialise case drawing
                skyBoxProgram.m_Program.use();
                // if there is not pit draw the case
                if (map[i]!=3){
                    box.drawCase(case_position, p, camera, skyBoxProgram.uMVMatrix, skyBoxProgram.uMVPMatrix, ProjMatrix);
                }
                // else draw a case with a pit
                else if(map[i]==3){
                    box.drawCasePit(case_position, p, camera, skyBoxProgram.uMVMatrix, skyBoxProgram.uMVPMatrix, ProjMatrix);
                }
                // if there is a piece draw it
                if(map[i]==1){
                    moonProgram.m_Program.use();
                    perso_draw.drawPiece(case_position, p, camera, moonProgram.uMVMatrix, moonProgram.uNormalMatrix, moonProgram.uMVPMatrix, ProjMatrix);
                }
                // if there is an obstacle draw it
                if(map[i]==2){
                    skyBoxProgram.m_Program.use();
                    box.drawObstacle(case_position, p, camera, skyBoxProgram.uMVMatrix, skyBoxProgram.uMVPMatrix, ProjMatrix);
                    }
            }
            // if the character is in danger draw and enemy chasing it
            if(p.isInDanger()){
                skyBoxProgram.m_Program.use();
                box.drawMonkeySkybox(p, camera, skyBoxProgram.uMVMatrix, skyBoxProgram.uMVPMatrix, ProjMatrix);
            }
        
    
        //flDrawMenu();
        // Unbind of GL_TEXTURE0 unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        // Unbind the VAO
        glBindVertexArray(0);
        // Update the display
        windowManager.swapBuffers();
	}

	return EXIT_SUCCESS;
}
