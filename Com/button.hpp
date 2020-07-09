//
//  Header.h
//  Com
//
//  Created by Jonah Glick on 7/8/20.
//  Copyright © 2020 Jonah Glick. All rights reserved.
//

#ifndef Header_h
#define Header_h

#include "globals.hpp"
#include <cmath>
#include <iostream>

class Button {
public:
    Button() {}
    
    Button(int x, int y, int width, int height, ButtonType buttonType, bool enabled) :
    _x(x),
    _y(y),
    _width(width),
    _height(height),
    _enabled(enabled),
    _buttonType(buttonType)
    {}

    bool checkMouseCollision(int mx, int my) {
        return 0;
    }
    
    void toggleEnable() {
        this->_enabled = !this->_enabled;
    }
    
    const int getX() const {return this->_x;}
    const int getY() const {return this->_y;}
    const int getWidth() const {return this->_width;}
    const int getHeight() const {return this->_height;}
    
    bool isEnabled() {
        return this->_enabled;
    }
    
    ButtonType getType() {
        return this->_buttonType;
    }
    
    
private:
    float _x, _y;
    int _width, _height;
    bool _enabled;
    ButtonType _buttonType;
};


#endif /* Header_h */
