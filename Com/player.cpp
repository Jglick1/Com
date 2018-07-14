//
//  player.cpp
//  Com
//
//  Created by Jonah Glick on 6/20/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "player.hpp"
#include "graphics.hpp"
#include <iostream>
#include <math.h>


namespace player_constants {
    const float WALK_SPEED = 0.2f;
}

Player::Player() {}

//Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY)

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
//AnimatedSprite(graphics, "/Users/jonahglick/Documents/Io/Link1.png", 171, 129, 16, 22, spawnPoint.x, spawnPoint.y, 75),
//Sprite(graphics, "/Users/jonahglick/Documents/Com/riflemant_30x40.png", 0, 0, 30, 40, 625, 660),
//Sprite(graphics, "/Users/jonahglick/Documents/Com/riflemant_30x40.png", 0, 0, 30, 40, 625, 375),
Sprite(graphics, "/Users/jonahglick/Documents/Com/rifleman.png", 0, 0, 16, 20, 625, 375),
_dx(0),
_dy(0),
_mx(0),
_my(0)
{
    //graphics.loadImage("/Users/jonahglick/Documents/Com/rifelman_final_96x96.png");
    
    
    //this->_fov = Sprite(graphics, "/Users/jonahglick/Documents/Com/view_direction.png", 0, 0, 96, 128, 300, 300);
    
    
    
    //this->setupAnimations();
    //this->playAnimation("IdleUp");
}

const float Player::getX() const {
    return this->_x;
}

const float Player::getY() const {
    return this->_y;
}

const float Player::getMX() const {
    return this->_mx;
}

const float Player::getMY() const {
    return this->_my;
}

/*
const float Player::getDX() const {
    return this->_dx;
}

const float Player::getDY() const {
    return this->_dy;
}
*/

void Player::moveUp() {
    this->_dy = -player_constants::WALK_SPEED;
    this->_dx = 0.0f;

}

void Player::moveDown() {
    this->_dy = player_constants::WALK_SPEED;
    this->_dx = 0.0f;

}

void Player::moveRight() {
    this->_dx = player_constants::WALK_SPEED;
    this->_dy = 0.0f;

}

void Player::moveLeft() {
    this->_dx = -player_constants::WALK_SPEED;
    this->_dy = 0.0f;

}


void Player::moveUpAndRight() {
    this->_dx = player_constants::WALK_SPEED/1.41421;
    this->_dy = -player_constants::WALK_SPEED/1.41421;
}

void Player::moveUpAndLeft() {
    this->_dx = -player_constants::WALK_SPEED/1.41421;
    this->_dy = -player_constants::WALK_SPEED/1.41421;
}

void Player::moveDownAndRight() {
    this->_dx = player_constants::WALK_SPEED/1.41421;
    this->_dy = player_constants::WALK_SPEED/1.41421;
}

void Player::moveDownAndLeft() {
    this->_dx = -player_constants::WALK_SPEED/1.41421;
    this->_dy = player_constants::WALK_SPEED/1.41421;
}


void Player::stopMoving() {
    this->_dy = 0.0f;
    this->_dx = 0.0f;
}
    

/*
void Player::handleTileCollisions(std::vector<Rectangle> &others) {
    for (int i = 0; i < others.size(); i++) {
        sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
        if (collisionSide != sides::NONE) {
            switch (collisionSide) {
                case sides::TOP:
                    this->_y = others.at(i).getBottom() + 1;
                    this->_dy = 0;
                    break;
                case sides::BOTTOM:
                    this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
                    this->_dy = 0;
                    break;
                case sides::LEFT:
                    this->_x = others.at(i).getRight() + 1;
                    this->_dx = 0;
                    break;
                case sides::RIGHT:
                    this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
                    this->_dx = 0;
            }
        }
    }
}
*/

void Player::handleTileCollisions(float elapsedTime) {
    //this->_x -= std::round(elapsedTime * this->_dx);
    //this->_y -= std::round(elapsedTime * this->_dy);
    
}




/*
void Player::handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics) {
    
    for(int i = 0; i<others.size(); i++) {
        std::string level_tileset_path = "/Users/jonahglick/Documents/Io/" + others.at(i).getDestination();
        level = Level(level_tileset_path, graphics);
        std::cout << level_tileset_path << std::endl;
        this->_x = level.getPlayerSpawnPoint().x;
        this->_y = level.getPlayerSpawnPoint().y;
    }
    
}
*/

void Player::update(float elapsedTime, int mx, int my) {
    //std::cout << "dx: " << this->_dx << std::endl;
    this->_y += this->_dy * elapsedTime;
    this->_x += this->_dx * elapsedTime;
    
    //this->_mx += deltaX;
    //this->_my += deltaY;
    
    //SDL_GetMouseState(&this->_mx, &this->_my);
    //std::cout << this->_mx << " " << this->_my << std::endl;
    //std::cout << mx << " " << my << std::endl;
    this->_mx = mx;
    this->_my = my;

    Sprite::update();
    
}

void Player::draw(Graphics &graphics) {
    /*
    float angle = 0.0;
    
    if(this->_my < (this->_y + 48)) {
        angle = -std::atan((this->_mx - (this->_x + 48))/(this->_my - (this->_y + 48))) * 180 / 3.14159;
    }
    else {
        angle = 180-std::atan((this->_mx - (this->_x + 48))/(this->_my - (this->_y + 48))) * 180 / 3.14159;
    }
    
    this->_fov.drawAngle(graphics, this->_x, this->_y-32, angle);
    */
    
    Sprite::draw(graphics, this->_x, this->_y);
    graphics.drawRect(this->_x, this->_y+10, 30, 30);
    //Sprite::drawAngle(graphics, this->_x, this->_y);
    
    //graphics.drawLine(640,400,640,0);

}
