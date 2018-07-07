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
#include <vector>


namespace player_constants {
    const float WALK_SPEED = 0.2f;
}

Unit::Unit() {}

Unit::Unit(Graphics &graphics, Vector2 spawnPoint) :
Sprite(graphics, "/Users/jonahglick/Documents/Com/riflemant_30x40.png", 0, 0, 30, 40, 500, 400),
_dx(0),
_dy(0),
_angle(90.0),
_playerAngle(0.0),
_staticdx(0.0),
_staticdy(0.0),
_staticx(500),
_staticy(400),
_state(STILL)
{
    //graphics.loadImage("/Users/jonahglick/Documents/Com/rifelman_final_96x96.png");
    
    this->_x = 0;
    this->_y = 0;
    
    //this->_staticx = 500;
    //this->_staticy = 400;
    
    //this->_fov = Sprite(graphics, "/Users/jonahglick/Documents/Com/view_direction.png", 0, 0, 96, 128, 600, 600);

}



void Unit::moveForwardParallax() {
    this->_dx = -player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180);
    this->_dy = -player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180);
}

void Unit::moveBackwardParallax() {
    this->_dx = player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180);
    this->_dy = player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180);
}


void Unit::moveRightParallax() {
    this->_dx = player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180);
    this->_dy = -player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180);
}

void Unit::moveLeftParallax() {
    this->_dx = -player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180);
    this->_dy = player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180);
}

void Unit::setDXDY(float dx, float dy) {
    this->_dx = dx;
    this->_dy = dy;
}


/*
void Unit::moveForward() {
    this->_dx += 0.0;
    this->_dy += -player_constants::WALK_SPEED;
}

void Unit::moveBackward() {
    this->_dx += 0.0;
    this->_dy += player_constants::WALK_SPEED;
}

void Unit::moveRight() {
    this->_dx += player_constants::WALK_SPEED;
    this->_dy += 0.0;
}

void Unit::moveLeft() {
    this->_dx += -player_constants::WALK_SPEED;
    this->_dy += 0.0;
}


void Unit::stopMoving() {
    this->_dx = 0.0;
    this->_dy = 0.0;
}
 */

void Unit::moveForward() {
    this->_staticdx = player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
    this->_staticdy = -player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
}

void Unit::moveBackward() {
    this->_staticdx = 0.0;
    this->_staticdy = player_constants::WALK_SPEED;
}

void Unit::moveRight() {
    this->_staticdx = player_constants::WALK_SPEED;
    this->_staticdy = 0.0;
}

void Unit::moveLeft() {
    this->_staticdx = -player_constants::WALK_SPEED;
    this->_staticdy = 0.0;
}


void Unit::stopMoving() {
    this->_staticdx = 0.0;
    this->_staticdy = 0.0;
}

/*
void Level::moveForward() {
    this->_dx = -player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
    this->_dy = -player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
}

void Level::moveBackward() {
    this->_dx = player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
    this->_dy = player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
}


void Level::moveRight() {
    this->_dx = player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
    this->_dy = -player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
}

void Level::moveLeft() {
    this->_dx = -player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
    this->_dy = player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
}
 */

void Unit::moveToPosition(int posX, int posY) {
    
}

void Unit::draw(Graphics &graphics) {
    //Sprite::draw(graphics, this->_x, this->_y);
    
    //float d = std::sqrt(std::pow(this->_x - 640 + 15,2) + std::pow(this->_y - 400 + 30,2));
    
    //std::cout << this->_x << " " << this->_y << std::endl;
    
    //Sprite::drawAngle(graphics, this->_x, this->_y, this->_angle + this->_playerAngle);
    
    
    //Sprite::drawAngle(graphics, this->_x + std::round(d - d * std::cos(this->_playerAngle*3.14159/180)), this->_y - std::round(d * std::sin(this->_playerAngle*3.14159/180)), this->_angle + this->_playerAngle);
    

    //Sprite::drawAngle(graphics, 640-15 + ( this->_x-640-15)* std::cos(this->_playerAngle*3.14159/180) - (this->_y-400+30) * std::sin(this->_playerAngle*3.14159/180), 400-30 + ( (this->_y-400+30) * std::cos(this->_playerAngle*3.14159/180) + (this->_x-640-15) * std::sin(this->_playerAngle*3.14159/180)), this->_angle + this->_playerAngle);
    
    
    //rotate point a around point b
    
    //R * (a-b) + b
    //R =   (   cos(theta), -sin(theta)    )
    //      (   sin(theta), cos(theta)     )
    
    //  (unit.staticx)  -   (player.staticx)
    //  (unit.staticy)      (player.staticy)
    

    
    Sprite::drawAngle(graphics, 640-15 + ( (this->_x+this->_staticx)-640+15)* std::cos(this->_playerAngle*3.14159/180) - ((this->_y+this->_staticy)-400+30) * std::sin(this->_playerAngle*3.14159/180), 400-30 + ( ((this->_y+this->_staticy)-400+30) * std::cos(this->_playerAngle*3.14159/180) + ((this->_x+this->_staticx)-640+15) * std::sin(this->_playerAngle*3.14159/180)), this->_angle + this->_playerAngle);
    
    
    //std::cout << "x: " << this->_x << " y : " << this->_y << " sx: " << this->_staticx << " sy: " << this->_staticy << std::endl;
    
    //this->stopMoving();

}

void Unit::update(float elapsedTime, float playerAngle) {
    //movement
    this->_y += std::round(this->_dy * elapsedTime);
    this->_x += std::round(this->_dx * elapsedTime);
    
    this->_staticy += std::round(this->_staticdy * elapsedTime);
    this->_staticx += std::round(this->_staticdx * elapsedTime);
    
    //this->_playerAngle = playerAngle;


    
    //this->_angle++;
    
    Sprite::update();
    
}

void Unit::setPlayerAngle(float playerAngle) {
    this->_playerAngle = playerAngle;
}

void Unit::handleTileCollisions() {
    this->_dx = 0.0;
    this->_dy = 0.0;
}

float Unit::getDX() {
    return this->_dx;
}

float Unit::getDY() {
    return this->_dy;
}


void Unit::setDX(float dx) {
    this->_dx = dx;
}
void Unit::setDY(float dy) {
    this->_dy = dy;
}

void Unit::setX(float x) {
    this->_x = x;
}
void Unit::setY(float y) {
    this->_y = y;
}

int Unit::getStartX() {
    return this->_startX;
}

int Unit::getStartY() {
    return this->_startY;
}

int Unit::getX() {
    return this->_x;
}

int Unit::getY() {
    return this->_y;
}

void Unit::handleTileCollisions(std::vector<Rectangle> &others, float elapsedTime) { //other are the level's collision rects

    
    Rectangle playerRec = Rectangle(625, 385, 30, 30);

    for (int i = 0; i < others.size(); i++) {
        sides::Side collisionSide = playerRec.getCollisionSide(others.at(i));
        
        if (collisionSide != sides::NONE) {
            switch (collisionSide) {
                case sides::TOP:                    //top refers to player's box
                    this->changeY(-(others.at(i).getStartY() + others.at(i).getHeight()) + 385); //this->_staticy);
                    this->_dy = 0.0f;
                    
                    break;
                case sides::BOTTOM:
                    this->changeY(-(others.at(i).getStartY()) + 385 + 30);// + this->_staticy);
                    this->_dy = 0.0f;
                    
                    break;
                case sides::LEFT:
                    this->changeX((-others.at(i).getStartX()) + 625 - others.at(i).getWidth());// + this->_staticx);
                    this->_dx = 0.0f;

                    break;
                case sides::RIGHT:
                    this->changeX((-others.at(i).getStartX()) + 625 + 30);// + this->_staticx);
                    this->_dx = 0.0f;
                    
                    break;
            }
        }
        
        
    }
}

void Unit::changeX(int newX) {
    this->_x = newX;
}

void Unit::changeY(int newY) {
    this->_y = newY;
}
