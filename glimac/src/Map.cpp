#include <cmath>
#include <vector>
#include <iostream>
#include <random>
#include "glimac/common.hpp"

#include <GL/glew.h>
#include <stb/stb_image.hpp>

#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Image.hpp>
#include <glimac/Map.hpp>

namespace glimac {

void update(std::vector<int> &map){
    map.erase(map.begin());
    map.push_back(rand()%4);
}

void isIn(const int start_map, std::vector<int> &map, Personnage &p){
    int start_map_abs = std::abs(start_map);
    if(p.movedleft()<-1.3 || p.movedleft() > 1.3){
        p.kill();
    }
    for(int i =0; i<10; i++){
        if(std::abs(p.distance()-2*(start_map_abs+6*i))<6){
            if(map[i]==1 && std::abs(p.distance()-2*(start_map_abs+6*i))<1 && p.movedup() < 2){
                if(std::abs(p.movedleft())<0.5){
                    map[i]=0;
                    p.colisionPiece();
                }
            }
            if(map[i]==2 && std::abs(p.distance()-2*(start_map_abs+6*i))<2 && p.movedup() < 2){
                if(std::abs(p.movedleft())<0.5){
                    map[i]=0;
                    p.colisionObstacle();
                }
            }
            if(map[i]==3 && std::abs(p.distance()-2*(start_map_abs+6*i))<2 && p.movedup() < 0.05){
                p.kill();
            }
        }
        
    }
}

void initialise(int &start_map, std::vector<int> &map, Personnage &p){
    p.initialise();
    start_map = 0;
    int basic_case =0;
    int map_length = 10;
    map = std::vector<int> (map_length, basic_case);
}

}