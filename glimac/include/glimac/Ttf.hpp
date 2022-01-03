#pragma once

#include <iostream>
#include <SDL_ttf.h>

#include "glimac/common.hpp"

void SDL_TTF_init();
void SDL_GL_RenderText(char *text, 
                      SDL_Color color,
                      SDL_Rect *location);
void glEnable2D();
void glDisable2D();
void flDrawMenu();
