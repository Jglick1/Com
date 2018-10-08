//
//  plusSign.hpp
//  Com
//
//  Created by Jonah Glick on 10/7/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef plusSign_hpp
#define plusSign_hpp

#include "sprite.hpp"
#include "globals.hpp"
#include "rectangle.hpp"
#include "graphics.hpp"

class Graphics;


class PlusSign {
public:
    PlusSign();
    PlusSign(Graphics &graphics);
    ~PlusSign();
    void update();
    void draw(Graphics &graphics);
    
    void changeY(int newY);
    void changeX(int newX);
    
    Rectangle getRectangle();
    
private:
    
    double _x, _y;
    int _width, _height;
    
    Sprite _plus;
    
    Rectangle _rectangle;
    
};

#endif /* plusSign_hpp */
