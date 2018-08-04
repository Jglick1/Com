//
//  controlSlide.cpp
//  Com
//
//  Created by Jonah Glick on 7/15/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "controlSlide.hpp"
#include "graphics.hpp"
#include <cmath>

namespace player_constants {
    const float WALK_SPEED = 0.2f;
}

ControlSlide::ControlSlide() {}

ControlSlide::ControlSlide(Graphics &graphics) :
Sprite(graphics, "/Users/jonahglick/Documents/Com/control_slider.png", 0, 0, 150, 25, 0, 0),
//_x(0),
//_y(0),
_staticx(500),
_staticy(500),
_angle(0.0),
_width(150),
_height(25),
_centerHold(0),
_playerAngle(0.0),
_dx(0.0),
_dy(0.0),
_camerax(0.0),
_cameray(0.0),
_RrotatedX(0.0),
_RrotatedY(0.0),
_LrotatedX(0.0),
_LrotatedY(0.0),
_sideHoldR(0),
_sideHoldL(0)
{}




//ControlSlide::~ControlSlide() {}

void ControlSlide::draw(Graphics &graphics) {
    //printf("test");
    
    int tx = std::round(this->_x + this->_staticx);
    int ty = std::round(this->_y + this->_staticy);
    int width = std::round(this->_width);
    int height = std::round(this->_height);

    //graphics.drawRect(x, y, width, height);
    //graphics.drawRect(x, y, height, height);
    //graphics.drawRect(x + width - height, y, height, height);
    
    
    //Sprite::drawControlSlider(graphics, tx, ty, this->_playerAngle);
    
    int posx = std::round(this->_x + this->_staticx);
    int posy = std::round(this->_y + this->_staticy);
    
    
    //Sprite::drawControlSlider(graphics, 640-75 + ( (posx)-640+75)* std::cos(this->_playerAngle*3.14159/180) - ((posy)-400+12) * std::sin(this->_playerAngle*3.14159/180), 400-12 + ( ((posy)-400+12) * std::cos(this->_playerAngle*3.14159/180) + ((posx)-640+75) * std::sin(this->_playerAngle*3.14159/180)), this->_angle + this->_playerAngle);
    
    Sprite::drawControlSlider(graphics, this->_camerax, this->_cameray, this->_angle + this->_playerAngle);
    
    //graphics.drawPoint(posx + width - height/2, posy + height/2);
    
    //a -> this->_camerax + width - height/2
    //b -> this->_camerax + width / 2
    
    /*
    double a_x = this->_camerax + width - height/2;
    double b_x = this->_camerax + width / 2;
    
    double a_y = this->_cameray + height / 2;
    double b_y = this->_cameray + height / 2;
    
    int rotated_x = std::round(std::cos(this->_angle*3.14159/180) * (a_x - b_x) - std::sin(this->_angle*3.14159/180) * (a_y - b_y) + b_x);
    int rotated_y = std::round(std::sin(this->_angle*3.14159/180) * (a_x - b_x) + std::cos(this->_angle*3.14159/180) * (a_y - b_y) + b_y);
    */
    
    graphics.drawPoint(this->_RrotatedX, this->_RrotatedY);
    graphics.drawPoint(this->_LrotatedX, this->_LrotatedY);
    
    //printf("%f\n", this->_playerAngle);
    //Sprite::drawControlSlider(graphics, this->_camerax, this->_cameray, this->_angle + this->_playerAngle);
    
    //Sprite::draw(graphics, 300, 300);
    
}


void ControlSlide::update(float elapsedTime) {
    this->_x += this->_dx * elapsedTime;
    this->_y += this->_dy * elapsedTime;
    
    double totAngle = this->_angle + this->_playerAngle;
    
    int posx = std::round(this->_x + this->_staticx);
    int posy = std::round(this->_y + this->_staticy);
    
    this->_camerax = 640-75 + ( (posx)-640+75)* std::cos(this->_playerAngle*3.14159/180) - ((posy)-400+12) * std::sin(this->_playerAngle*3.14159/180);
    this->_cameray = 400-12 + ( ((posy)-400+12) * std::cos(this->_playerAngle*3.14159/180) + ((posx)-640+75) * std::sin(this->_playerAngle*3.14159/180));
    
    
    
    double a_x = this->_camerax + this->_width - this->_height/2;
    double b_x = this->_camerax + this->_width / 2;
    
    double a_lx = this->_camerax + this->_height/2;
    
    double a_y = this->_cameray + this->_height / 2;
    double a_ly = this->_cameray + this->_height / 2;
    double b_y = this->_cameray + this->_height / 2;
    
    this->_RrotatedX = std::round(std::cos(totAngle * 3.14159/180) * (a_x - b_x) - std::sin(totAngle * 3.14159/180) * (a_y - b_y) + b_x);
    this->_RrotatedY = std::round(std::sin(totAngle * 3.14159/180) * (a_x - b_x) + std::cos(totAngle * 3.14159/180) * (a_y - b_y) + b_y);
    
    
    this->_LrotatedX = std::round(std::cos(totAngle*3.14159/180) * (a_lx - b_x) - std::sin(totAngle*3.14159/180) * (a_ly - b_y) + b_x);
    this->_LrotatedY = std::round(std::sin(totAngle*3.14159/180) * (a_lx - b_x) + std::cos(totAngle*3.14159/180) * (a_ly - b_y) + b_y);
    
    
    
    
}

void ControlSlide::setPlayerAngle(float playerAngle) {
    this->_playerAngle = playerAngle;
}

bool ControlSlide::checkSlideCollision(int xm, int ym) {
    
    int tx = std::round(this->_x + this->_staticx);
    int ty = std::round(this->_y + this->_staticy);
    int width = std::round(this->_width);
    int height = std::round(this->_height);
    
    int posx = std::round(this->_x + this->_staticx);
    int posy = std::round(this->_y + this->_staticy);
    
    tx = std::round(this->_camerax);
    ty = std::round(this->_cameray);
    
    //this->_camerax = 640-8 + ( (posx)-640+8)* std::cos(this->_playerAngle*3.14159/180) - ((posy)-400+16) * std::sin(this->_playerAngle*3.14159/180);
    //this->_cameray = 400-16 + ( ((posy)-400+16) * std::cos(this->_playerAngle*3.14159/180) + ((posx)-640+8) * std::sin(this->_playerAngle*3.14159/180));
    
    
    
    //printf("xm: %d, ym: %d\n", xm, ym);
    
    //printf("xm: %d, ym: %d\n", x, x + height);
    
    
    //the left box
    if(std::sqrt(std::pow(xm - this->_LrotatedX,2) + std::pow(ym - this->_LrotatedY,2)) < (height/2) ) {
        //printf("right collision!\n");
        this->_sideHoldL = 1;
        return 1;
    }
    
    //the middle circle
    if(std::sqrt(std::pow(xm - (tx + width/2),2) + std::pow(ym - (ty + height/2),2)) < (height/2) ) {
        //printf("middle collision!\n");
        
        this->_centerHold = 1;
        return 1;
    }

    
    //the right box
    if(std::sqrt(std::pow(xm - this->_RrotatedX,2) + std::pow(ym - this->_RrotatedY,2)) < (height/2) ) {
        //printf("right collision!\n");
        this->_sideHoldR = 1;
        return 1;
    }
    
    
    
    
    /*
    if((tx + width - height < xm  && xm < (tx + width)) && (ty < ym  && ym < (ty + height))) {
        //printf("right collision!\n");
        this->_sideHold = 1;
        return 1;
    }
    */
    
    
    
    /*
    if(std::sqrt(std::pow(xm - (tx + width/2),2) + std::pow(ym - (ty + height/2),2)) < (height/2) ) {
        //printf("middle collision!\n");
        
        this->_centerHold = 1;
        return 1;
    }
    */
    
    
    /*
    if(this->_centerHold) {
        this->_staticx = xm - width/2;
        this->_staticy = ym - height/2;
    }
     */
    return 0;
    
}

void ControlSlide::handleSlideMovement(int xm, int ym) {
    
    int width = std::round(this->_width);
    int height = std::round(this->_height);
    //printf("%d\n", this->_centerHold);
    
    xm += this->_x;
    ym -= this->_y;
    
    if(this->_centerHold) {
        this->_staticx = xm - width/2;
        this->_staticy = ym - height/2;
    }
    else if (this->_sideHoldR) {
        float xdiff = xm - (this->_staticx + width/2);
        float ydiff = ym - (this->_staticy + height/2);
        float angle = 0.0;
    
        if(ydiff < 0) {
            angle = -std::atan(xdiff/ydiff) - 3.14159/2;
        }
        else {
            angle = -std::atan(xdiff/ydiff) + 3.14159/2;
        }
        
        this->_angle = angle*180/3.14159;
    }
    else if (this->_sideHoldL) {
        float xdiff = xm - (this->_staticx + width/2);
        float ydiff = ym - (this->_staticy + height/2);
        float angle = 0.0;
        
        if(ydiff < 0) {
            angle = -std::atan(xdiff/ydiff) - 3.14159/2 + 3.14159;
        }
        else {
            angle = -std::atan(xdiff/ydiff) + 3.14159/2 + 3.14159;
        }
        
        this->_angle = angle*180/3.14159;
    }
    
    
    
}

void ControlSlide::centerSlideToZero() {
    
    this->_centerHold = 0;
    this->_sideHoldR = 0;
    this->_sideHoldL = 0;
    
}

void ControlSlide::moveForwardParallax() {
    this->_dx = -player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180);
    this->_dy = -player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180);
}

void ControlSlide::moveBackwardParallax() {
    this->_dx = player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180);
    this->_dy = player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180);
}


void ControlSlide::moveRightParallax() {
    this->_dx = player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180);
    this->_dy = -player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180);
}

void ControlSlide::moveLeftParallax() {
    this->_dx = -player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180);
    this->_dy = player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180);
}

void ControlSlide::stopMovingParallax() {
    this->_dx = 0.0;
    this->_dy = 0.0;
}

void ControlSlide::moveUpRightParallax() {                 //should have said forwardRight and backwardRight instead
    this->_dx = -player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421;
    this->_dy = -player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421 - player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421;
    //this->_unit.moveForwardRightParallax();
}
void ControlSlide::moveUpLeftParallax() {
    this->_dx = -player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421 + -player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421;
    this->_dy = -player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421;
    //this->_unit.moveForwardRightParallax();
}
void ControlSlide::moveDownRightParallax() {
    this->_dx = player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421;
    this->_dy = player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421 + -player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421;
}
void ControlSlide::moveDownLeftParallax() {
    this->_dx = player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421 + -player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421;
    this->_dy = player_constants::WALK_SPEED * std::cos(this->_playerAngle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::sin(this->_playerAngle * 3.14159 / 180)/1.41421;
}
