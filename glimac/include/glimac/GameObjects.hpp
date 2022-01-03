#pragma once

#include <vector>
#include "common.hpp"

namespace glimac {

class Personnage {
    private : 
    float m_vitesse_side = 0;
    float m_vitesse = 0;
    int m_score = 0;
    int m_moon_number =1;

    float m_distance = 0;
    float m_moved_left = 0;
    float m_moved_up = 0;
    bool m_isMovingLeft = false;
    bool m_isMovingRight = false;
    bool m_isMovingUp = false;

    bool m_danger = false;
    int m_time_danger = 0;
    bool m_alive = false;
    int m_immortal = 10;
    bool m_pause = false;
    

    public :
        void saut();
        void avancer();
        inline int moons() const {return m_moon_number;}
        inline float movedleft() const {return m_moved_left;}
        inline float movedup() const {return m_moved_up;}
        inline void upscore(int gain) {m_score += gain; m_moon_number = std::min(32,m_moon_number+1);}
        inline float distance() const {return m_distance;}
        inline bool isInDanger() const {return m_danger;}
        inline bool isAlive() const {return m_alive;}
        inline void movingRight(){m_isMovingRight = true; m_isMovingLeft = false;}
        inline void movingLeft(){m_isMovingLeft = true; m_isMovingRight = false;}
        void colisionObstacle();
        void colisionPiece();
        void kill();
        inline void resurect(){m_alive=true; m_immortal =100;}
        void initialise();
        inline void pause(){m_pause = !m_pause;}
        inline bool paused() const {return m_pause;} 
        inline int score() const {return m_score;}
        void write_highscore();
        int highscore();
        void save(const int &map_start, const std::vector<int> &map);
        void load(int &map_start, std::vector<int> &map);
};
}