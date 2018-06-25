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
Sprite(graphics, "/Users/jonahglick/Documents/Com/riflemant_30x40.png", 0, 0, 30, 40, 300, 500),
_dx(0),
_dy(0),
_angle(0.0)
{
    //graphics.loadImage("/Users/jonahglick/Documents/Com/rifelman_final_96x96.png");
    
    
    //this->_fov = Sprite(graphics, "/Users/jonahglick/Documents/Com/view_direction.png", 0, 0, 96, 128, 600, 600);

}

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

void Unit::draw(Graphics &graphics) {
    //Sprite::draw(graphics, this->_x, this->_y);
    Sprite::drawAngle(graphics, this->_x, this->_y, this->_angle);
}

void Unit::update(float elapsedTime) {

    this->_y += this->_dy * elapsedTime;
    this->_x += this->_dx * elapsedTime;
    
    this->_angle++;
    
    Sprite::update();
    
}
