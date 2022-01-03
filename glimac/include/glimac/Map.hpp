#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"

#include <GL/glew.h>
#include <stb/stb_image.hpp>

#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Image.hpp>
#include "glimac/GameObjects.hpp"

namespace glimac {

void update(std::vector<int> &map);

void isIn(const int start_map, std::vector<int> &map, Personnage &p);

void initialise(int &start_map, std::vector<int> &map, Personnage &p);

}