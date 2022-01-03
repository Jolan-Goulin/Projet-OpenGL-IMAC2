#pragma once

#include <vector>
#include "common.hpp"

class Personnage {
    private : 
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
        inline int moons() {return m_moon_number;}
        inline float movedleft() {return m_moved_left;}
        inline float movedup() {return m_moved_up;}
        inline void upscore(int gain) {m_score += gain; m_moon_number = std::min(32,m_moon_number+1);}
        inline float distance() {return m_distance;}
        inline bool isInDanger() {return m_danger;}
        inline bool isAlive() {return m_alive;}
        inline bool isMovingRight() {return m_isMovingRight;}
        inline void movingRight(){m_isMovingRight = true; m_isMovingLeft = false;}
        inline void movingLeft(){m_isMovingLeft = true; m_isMovingRight = false;}
        void colisionObstacle();
        void colisionPiece();
        void kill();
        inline void resurect(){m_alive=true; m_immortal =100;}
        void initialise();
        inline void pause(){m_pause = !m_pause;}
        inline bool paused(){return m_pause;}
};
