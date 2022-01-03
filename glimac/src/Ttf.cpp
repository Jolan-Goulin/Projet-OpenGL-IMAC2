#include "glimac/common.hpp"
#include "glimac/Ttf.hpp"


int screenwidth = 800;
int screenheight = 600;

int Round(double x)
{
	return (int)(x + 0.5);
}

int nextpoweroftwo(int x)
{
	double logbase2 = log(x) / log(2);
	return Round(pow(2,ceil(logbase2)));
}

void SDL_TTF_init(){
	std::cout << "yo" << std::endl;
	if(TTF_Init()==1){
		std::cout << " Initialising TTF error" << std::endl;
		atexit(TTF_Quit);
    }
}
void SDL_GL_RenderText(char *text, 
                      SDL_Color color,
                      SDL_Rect *location)
{
    TTF_Font *font = TTF_OpenFont("/home/jolan/Desktop/GLImac-main/assets/font/Roboto-Bold.ttf", 20);
	std::cout << "yo" << std::endl;
	SDL_Surface *initial;
	SDL_Surface *intermediary;
	SDL_Rect rect;
	int w,h;
	GLuint texture;
	
	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(font, text, color);
	
	/* Convert the rendered text to a known format */
	w = nextpoweroftwo(initial->w);
	h = nextpoweroftwo(initial->h);
	
	intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	SDL_BlitSurface(initial, 0, intermediary, 0);
	
	/* Tell GL about our new texture */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, 
			GL_UNSIGNED_BYTE, intermediary->pixels );
	
	/* GL_NEAREST looks horrible, if scaled... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	/* prepare to render our texture */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);
	
	/* Draw a quad at location */
	glBegin(GL_QUADS);
		/* Recall that the origin is in the lower-left corner
		   That is why the TexCoords specify different corners
		   than the Vertex coors seem to. */
		glTexCoord2f(0.0f, 1.0f); 
			glVertex2f(location->x    , location->y);
		glTexCoord2f(1.0f, 1.0f); 
			glVertex2f(location->x + w, location->y);
		glTexCoord2f(1.0f, 0.0f); 
			glVertex2f(location->x + w, location->y + h);
		glTexCoord2f(0.0f, 0.0f); 
			glVertex2f(location->x    , location->y + h);
	glEnd();
	
	/* Bad things happen if we delete the texture before it finishes */
	glFinish();
	
	/* return the deltas in the unused w,h part of the rect */
	location->w = initial->w;
	location->h = initial->h;
	
	/* Clean up */
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &texture);
	
}

void glEnable2D()
{
	int vPort[4];
  
	glGetIntegerv(GL_VIEWPORT, vPort);
  
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
  
	glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void glDisable2D()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();   
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	
}

void flDrawMenu(){
	std::cout << "yo" << std::endl;
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);
	glDisable(GL_TEXTURE_2D);

	/* Go in HUD-drawing mode */
	glEnable2D();
	glDisable(GL_DEPTH_TEST);
	
	SDL_Color color;
	SDL_Rect position;

	/* Draw some text */
	color.r = 255;
	color.g = 255;
	color.b = 255;
	
	position.x = screenwidth / 6;
	position.y = screenheight / 2;
	SDL_GL_RenderText("Restart", color, &position);
	position.y -= position.h;
	SDL_GL_RenderText("Best scores", color, &position);
	position.y -= position.h;
	SDL_GL_RenderText("Play", color, &position);

	/* Come out of HUD mode */
	glEnable(GL_DEPTH_TEST);
	glDisable2D();
}
