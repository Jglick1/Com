//
//  hud.cpp
//  Com
//
//  Created by Jonah Glick on 8/3/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//


#include "hud.hpp"
#include "graphics.hpp"

HUD::HUD() {}

HUD::HUD(Graphics &graphics, Player &player) {
    
    this->_player = player;
    
    this->_fov = Sprite(graphics, "/Users/jonahglick/Documents/Com/fov.png", 0,0,1280,800,0,0);
    
    //int sourceX, int sourceY, int width, int height, float posX, float posY
    
    this->_TextureSheet_SA = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadText("SA", 255, 0, 0));
    this->_TextureSheet_BA = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadText("BA", 255, 0, 0));
    
    this->_fireMode = 0;
    
}

void HUD::update(int elapsedTime, Player &player) {
    
    this->_player = player;
    
    
}

void HUD::draw(Graphics &graphics) {
    
    //this->_fov.draw(graphics, 0, 0);
    
    //drawing some text
    SDL_Rect desintationRectangle;
    desintationRectangle.x = 1280 - 60;
    desintationRectangle.y = 0;
    
    //SDL_Surface* tmp = graphics.loadText("BA");
    
    //desintationRectangle.w = tmp->w;
    //desintationRectangle.h = tmp->h;

    desintationRectangle.w = 50;
    desintationRectangle.h = 50;
    
    
    //graphics.drawText(this->_TextureSheet_SA, &desintationRectangle);
    
    if(this->_fireMode == 1) {
        graphics.drawText(this->_TextureSheet_SA, &desintationRectangle);
    }
    else {
        graphics.drawText(this->_TextureSheet_BA, &desintationRectangle);
    }
    
    
}

void HUD::cycleFireMode() {
    this->_fireMode = !this->_fireMode;
}
