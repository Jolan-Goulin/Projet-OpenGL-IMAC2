#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/GameObjects.hpp"
    



void Personnage::saut(){
    // if the character is on the gound, activate jumping state
    if( m_moved_up <0.01){
        m_isMovingUp = true;
    }
}

void Personnage::avancer(){
    // check if the game is paused
    if(m_pause == false){
        // move the character
        if(m_isMovingLeft == true){
            m_moved_left += m_vitesse/2;
        }
        if(m_isMovingRight == true){
            m_moved_left -= m_vitesse/2;
        }
        if(m_isMovingUp == true){
            m_moved_up += m_vitesse;
            // stop the jump if the deplacement is ended ( the character is to high)
            if(m_moved_up > 6.0){
                m_isMovingUp = false;
            }
        }
        // if not jumping and in the air, the character fall
        if(m_isMovingUp == false){
            m_moved_up = std::max(0.0f, m_moved_up-m_vitesse);
        }
        //check if deplacement ended and change the boolean if needed
        if( 0.99<m_moved_left && m_moved_left<1.01){
            m_isMovingLeft = false;
            m_isMovingRight = false;
        }
        if( -1.01<m_moved_left && m_moved_left<-0.99){
            m_isMovingLeft = false;
            m_isMovingRight = false;
        }
        if(-0.01<m_moved_left && m_moved_left<0.01){
            m_isMovingLeft = false;
            m_isMovingRight = false;
        }
        // increment distance
        m_distance += m_vitesse;
        //reduce countdown
        m_time_danger = std::max(m_time_danger-1, 0);
        m_immortal= std::max(m_immortal-1, 0);
        // End danger state if countdown is finished
        if (m_time_danger<0.5){
            m_danger = false;
        }
        // increment score
        m_score += m_vitesse;
    }
}


void Personnage::colisionObstacle(){
    // if the character is in danger kill it, else put it in danger state
    if(m_danger == true){
        kill();
    }
    else{
        m_time_danger = 1000;
        m_danger = true;
    }
}


void Personnage::colisionPiece(){
    // increment score
    upscore(100);
}

void Personnage::initialise(){
    // initialise character variables values
    m_vitesse = 0.1;
    m_score = 0;
    m_moon_number =1;

    m_distance = 0;
    m_moved_left = 0;
    m_moved_up = 0;
    m_isMovingLeft = false;
    m_isMovingRight = false;
    m_isMovingUp = false;

    m_danger = true;
    m_time_danger = 50;
    m_alive = true;
}

void Personnage::kill() {
    // if the character ain't immortal, kill it by putting bollean m_alive to false
    if(m_immortal < 1){
        m_alive=false;
        }
    }