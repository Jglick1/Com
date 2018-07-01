//
//  map.hpp
//  Com
//
//  Created by Jonah Glick on 6/23/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef map_hpp
#define map_hpp

#include <SDL2/SDL.h>
#include <string>
#include "rectangle.hpp"
#include "globals.hpp"

class Graphics;

class Map {
public:
    Map();
    Map(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY);
    virtual ~Map();
    virtual void update();
    void draw(Graphics &graphics, int x, int y);
    void drawAngle(Graphics &graphics, int x, int y, float angle);
    
    void drawTrans(Graphics &graphics, int transx, int transy, float ang, int x, int y, bool camera_move);
    
    const inline float getX() const { return this->_x; }
    const inline float getY() const { return this->_y; }
    
    /*
    const sides::Side getCollisionSide(Rectangle &other) const;
    const Rectangle getBoundingBox() const;
    */
    
protected:
    SDL_Rect _sourceRect;
    SDL_Texture* _spriteSheet;
    

    float _x, _y;

    
};


#endif /* map_hpp */
