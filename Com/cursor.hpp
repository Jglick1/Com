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
    void update(int elapsedTime, int ym, int old_ym);
    void draw(Graphics &graphics);
    
private:
    
    Sprite _cursor;
    
};

#endif /* commandMouse_hpp */
