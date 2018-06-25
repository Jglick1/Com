//
//  unit.cpp
//  Com
//
//  Created by Jonah Glick on 6/23/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "unit.hpp"
#include "graphics.hpp"
#include <iostream>
#include <math.h>


namespace player_constants {
    const float WALK_SPEED = 0.2f;
}

Unit::Unit() {}

Unit::Unit(Graphics &graphics, Vector2 spawnPoint) :
Sprite(graphics, "/Users/jonahglick/Documents/Com/riflemant_30x40.png", 0, 0, 30, 40, 400 - 15, 600 - 30),
_dx(0),
_dy(0),
_angle(0.0),
_playerAngle(0.0)
{
    //graphics.loadImage("/Users/jonahglick/Documents/Com/rifelman_final_96x96.png");
    
    
    //this->_fov = Sprite(graphics, "/Users/jonahglick/Documents/Com/view_direction.png", 0, 0, 96, 128, 600, 600);

}


/*
void Unit::moveForward() {
    this->_dx = player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
    this->_dy = -player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
}

void Unit::moveBackward() {
    this->_dx = -player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
    this->_dy = player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
}

void Unit::moveRight() {
    this->_dx = player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
    this->_dy = player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
}

void Unit::moveLeft() {
    this->_dx = -player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
    this->_dy = -player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
}
 */

void Unit::moveForward() {
    this->_dx = 0.0;
    this->_dy = -player_constants::WALK_SPEED;
}

void Unit::moveBackward() {
    this->_dx = 0.0;
    this->_dy = player_constants::WALK_SPEED;
}

void Unit::moveRight() {
    this->_dx = player_constants::WALK_SPEED;
    this->_dy =0.0;
}

void Unit::moveLeft() {
    this->_dx = -player_constants::WALK_SPEED;
    this->_dy = 0.0;
}

void Unit::stopMoving() {
    this->_dx = 0.0;
    this->_dy = 0.0;
}

void Unit::draw(Graphics &graphics) {
    //Sprite::draw(graphics, this->_x, this->_y);
    
    //float d = std::sqrt(std::pow(this->_x - 640 + 15,2) + std::pow(this->_y - 400 + 30,2));
    
    //std::cout << this->_x << " " << this->_y << std::endl;
    
    //Sprite::drawAngle(graphics, this->_x, this->_y, this->_angle + this->_playerAngle);
    
    
    //Sprite::drawAngle(graphics, this->_x + std::round(d - d * std::cos(this->_playerAngle*3.14159/180)), this->_y - std::round(d * std::sin(this->_playerAngle*3.14159/180)), this->_angle + this->_playerAngle);
    

    Sprite::drawAngle(graphics,640 + ( this->_x-640+15)* std::cos(this->_playerAngle*3.14159/180) - (this->_y-400+30) * std::sin(this->_playerAngle*3.14159/180), 400 + ( (this->_y-400+30) * std::cos(this->_playerAngle*3.14159/180) + (this->_x-640+15) * std::sin(this->_playerAngle*3.14159/180)), this->_angle + this->_playerAngle);

}

void Unit::update(float elapsedTime, float playerAngle) {
    //movement
    this->_y += std::round(this->_dy * elapsedTime);
    this->_x += std::round(this->_dx * elapsedTime);
    
    this->_playerAngle = playerAngle;


    
    //this->_angle++;
    
    Sprite::update();
    
}
