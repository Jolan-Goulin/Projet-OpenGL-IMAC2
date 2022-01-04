#include <vector>
#include <iostream>
#include "glimac/GameObjects.hpp"
#include <fstream>
#include <typeinfo>
#include <cassert>

namespace glimac {

void Personnage::saut(){
    // if the character is on the ground, activate jumping state
    if( m_moved_up <0.01){
        m_isMovingUp = true;
    }
}

void Personnage::avancer(){
    // check if the game is paused
    if(m_pause == false){
        // move the character
        if(m_isMovingLeft == true){
            m_moved_left += m_vitesse_side/2;
        }
        if(m_isMovingRight == true){
            m_moved_left -= m_vitesse_side/2;
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
        m_score += m_vitesse*10;
        m_vitesse = m_vitesse*1.0001;
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
    upscore(1000);
}

void Personnage::initialise(){
    // initialise character variables values
    m_vitesse_side = 0.1;
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
    write_highscore();
    }

int Personnage::highscore(){
    // Open highscore file
    std::string const nomFichier("./../../assets/docs/highscore.txt");
    std::ifstream highscore_file(nomFichier.c_str());
    try{
    if (!highscore_file) {
        std::cerr << "Couldn't open highscore file. An error we could have dodged if you were more humble" << std::endl;
        throw std::exception_ptr();
    }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    // get highscore from file
    int score;
    highscore_file >> score;
    return score;
}

void Personnage::write_highscore(){
    // check if score is new highscore
    if (m_score>highscore()){
        //all create/override highscore file
        std::string const nomFichier("./../../assets/docs/highscore.txt");
        std::ofstream highscore_file(nomFichier.c_str());
        try{
        if (!highscore_file) {
            std::cerr << "Couldn't open highscore file. An error we could have dodged if you were more humble" << std::endl;
            throw std::exception_ptr();
        }
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
        // write highscore in file
        assert(typeid(m_score).name()==typeid(int).name());
        highscore_file << m_score << std::endl;
    }
}

void Personnage::save(const int &map_start, const std::vector<int> &map){
    // check if character is alive (to assure we don't save an ended game if the player confuse imput keys)
    if (m_alive){
        // open save file
        std::string const nomFichier("./../../assets/docs/save.txt");
        std::ofstream save(nomFichier.c_str());
        try{
        if (!save) {
            std::cerr << "Couldn't open highscore file. An error we could have dodged if you were more humble" << std::endl;
            throw std::invalid_argument("invalid texture path");
        }
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
        // write all relevant game info in save file
        save << m_vitesse << std::endl;
        save << m_score << std::endl;
        save << m_moon_number << std::endl;
        save << m_distance << std::endl;
        save << m_moved_left << std::endl;
        save << m_moved_up << std::endl;
        save << m_isMovingLeft << std::endl;
        save << m_isMovingRight << std::endl;
        save << m_isMovingUp << std::endl;
        save << m_danger << std::endl;
        save << m_time_danger << std::endl;
        save << m_immortal << std::endl;
        save << map_start << std::endl;
        for(int i=0;i<map.size();i++){
            save << map[i] << std::endl;
        }
    }
}

void Personnage::load(int &map_start, std::vector<int> &map){
    // open save file
    std::string const nomFichier("./../../assets/docs/save.txt");
    std::ifstream save(nomFichier.c_str());
    try{
    if (!save) {
        std::cerr << "Couldn't open highscore file. An error we could have dodged if you were more humble" << std::endl;
        throw std::invalid_argument("invalid texture path");
    }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    // get all relevants infos from save file
    save >> m_vitesse;
    save >> m_score;
    save >> m_moon_number;
    save >> m_distance;
    save >> m_moved_left;
    save >> m_moved_up;
    save >> m_isMovingLeft;
    save >> m_isMovingRight;
    save >> m_isMovingUp;
    save >> m_danger;
    save >> m_time_danger;
    save >> m_immortal;
    m_pause = true;
    m_alive = true;
    save >> map_start;
    for(int i=0;i<map.size();i++){
        save >> map[i];
    }
}

}