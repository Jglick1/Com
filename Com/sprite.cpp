//
//  sprite.cpp
//  Com
//
//  Created by Jonah Glick on 6/20/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "sprite.hpp"
#include "graphics.hpp"
#include "globals.hpp"
#include <iostream>

Sprite::Sprite() {}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY) :
_x(posX), _y(posY)
{
    
    this->_sourceRect.x = sourceX;
    this->_sourceRect.y = sourceY;
    this->_sourceRect.w = width;
    this->_sourceRect.h = height;
    
    this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
    if (this->_spriteSheet == NULL) {
        printf("Error: Unable to load image\n");
    }
    
    this->_boundingBox = Rectangle(this->_x, this->_y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
    
}

Sprite::~Sprite() {}

void Sprite::draw(Graphics &graphics, int x, int y) {
    
    SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE };
    
    //std::cout << "x: " << x << " y: " << y << std::endl;
    
    graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
    
}


void Sprite::drawAngle(Graphics &graphics, int x, int y, float angle) {
    
    SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE };
    
    
    //std::cout << this->_sourceRect.w << " " <<this->_sourceRect.h << std::endl;
    
    //std::cout << "x: " << x << " y: " << y << std::endl;
    
    //graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
    
    //texture, sourceRectangle, destinationRectangle
    
    /*
    int SDL_RenderCopyEx(SDL_Renderer*          renderer,
                         SDL_Texture*           texture,
                         const SDL_Rect*        srcrect,
                         const SDL_Rect*        dstrect,
                         const double           angle,
                         const SDL_Point*       center,
                         const SDL_RendererFlip flip)
    */
    //SDL_Point point = {x, y};
    SDL_Point point = {48, 80}; // 96/2 and 32+32+16
    
    //std::cout << "x: " << x << " y: " << y << std::endl;
    
    SDL_RenderCopyEx(graphics.getRenderer(), this->_spriteSheet, &this->_sourceRect, &destinationRectangle, angle, &point, SDL_FLIP_NONE);
    
}

void Sprite::update() {
    this->_boundingBox = Rectangle(this->_x, this->_y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE);
}

const Rectangle Sprite::getBoundingBox() const {
    return this->_boundingBox;
}

const sides::Side Sprite::getCollisionSide(Rectangle &other) const {
    int amtRight, amtLeft, amtTop, amtBottom;
    amtRight = this->getBoundingBox().getRight() - other.getLeft();
    amtLeft = other.getRight() - this->getBoundingBox().getLeft();
    amtTop = other.getBottom() - this->getBoundingBox().getTop();
    amtBottom = this->getBoundingBox().getBottom() - other.getTop();
    
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