//
//  plusSign.cpp
//  Com
//
//  Created by Jonah Glick on 10/7/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "plusSign.hpp"


PlusSign::PlusSign() {}

PlusSign::~PlusSign() {}

PlusSign::PlusSign(Graphics &graphics) :
_width(64),
_height(64),
_x(700.0),
_y(100.0)

{

    
    this->_plus = Sprite(graphics, "/Users/jonahglick/Documents/Com/plus_sign.png", 0, 0, 64, 64, 0, 0);

    
    this->_rectangle = Rectangle(700, 100, 64, 64);
    
}



void PlusSign::draw(Graphics &graphics) {
    
    this->_plus.draw(graphics, this->_x, this->_y);
    
    //graphics.drawRect(std::round(this->_rectangle.getX()), std::round(this->_rectangle.getY()), this->_rectangle.getWidth(), this->_rectangle.getHeight());
    
    
}

Rectangle PlusSign::getRectangle() {
    return this->_rectangle;
}

void PlusSign::update() {
    
}
