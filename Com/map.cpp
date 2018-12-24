//
//  map.cpp
//  Com
//
//  Created by Jonah Glick on 6/23/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "map.hpp"
#include "graphics.hpp"
#include "globals.hpp"
#include <iostream>

Map::Map() {}

Map::Map(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY) //:
//_x(posX), _y(posY)
{
    
    this->_sourceRect.x = sourceX;
    this->_sourceRect.y = sourceY;
    this->_sourceRect.w = width;
    this->_sourceRect.h = height;
    
    this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
    if (this->_spriteSheet == NULL) {
        printf("Error: Unable to load image\n");
    }
    
    
}

Map::~Map() {}

/*
void Map::draw(Graphics &graphics, int x, int y) {
    
    SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::LEVEL_SCALE, this->_sourceRect.h * globals::LEVEL_SCALE };

    
    //std::cout << "x: " << x << " y: " << y << std::endl;
    
    graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
    
}
*/
/*
void Map::drawAngle(Graphics &graphics, int x, int y, float angle) {
    SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::LEVEL_SCALE, this->_sourceRect.h * globals::LEVEL_SCALE };
    
    //SDL_Point point = {640, 690};
    SDL_Point point = {625 + 15 - x, 660 + 30 - y};
    

    SDL_RenderCopyEx(graphics.getRenderer(), this->_spriteSheet, &this->_sourceRect, &destinationRectangle, angle, &point, SDL_FLIP_NONE);
}
*/

void Map::update() {

}

void Map::drawTrans(Graphics &graphics, int transx, int transy, float ang, int x, int y) {
    
    SDL_Rect destinationRectangle = {transx, transy, this->_sourceRect.w * globals::LEVEL_SCALE, this->_sourceRect.h * globals::LEVEL_SCALE };

    


    //SDL_Point point = {0, 0};
    
    //SDL_Point point = {640 - x, 400 - y};
    
    SDL_Point point = {graphics.getPlayerCenterX() - x, graphics.getPlayerCenterY() - y};

    
    SDL_RenderCopyEx(graphics.getRenderer(), this->_spriteSheet, &this->_sourceRect, &destinationRectangle, ang, &point, SDL_FLIP_NONE);
    
}

/*
const sides::Side Map::getCollisionSide(Rectangle &other) const {
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

const Rectangle Map::getBoundingBox() const {
    return this->_boundingBox;
}
*/




