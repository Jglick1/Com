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
//Sprite(graphics, "/Users/jonahglick/Documents/Com/riflemant_30x40.png", 0, 0, 30, 40, 500, 400),
Sprite(graphics, "/Users/jonahglick/Documents/Com/rifleman.png", 0, 0, 16, 20, 0, 0),
_dx(0),
_dy(0),
_angle(0.0),
_playerAngle(0.0),
_staticdx(0.0),
_staticdy(0.0),
_staticx(0),
_staticy(0),
_state(STILL),
_destinationx(0),
_destinationy(0),
_destinationDirection(NONE)
{
    //graphics.loadImage("/Users/jonahglick/Documents/Com/rifelman_final_96x96.png");
    
    this->_x = 0;
    this->_y = 0;
    
    //this->_staticx = 500;
    //this->_staticy = 400;
    
    Rectangle rec(0,4,16,16);
    
    this->_collisionRect = rec;
    
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

void Unit::stopMovingParallax() {
    this->_dx = 0.0;
    this->_dy = 0.0;
}

void Unit::moveUpRightParallax() {                 //should have said forwardRight and backwardRight instead
    this->_dx = -player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421;
    this->_dy = -player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421 - player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421;
    //this->_unit.moveForwardRightParallax();
}
void Unit::moveUpLeftParallax() {
    this->_dx = -player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421 + -player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421;
    this->_dy = -player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421;
    //this->_unit.moveForwardRightParallax();
}
void Unit::moveDownRightParallax() {
    this->_dx = player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421;
    this->_dy = player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421 + -player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421;
}
void Unit::moveDownLeftParallax() {
    this->_dx = player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421 + -player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421;
    this->_dy = player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421;
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

void Unit::moveToNextPosition() {
    this->_unitMovementOrders.erase(this->_unitMovementOrders.begin());//erase last order
    if(this->_unitMovementOrders.size() > 0) {
        moveToPosition(this->_unitMovementOrders[0].x, this->_unitMovementOrders[0].y);
    }
}

void Unit::moveToPosition(int posX, int posY) {
    
    
    //if there is a (bullet) collision when pointed towards destination position
        //find nearest corner without collision closest to destination location
    
        //find the next corner (has either the same x or y) that is closest to the destination
    
    //position vector
        //decide the best path to take and fill the positon array
    
    
    
    
    
    
    
    
    stopMoving();
    
    //find the angle the unit will need to be at
    //float xdiff = posX - (this->_staticx+15);
    //float ydiff = posY - (this->_staticy+30);
    float xdiff = posX - (this->_staticx);
    float ydiff = posY - (this->_staticy+4);
    float angle = 0.0;
    
    if(ydiff < 0) {
        angle = -std::atan(xdiff/ydiff);
    }
    else {
        angle = -std::atan(xdiff/ydiff) - 3.14159;
    }
    
    if((xdiff == 0) && (ydiff == 0)) {
        this->_destinationAngle = this->_angle;
    }
    else {
        this->_destinationAngle = angle * 180/3.14159;
    }
    
    if(this->_destinationAngle > 180) {
        this->_destinationAngle -= 360;
    }
    else if(this->_destinationAngle < -180) {
        this->_destinationAngle += 360;
    }

    
    
    //this->_angle = angle * 180/3.14159;
    
    this->_destinationx = posX;
    this->_destinationy = posY;
    
    double tempAngle = this->_angle;
    
    if(this->_angle < 0.0 && this->_destinationAngle > 0.0) {
        tempAngle = this->_angle + 360;
    }
    else if (this->_angle > 0.0 && this->_destinationAngle < 0.0) {
        tempAngle = this->_angle - 360;
    }
    
    if(tempAngle >= this->_destinationAngle) {
        this->_state = CHANGE_ANGLE_DOWN;
    }
    else {
        this->_state = CHANGE_ANGLE_UP;         //TODO. decided whether to go up or down
    }

    this->_angle = tempAngle;
    
    
    
    
    if(this->_destinationx >= this->_staticx && this->_destinationy >= this->_staticy) {
        this->_destinationDirection = DOWNRIGHT;
        
        if(std::round(this->_destinationy) == std::round(this->_staticy+4)) {
            this->_destinationDirection = RIGHT;
            printf("right\n");
        }
        
        else if(std::round(this->_destinationx) == std::round(this->_staticx)) {
            this->_destinationDirection = DOWN;
            printf("down\n");
        }
        
        printf("DOWNRIGHT\n");
    }
    else if(this->_destinationx <= this->_staticx && this->_destinationy >= this->_staticy) {
        this->_destinationDirection = DOWNLEFT;
        
        
        if(std::round(this->_destinationy) == std::round(this->_staticy+4)) {
            this->_destinationDirection = LEFT;
            printf("left\n");
        }
        
        else if(std::round(this->_destinationx) == std::round(this->_staticx)) {
            this->_destinationDirection = DOWN;
            printf("down\n");
        }
        
        //printf("%f, %f\n", std::round(this->_destinationy), std::round(this->_staticy));
        printf("DOWNLEFT\n");
    }
    else if(this->_destinationx >= this->_staticx && this->_destinationy <= this->_staticy) {
        this->_destinationDirection = UPRIGHT;
        
        
        if(std::round(this->_destinationy) == std::round(this->_staticy+4)) {
            this->_destinationDirection = RIGHT;
            printf("right\n");
        }
        
        else if(std::round(this->_destinationx) == std::round(this->_staticx)) {
            this->_destinationDirection = UP;
            printf("up\n");
        }
        
        
        printf("UPRIGHT\n");
    }
    else if(this->_destinationx <= this->_staticx && this->_destinationy <= this->_staticy) {
        this->_destinationDirection = UPLEFT;
        
        
        if(std::round(this->_destinationy) == std::round(this->_staticy+4)) {
            this->_destinationDirection = LEFT;
            printf("left\n");
        }
    
        else if(std::round(this->_destinationx) == std::round(this->_staticx)) {
            this->_destinationDirection = UP;
            printf("up\n");
        }
        
        printf("UPLEFT\n");
    }
    
    //printf("angle: %f, destAngle: %f\n", this->_angle, this->_destinationAngle);
    
    //std::cout << xdiff << " " << ydiff << std::endl;
    //std::cout << this->_angle << std::endl;
    
    
    
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
    
    //int posx = std::round(this->_x+this->_staticx);
    //int posy = std::round(this->_y+this->_staticy);
    
    float posx = this->_x+this->_staticx;
    float posy = this->_y+this->_staticy;
    
    
    
    //std::cout << this->_staticdx << " " << this->_staticdy << std::endl;
    
    //Sprite::drawAngle(graphics, 640-15 + ( (this->_x+this->_staticx)-640+15)* std::cos(this->_playerAngle*3.14159/180) - ((this->_y+this->_staticy)-400+30) * std::sin(this->_playerAngle*3.14159/180), 400-30 + ( ((this->_y+this->_staticy)-400+30) * std::cos(this->_playerAngle*3.14159/180) + ((this->_x+this->_staticx)-640+15) * std::sin(this->_playerAngle*3.14159/180)), this->_angle + this->_playerAngle);
    
    
    
//R(a-b)+b
    
    
    Sprite::drawAngle(graphics, 640-8 + ( (posx)-640+8)* std::cos(this->_playerAngle*3.14159/180) - ((posy)-400+12) * std::sin(this->_playerAngle*3.14159/180), 400-12 + ( ((posy)-400+12) * std::cos(this->_playerAngle*3.14159/180) + ((posx)-640+8) * std::sin(this->_playerAngle*3.14159/180)), this->_angle + this->_playerAngle);
    
    
    //Sprite::drawAngle(graphics, 640-8 + ( (posx)-640+8)* std::cos(this->_playerAngle*3.14159/180) - ((posy)-400+16) * std::sin(this->_playerAngle*3.14159/180), 400-16 + ( ((posy)-400+16) * std::cos(this->_playerAngle*3.14159/180) + ((posx)-640+8) * std::sin(this->_playerAngle*3.14159/180)), this->_angle + this->_playerAngle);
    
    
    //graphics.drawLine(posx, posy, 0, 0);
    
    
    //std::cout << "x: " << this->_x << " y : " << this->_y << " sx: " << this->_staticx << " sy: " << this->_staticy << std::endl;
    
    //std::cout << "dx: " << this->_dx << " dy : " << this->_dy << " dsx: " << this->_staticdx << " dsy: " << this->_staticdy << std::endl;

    
    //this->stopMoving();
    
    //graphics.drawLine(this->_staticx+15, this->_staticy+30, this->_staticx+15 + 100*std::sin(this->_angle*3.14159/180), this->_staticy+30 - 100*std::cos(this->_angle*3.14159/180) );
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //graphics.drawLine(this->_staticx + this->_x+15, this->_staticy + this->_y+30, this->_staticx + this->_x+15 + 100*std::sin(this->_angle*3.14159/180), this->_staticy + this->_y+30 - 100*std::cos(this->_angle*3.14159/180) );
    

    
    

    
    

}

void Unit::update(float elapsedTime, float playerAngle) {
    //movement


    this->_y += this->_dy * elapsedTime;
    this->_x += this->_dx * elapsedTime;
    
    this->_staticy += this->_staticdy * elapsedTime;
    this->_staticx += this->_staticdx * elapsedTime;
    
    //this->_collisionRect.update(elapsedTime, this->_dx+this->_staticdx, this->_dy+this->_staticdy);
    this->_collisionRect.setXY(this->_x+this->_staticx, this->_y+this->_staticy + 4);
    
    Sprite::update();
    
    
}

void Unit::setPlayerAngle(float playerAngle) {
    this->_playerAngle = playerAngle;
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

float Unit::getStaticX() {
    return this->_staticx;
}
float Unit::getStaticY() {
    return this->_staticy;
}

void Unit::handleTileCollisions(std::vector<Rectangle> &others, float elapsedTime) { //other are the level's collision rects

    
    Rectangle playerRec = Rectangle(632, 392, 16, 16);

    for (int i = 0; i < others.size(); i++) {
        sides::Side collisionSide = playerRec.getCollisionSide(others.at(i));
        
        if (collisionSide != sides::NONE) {
            switch (collisionSide) {
                case sides::TOP:                    //top refers to player's box
                    this->changeY(-(others.at(i).getStartY() + others.at(i).getHeight()) + 400 - 8); //this->_staticy);
                    this->_dy = 0.0f;
                    
                    break;
                case sides::BOTTOM:
                    this->changeY(-(others.at(i).getStartY()) + 400-12 + 20);// + this->_staticy);
                    this->_dy = 0.0f;
                    
                    break;
                case sides::LEFT:
                    this->changeX((-others.at(i).getStartX()) + 640 - 8 - others.at(i).getWidth());// + this->_staticx);
                    this->_dx = 0.0f;

                    break;
                case sides::RIGHT:
                    this->changeX((-others.at(i).getStartX()) + 640 + 8);// + this->_staticx);
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

State Unit::getState() {
    return this->_state;
}

float Unit::getAngle() {
    return this->_angle;
}

float Unit::getPlayerAngle() {
    return this->_playerAngle;
}

void Unit::handleMovement() {
    
    //printf("staticx: %f, staticy: %f\n", this->_staticx, this->_staticy);

    
    switch(this->_state) {
        case CHANGE_ANGLE_UP:
            if(this->_angle < this->_destinationAngle) {
                this->_angle+=5;
            }
            else {
                this->_angle = this->_destinationAngle;
                this->_state = MOVE_FORWARD;
            }
            break;
        case CHANGE_ANGLE_DOWN:
            if(this->_angle > this->_destinationAngle) {
                this->_angle-=5;
            }
            else {
                this->_angle = this->_destinationAngle;
                this->_state = MOVE_FORWARD;
            }
            break;
        case MOVE_FORWARD:
            
            switch(this->_destinationDirection) {
                case UPRIGHT:
                    if((this->_staticx >= this->_destinationx) && (this->_staticy+4 <= this->_destinationy)) {
                        stopMoving();
                        this->_staticx = this->_destinationx - 0;
                        this->_staticy = this->_destinationy - 4;
                        //this->_angle = 0.0;
                        this->_state = STILL;
                        moveToNextPosition();
                    }
                    else {
                        moveForward();
                    }
                    break;
                case UPLEFT:
                    if((this->_staticx <= this->_destinationx) && (this->_staticy+4 <= this->_destinationy)) {
                        stopMoving();
                        this->_staticx = this->_destinationx - 0;
                        this->_staticy = this->_destinationy - 4;
                        //this->_angle = 0.0;
                        this->_state = STILL;
                        moveToNextPosition();
                    }
                    else {
                        moveForward();
                    }
                    break;
                case DOWNRIGHT:
                    if((this->_staticx >= this->_destinationx) && (this->_staticy+4 >= this->_destinationy)) {
                        stopMoving();
                        this->_staticx = this->_destinationx - 0;
                        this->_staticy = this->_destinationy - 4;
                        //this->_angle = 0.0;
                        this->_state = STILL;
                        moveToNextPosition();
                    }
                    else {
                        moveForward();
                    }
                    break;
                case DOWNLEFT:
                    if((this->_staticx <= this->_destinationx) && (this->_staticy+4 >= this->_destinationy)) {
                        stopMoving();
                        this->_staticx = this->_destinationx - 0;
                        this->_staticy = this->_destinationy - 4;
                        //this->_angle = 0.0;
                        this->_state = STILL;
                        moveToNextPosition();
                    }
                    else {
                        moveForward();
                    }
                    break;
                case DOWN:
                    if(this->_staticy+4 >= this->_destinationy) {
                        stopMoving();
                        this->_staticx = this->_destinationx - 0;
                        this->_staticy = this->_destinationy - 4;
                        this->_state = STILL;
                        moveToNextPosition();
                    }
                    else {
                        moveForward();
                    }
                    break;
                case UP:
                    if(this->_staticy+4 <= this->_destinationy) {
                        stopMoving();
                        this->_staticx = this->_destinationx - 0;
                        this->_staticy = this->_destinationy - 4;
                        this->_state = STILL;
                        moveToNextPosition();
                    }
                    else {
                        moveForward();
                    }
                    break;
                case RIGHT:
                    if(this->_staticx >= this->_destinationx) {
                        stopMoving();
                        this->_staticx = this->_destinationx - 0;
                        this->_staticy = this->_destinationy - 4;
                        //this->_angle = 0.0;
                        this->_state = STILL;
                        moveToNextPosition();
                    }
                    else {
                        moveForward();
                    }
                    break;
                case LEFT:
                    if(this->_staticx <= this->_destinationx) {
                        stopMoving();
                        this->_staticx = this->_destinationx - 0;
                        this->_staticy = this->_destinationy - 4;
                        //this->_angle = 0.0;
                        this->_state = STILL;
                        moveToNextPosition();
                    }
                    else {
                        moveForward();
                    }
                    break;
                    
            }
            
            //printf("sx: %f sy: %f dx: %f dy: %f\n", this->_staticx, this->_staticy, this->_destinationx, this->_destinationy);
            
            
            /*
            if(!((std::floor(this->_staticx) == std::floor(this->_destinationx)) && (std::floor(this->_staticy) == std::floor(this->_destinationy)))) { // don't need y because angle is already defined
                
                moveForward();
            }
            else {
                stopMoving();
                this->_state = STILL;
            }
             */
            break;
            
        case STILL:
            //stopMoving();
            break;
    }
    
}

void Unit::addToMovementOrders(std::vector<Vector2> pos) {
    for(Vector2 i : pos) {
        this->_unitMovementOrders.push_back(i);
    }
}

void Unit::addToMovementOrders(Vector2 pos) {

    this->_unitMovementOrders.push_back(pos);
    
}

void Unit::printMovementOrders() {
    for(Vector2 i : this->_unitMovementOrders) {
        printf("%d, %d\n", i.x, i.y);
    }
}

Rectangle Unit::getCollisionRect() {
    return this->_collisionRect;
}

