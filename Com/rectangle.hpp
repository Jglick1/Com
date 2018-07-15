//
//  rectangle.hpp
//  Com
//
//  Created by Jonah Glick on 6/20/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef rectangle_h
#define rectangle_h

#include "globals.hpp"
#include <cmath>
#include <iostream>

class Rectangle {
public:
    Rectangle() {}
    
    Rectangle(int x, int y, int width, int height) :
    _x(x),
    _y(y),
    _width(width),
    _height(height),
    _startX(x),
    _startY(y)
    {}
    
    const int getCenterX() const { return this->_x + this->_width / 2; }
    const int getCenterY() const { return this->_y + this->_height / 2; }
    
    
    const int getLeft() const { return this->_x; }
    const int getRight() const { return this->_x + this->_width; }
    const int getTop() const { return this->_y; }
    const int getBottom() const{ return this->_y + this->_height; }
    
    /*
    const float getLeft() const { return this->_x; }
    const float getRight() const { return this->_x + this->_width; }
    const float getTop() const { return this->_y; }
    const float getBottom() const{ return this->_y + this->_height; }
    */
    const int getX() const {return this->_x; }
    const int getY() const {return this->_y; }
    
    const int getStartX() const {return this->_startX; }
    const int getStartY() const {return this->_startY; }
    
    const int getWidth() const { return this->_width; }
    const int getHeight() const { return this->_height; }
    
    const int getSide(const sides::Side side) const {
        return
        side == sides::LEFT ? this->getLeft() :
        side == sides::RIGHT ? this->getRight() :
        side == sides::TOP ? this->getTop() :
        side == sides::BOTTOM ? this->getBottom() :
        sides::NONE;
    }
    
    const bool collidesWith(const Rectangle &other) const {
        return
        this->getRight() >= other.getLeft() &&
        this->getLeft() <= other.getRight() &&
        this->getTop() <= other.getBottom() &&
        this->getBottom() >= other.getTop();
    }
     
    
    
    const bool isValidRectangle() const {
        return (this->_x >= 0 && this->_y >= 0 && this->_width >= 0 && this->_height >= 0);
    }
    
    void update(float elapsedTime, float dx, float dy) {
        
        //this->_x += std::round(dx * elapsedTime);
        //this->_y += std::round(dy * elapsedTime);
        
        
        this->_x += dx * elapsedTime;
        this->_y += dy * elapsedTime;
        
    }
    
    const inline Rectangle getRect() const { return *this; }
    
    void changeX(int x) {
        this->_x = x;
    }
    
    void changeY(int y) {
        this->_y = y;
    }
    
    const sides::Side getCollisionSide(Rectangle &other) const { //player is the other
        int amtRight, amtLeft, amtTop, amtBottom;
        amtRight = getRight() - other.getLeft();
        amtLeft = other.getRight() - getLeft();
        amtTop = other.getBottom() - getTop();
        amtBottom = getBottom() - other.getTop();
        
        
        //std::cout << "amtRight: " << amtRight << std::endl;
        //std::cout << "amtLeft: " << amtLeft << std::endl;
        //std::cout << "amtTop: " << amtTop << std::endl;
        //std::cout << "amtBottom: " << amtBottom << std::endl;
        
        //std::cout << "playerx: " << other.getX() << std::endl;
        //std::cout << "playery: " << other.getY() << std::endl;
        //std::cout << "playerwidth: " << other.getWidth() << std::endl;
        //std::cout << "playerheight: " << other.getHeight() << std::endl;
        
        //std::cout << "x: " << getX() << std::endl;
        //std::cout << "y: " << getY() << std::endl;
        //std::cout << "width: " << getWidth() << std::endl;
        //std::cout << "height: " << getHeight() << std::endl;
        
        int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
        int lowest = vals[0];
        for (int i = 0; i < 4; i++) {
            if (vals[i] < lowest) {
                lowest = vals[i];
            }
        }
        
        return
        lowest == abs(amtRight) ? sides::RIGHT :
        lowest == abs(amtLeft) ? sides::LEFT :
        lowest == abs(amtTop) ? sides::TOP :
        lowest == abs(amtBottom) ? sides::BOTTOM :
        sides::NONE;
        
    }
    
private:
    float _x, _y;
    int _width, _height;
    int _startX;
    int _startY;
    
};


#endif /* rectangle_h */
