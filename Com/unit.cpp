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
Unit::~Unit() {}

Unit::Unit(Graphics &graphics, Vector2 spawnPoint, bool allie) :
//Sprite(graphics, "/Users/jonahglick/Documents/Com/riflemant_30x40.png", 0, 0, 30, 40, 500, 400),
//Sprite(graphics, "/Users/jonahglick/Documents/Com/rifleman_enemy.png", 0, 0, 16, 20, 0, 0),
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
_destinationDirection(NONE),
_distTraveled(0.0),
_distToNextPoint(1.0),
_isMoving(0),
_isAngleMoving(0),
_firstName("John"),
_lastName("Doe")
{

    if(allie) {
        this->_unitSprite = Sprite(graphics, "/Users/jonahglick/Documents/Com/rifleman.png", 0, 0, 16, 20, 0, 0);
    }
    else {
        this->_unitSprite = Sprite(graphics, "/Users/jonahglick/Documents/Com/rifleman_enemy.png", 0, 0, 16, 20, 0, 0);
    }
    
    this->_unitSelected = Sprite(graphics, "/Users/jonahglick/Documents/Com/unit_selected.png", 0, 0, 16, 16, 0, 0);
    
    
    //graphics.loadImage("/Users/jonahglick/Documents/Com/rifelman_final_96x96.png");
    
    //this->_x = 0;
    //this->_y = 0;
    
    //this->_staticx = 500;
    //this->_staticy = 400;
    
    this->_staticx = spawnPoint.x;
    this->_staticy = spawnPoint.y;
    
    //Rectangle rec(0,4,16,16);
    
    //this->_collisionRect = rec;
    
    //this->_fov = Sprite(graphics, "/Users/jonahglick/Documents/Com/view_direction.png", 0, 0, 96, 128, 600, 600);

    
    
    
    
    
    
    //find way to randomly assign first and last names
    
    
    
    
    
}



/*
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
*/




void Unit::moveToNextPosition() {
    this->_unitMovementOrders.erase(this->_unitMovementOrders.begin());//erase last order
    if(this->_unitMovementOrders.size() > 0) {
        moveToPosition(this->_unitMovementOrders[0].x, this->_unitMovementOrders[0].y);
    }
    else if (this->_unitAngleOrders.size() > 0){
        moveToAngle(this->_unitAngleOrders.at(0));
        this->_unitAngleOrders.erase(this->_unitAngleOrders.begin());//erase last order
    }
}

void Unit::moveToAngle(double angle) {
    
    this->_destinationAngle = angle;
    
    if(this->_angle >= this->_destinationAngle) {
        this->_state = CHANGE_ANGLE_DOWN;
    }
    else {
        this->_state = CHANGE_ANGLE_UP;         //TODO. decided whether to go up or down
    }
}

void Unit::moveToPosition(int posX, int posY) {
    
    
    //if there is a (bullet) collision when pointed towards destination position
        //find nearest corner without collision closest to destination location
    
        //find the next corner (has either the same x or y) that is closest to the destination
    
    //position vector
        //decide the best path to take and fill the positon array
    
    
    //stopMoving();
    
    
    
    
    
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
    
    
    
    
    
    
    //we want to change staticdx and staticdy
    
    double dx = posX - this->_staticx;
    double dy = posY - (this->_staticy+4);
    
    double distTotal = std::sqrt(dx*dx + dy*dy);
    
    this->_distToNextPoint = std::sqrt(dx*dx + dy*dy);
    
    
    this->_staticdx = (dx / distTotal) * player_constants::WALK_SPEED; //then *elapsedTime in update
    this->_staticdy = (dy / distTotal) * player_constants::WALK_SPEED;
    
    
    this->_distTraveled = 0;
    
    this->_isMoving = 1;
    
    this->_isAngleMoving = 1;
    
    /*
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
    */
    
    
    
    
    
    
    
    
    
    
    
    
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
    
    
    
    
    //float posx = this->_x+this->_staticx;
    //float posy = this->_y+this->_staticy;
    
    
    float posx = graphics.getCameraX() + this->_staticx;
    float posy = graphics.getCameraY() + this->_staticy;
    
    
    
    
    //std::cout << this->_staticdx << " " << this->_staticdy << std::endl;
    
    //Sprite::drawAngle(graphics, 640-15 + ( (this->_x+this->_staticx)-640+15)* std::cos(this->_playerAngle*3.14159/180) - ((this->_y+this->_staticy)-400+30) * std::sin(this->_playerAngle*3.14159/180), 400-30 + ( ((this->_y+this->_staticy)-400+30) * std::cos(this->_playerAngle*3.14159/180) + ((this->_x+this->_staticx)-640+15) * std::sin(this->_playerAngle*3.14159/180)), this->_angle + this->_playerAngle);
    
    
    
//R(a-b)+b
    int playerx = graphics.getPlayerCenterX();
    int playery = graphics.getPlayerCenterY();
    
    //Sprite::drawAngle(graphics, 640-8 + ( (posx)-640+8)* std::cos(this->_playerAngle*3.14159/180) - ((posy)-400+12) * std::sin(this->_playerAngle*3.14159/180), 400-12 + ( ((posy)-400+12) * std::cos(this->_playerAngle*3.14159/180) + ((posx)-640+8) * std::sin(this->_playerAngle*3.14159/180)), this->_angle + this->_playerAngle);
    
    double playerAngle = graphics.getCameraAngle();
    
    
    
    
    double renderX = playerx-8 + ( (posx)- playerx+8)* std::cos(playerAngle*3.14159/180) - ((posy)-playery+12) * std::sin(playerAngle*3.14159/180);
    double renderY = playery-12 + ( ((posy)-playery+12) * std::cos(playerAngle*3.14159/180) + ((posx)-playerx+8) * std::sin(playerAngle*3.14159/180));

    this->_unitSprite.drawAngle(graphics, renderX, renderY, this->_angle + playerAngle);
    
    

    
    
    
    
    
    
    
    //Sprite::drawAngle(graphics, 640-8 + ( (posx)-640+8)* std::cos(this->_playerAngle*3.14159/180) - ((posy)-400+16) * std::sin(this->_playerAngle*3.14159/180), 400-16 + ( ((posy)-400+16) * std::cos(this->_playerAngle*3.14159/180) + ((posx)-640+8) * std::sin(this->_playerAngle*3.14159/180)), this->_angle + this->_playerAngle);
    
    
    //graphics.drawLine(posx, posy, 0, 0);
    
    
    //std::cout << "x: " << this->_x << " y : " << this->_y << " sx: " << this->_staticx << " sy: " << this->_staticy << std::endl;
    
    //std::cout << "dx: " << this->_dx << " dy : " << this->_dy << " dsx: " << this->_staticdx << " dsy: " << this->_staticdy << std::endl;

    
    //this->stopMoving();
    
    //graphics.drawLine(this->_staticx+15, this->_staticy+30, this->_staticx+15 + 100*std::sin(this->_angle*3.14159/180), this->_staticy+30 - 100*std::cos(this->_angle*3.14159/180) );
    
    

    
    //graphics.drawLine(this->_staticx + this->_x+15, this->_staticy + this->_y+30, this->_staticx + this->_x+15 + 100*std::sin(this->_angle*3.14159/180), this->_staticy + this->_y+30 - 100*std::cos(this->_angle*3.14159/180) );
    



}

void Unit::draw(Graphics &graphics, bool isSelected) {

    
    float posx = graphics.getCameraX() + this->_staticx;
    float posy = graphics.getCameraY() + this->_staticy;


    int playerx = graphics.getPlayerCenterX();
    int playery = graphics.getPlayerCenterY();

    double playerAngle = graphics.getCameraAngle();
    
    
    
    
    double renderX = playerx-8 + ( (posx)- playerx+8)* std::cos(playerAngle*3.14159/180) - ((posy)-playery+12) * std::sin(playerAngle*3.14159/180);
    double renderY = playery-12 + ( ((posy)-playery+12) * std::cos(playerAngle*3.14159/180) + ((posx)-playerx+8) * std::sin(playerAngle*3.14159/180));
    
    this->_unitSprite.drawAngle(graphics, renderX, renderY, this->_angle + playerAngle);
    
    
    if(isSelected) {            //
        this->_unitSelected.drawAngle(graphics, renderX, renderY, this->_angle + playerAngle);
    }
       
}



void Unit::update(int elapsedTime, Graphics &graphics) {
    //movement

    //printf("%f, %f\n", this->_staticdx, this->_staticdy);
    

    //this->_y += this->_dy * elapsedTime;
    //this->_x += this->_dx * elapsedTime;
    
    this->_staticy += this->_staticdy * elapsedTime;
    this->_staticx += this->_staticdx * elapsedTime;
    
    
    
    if(this->_isMoving) {
        this->_distTraveled += player_constants::WALK_SPEED * elapsedTime;
    }
    
    
    
    
    
    //this->_collisionRect.update(elapsedTime, this->_dx+this->_staticdx, this->_dy+this->_staticdy);
    //this->_collisionRect.setXY(this->_x+this->_staticx, this->_y+this->_staticy + 4);
    
    //this->_collisionRect.setXY(graphics.getCameraX()+this->_staticx, graphics.getCameraY()+this->_staticy + 4);
    
    this->_unitSprite.update();
    
    
}

void Unit::setPlayerAngle(float playerAngle) {
    this->_playerAngle = playerAngle;
}



float Unit::getStaticX() {
    return this->_staticx;
}
float Unit::getStaticY() {
    return this->_staticy;
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
    
    
    
    //check if destination arrived
    if(this->_isMoving) {
        if(this->_distTraveled > this->_distToNextPoint) {
            
            printf("%f, %f\n", this->_distTraveled, this->_distToNextPoint);
            
            
            //stop the unit
            this->_staticdx = 0;
            this->_staticdy = 0;
            
            
            this->_staticx = this->_destinationx;
            this->_staticy = this->_destinationy - 4;
            
            this->_isMoving = 0;
            
            
            //move on to next destination
            if(this->_unitMovementOrders.size() > 0) {
                moveToNextPosition();
            }
            
        }
    }
    
    
    //check for angle things
    if(this->_isAngleMoving) {                      //this is sloppy. it just checks every cycle. FIX ME!
        switch(this->_state)  {
            case CHANGE_ANGLE_UP:
                if(this->_angle < this->_destinationAngle) {
                    this->_angle += 20;
                }
                else {
                    this->_angle = this->_destinationAngle;
                    /*
                    if(this->_unitMovementOrders.size() > 0) {
                        this->_state = MOVE_FORWARD;
                    }
                    else {
                        this->_state = STILL;
                    }
                     */
                    //this->_isAngleMoving = 0;
                }
                
                break;
            case CHANGE_ANGLE_DOWN:
                
                if(this->_angle > this->_destinationAngle) {
                    this->_angle -= 20;
                }
                else {
                    this->_angle = this->_destinationAngle;
                    /*
                    if(this->_unitMovementOrders.size() > 0) {
                        this->_state = MOVE_FORWARD;
                    }
                    else {
                        this->_state = STILL;
                    }
                     */
                    
                    //this->_isAngleMoving = 0;
                }
                
                break;
        }
        
        
        
        
        
        
    }

    
    
    

    
    
    /*
    for( double i : this->_unitAngleOrders) {
        printf("%f, ", i);
    }
    printf("\n");
    */
    
    //printf("staticx: %f, staticy: %f\n", this->_staticx, this->_staticy);

    
    
    /*
    switch(this->_state) {
        case CHANGE_ANGLE_UP:
            if(this->_angle < this->_destinationAngle) {
                this->_angle+=5;
            }
            else {
                this->_angle = this->_destinationAngle;
                if(this->_unitMovementOrders.size() > 0) {
                    this->_state = MOVE_FORWARD;
                }
                else {
                    this->_state = STILL;
                }
            }
            break;
        case CHANGE_ANGLE_DOWN:
            if(this->_angle > this->_destinationAngle) {
                this->_angle-=5;
            }
            else {
                this->_angle = this->_destinationAngle;
                if(this->_unitMovementOrders.size() > 0) {
                    this->_state = MOVE_FORWARD;
                }
                else {
                    this->_state = STILL;
                }
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
            
     
            break;
            
        case STILL:
            //stopMoving();
            break;
    }
    */
    
    
}

void Unit::addToMovementOrders(std::vector<Vector2> pos) {
    
    this->_unitMovementOrders.clear();
    this->_unitAngleOrders.clear();
    
    for(Vector2 i : pos) {
        this->_unitMovementOrders.push_back(i);
    }
    
    //start moving to first position
    moveToPosition(pos[0].x, pos[0].y);

}

void Unit::addToMovementOrders(Vector2 pos) {

    this->_unitMovementOrders.push_back(pos);
    
    moveToPosition(pos.x, pos.y);
    
}

void Unit::addToAngleOrders(double angle) {
    this->_unitAngleOrders.clear();
    
    
    //if your not busy, move to that angle
    if(this->_state == STILL) {
        //printf("static angle move\n");
        moveToAngle(angle);
    }
    else {  //otherwise do it after you move to the relevent position
        this->_unitAngleOrders.push_back(angle);
    }
    
    
    
}

void Unit::printMovementOrders() {
    for(Vector2 i : this->_unitMovementOrders) {
        printf("%d, %d\n", i.x, i.y);
    }
}

void Unit::printFirstName() {
    printf("%s\n", this->_firstName.c_str());
}

/*
Rectangle Unit::getCollisionRect() {
    return this->_collisionRect;
}
*/
