//
//  commandMouse.hpp
//  Com
//
//  Created by Jonah Glick on 8/3/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef cursor_hpp
#define cursor_hpp

#include "sprite.hpp"
#include <vector>

class Graphics;

class Cursor : public Sprite{
    
public:
    Cursor();
    Cursor(Graphics &graphics);
    void update(int elapsedTime, int ym, int old_ym, int xm, int old_xm, double cameraAngle, Graphics &graphics);
    void draw(Graphics &graphics);
    
    double getMapX();
    double getMapY();
    
    double getRotatedX();
    double getRotatedY();
    
private:
    
    Sprite _cursor;
    double _mapX, _mapY;
    double _rotatedX, _rotatedY;
    
};

#endif /* commandMouse_hpp */
