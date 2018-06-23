//
//  sprite.hpp
//  Com
//
//  Created by Jonah Glick on 6/20/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef sprite_hpp
#define sprite_hpp

#include <SDL2/SDL.h>
#include <string>
#include "rectangle.hpp"
#include "globals.hpp"

class Graphics;

class Sprite {
public:
    Sprite();
    Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY);
    virtual ~Sprite();
    virtual void update();
    void draw(Graphics &graphics, int x, int y);
    void drawAngle(Graphics &graphics, int x, int y, float angle);
    
    const Rectangle getBoundingBox() const;
    const sides::Side getCollisionSide(Rectangle &other) const;
    
    const inline float getX() const { return this->_x; }
    const inline float getY() const { return this->_y; }
    
    
protected:
    SDL_Rect _sourceRect;
    SDL_Texture* _spriteSheet;
    
    Rectangle _boundingBox;
    
    float _x, _y;
    
};


#endif /* sprite_hpp */
