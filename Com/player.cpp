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
Sprite(graphics, "/Users/jonahglick/Documents/Com/rifelman_final_96x96.png", 0, 0, 96, 96, 300, 300),
_dx(0),
_dy(0),
_mx(0),
_my(0)
{
    //graphics.loadImage("/Users/jonahglick/Documents/Com/rifelman_final_96x96.png");
    
    
    this->_fov = Sprite(graphics, "/Users/jonahglick/Documents/Com/view_direction.png", 0, 0, 96, 128, 300, 300);
    
    
    
    //this->setupAnimations();
    //this->playAnimation("IdleUp");
}
/*
void Player::setupAnimations() {
 
    this->addAnimation(5, 32, 0, "RunDown", 16, 23, Vector2(0, 0));
    this->addAnimation(5, 32, 23, "RunUp", 16, 23, Vector2(0, 0));
    this->addAnimation(1, 32, 0, "IdleDown", 16, 23, Vector2(0, 0));
    this->addAnimation(1, 32, 23, "IdleUp", 16, 23, Vector2(0, 0));
    
    this->addAnimation(3, 16, 46, "RunRight", 16, 24, Vector2(0, 0));
    this->addAnimation(3, 16, 70, "RunLeft", 16, 24, Vector2(0, 0));
    this->addAnimation(1, 16, 46, "IdleRight", 16, 24, Vector2(0, 0));
    this->addAnimation(1, 16, 70, "IdleLeft", 16, 24, Vector2(0, 0));
}
*/
/*
void Player::animationDone(std::string currentAnimation) {}
*/
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
    //this->playAnimation("RunUp");
    //this->_facing = UP;
}

void Player::moveDown() {
    this->_dy = player_constants::WALK_SPEED;
    this->_dx = 0.0f;
    //this->playAnimation("RunDown");
    //this->_facing = DOWN;
}

void Player::moveRight() {
    this->_dx = player_constants::WALK_SPEED;
    this->_dy = 0.0f;
    //this->playAnimation("RunRight");
    //this->_facing = RIGHT;
}

void Player::moveLeft() {
    this->_dx = -player_constants::WALK_SPEED;
    this->_dy = 0.0f;
    //this->playAnimation("RunLeft");
    //this->_facing = LEFT;
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
    /*
    switch (this->_facing) {
        case DOWN:
            this->playAnimation("IdleDown");
            break;
        case UP:
            this->playAnimation("IdleUp");
            break;
        case RIGHT:
            this->playAnimation("IdleRight");
            break;
        case LEFT:
            this->playAnimation("IdleLeft");
            break;
    }
    */
    
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
    //std::cout << float(this->_x) << " " << float(this->_y) << std::endl;
    //this->_fov.draw(graphics, this->_x, this->_y-32);
    float angle = 0.0;
    
    if(this->_my < (this->_y + 48)) {
        angle = -std::atan((this->_mx - (this->_x + 48))/(this->_my - (this->_y + 48))) * 180 / 3.14159;
    }
    else {
        angle = 180-std::atan((this->_mx - (this->_x + 48))/(this->_my - (this->_y + 48))) * 180 / 3.14159;
    }
    //float angle = -std::atan((this->_mx - (this->_x + 48))/(this->_my - (this->_y + 48))) * 180 / 3.14159;
    
    this->_fov.drawAngle(graphics, this->_x, this->_y-32, angle);
    Sprite::draw(graphics, this->_x, this->_y);
    //Sprite::drawAngle(graphics, this->_x, this->_y);
}
