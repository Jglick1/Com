//
//  commandMouse.cpp
//  Com
//
//  Created by Jonah Glick on 8/3/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "cursor.hpp"
#include "graphics.hpp"

Cursor::Cursor() {}

Cursor::Cursor(Graphics &graphics) :
_mapX(640),
_mapY(0)

{
        
    this->_cursor = Sprite(graphics, "/Users/jonahglick/Documents/Com/cursor.png", 0,0,16,16,632,300);
    
    //int sourceX, int sourceY, int width, int height, float posX, float posY
    
}

void Cursor::update(int elapsedTime, int ym, int old_ym, int xm, int old_xm, double cameraAngle, Graphics &graphics) {
    
    //this->_y = ym;
    
    //if(std::abs(ym - old_ym) > 2) {
    //    this->_y = ym;
    //}
    //printf("%f\n",levelAngle);
    
    //45
    //30
    
    if((xm - old_xm) < 100 && (ym - old_ym) < 100) {
        this->_mapX += (xm - old_xm);
        this->_mapY += (ym - old_ym);
    }
    
    

    
    
    this->_y = 400 - 100 / std::cos(std::abs(cameraAngle * 3.14159 / 180));
    
    this->_y += 0.0;
    
    //45*std::tan(std::abs(cameraAngle * 3.14159 / 180));
    
    float posx = graphics.getCameraX() + this->_mapX;
    float posy = graphics.getCameraY() + this->_mapY;
    
    int playerx = graphics.getPlayerCenterX();
    int playery = graphics.getPlayerCenterY();

    double playerAngle = graphics.getCameraAngle();
    
    this->_rotatedX = playerx-8 + ( (posx)- playerx+8)* std::cos(playerAngle*3.14159/180) - ((posy)-playery+12) * std::sin(playerAngle*3.14159/180);
    this->_rotatedY = playery-12 + ( ((posy)-playery+12) * std::cos(playerAngle*3.14159/180) + ((posx)-playerx+8) * std::sin(playerAngle*3.14159/180));
    
}

void Cursor::draw(Graphics &graphics) {
    
    //printf("%f, %f\n",this->_x, this->_y);
    
    //this->_cursor.draw(graphics, 632, this->_y);
    
    //this->_cursor.draw(graphics, this->_mapX, this->_mapY);
    
    
    //float posx = graphics.getCameraX() + this->_mapX;
    //float posy = graphics.getCameraY() + this->_mapY;
    
    //int playerx = graphics.getPlayerCenterX();
    //int playery = graphics.getPlayerCenterY();
    
    
    
    double playerAngle = graphics.getCameraAngle();
    //this->_cursor.drawAngle(graphics, playerx-8 + ( (posx)- playerx+8)* std::cos(playerAngle*3.14159/180) - ((posy)-playery+12) * std::sin(playerAngle*3.14159/180), playery-12 + ( ((posy)-playery+12) * std::cos(playerAngle*3.14159/180) + ((posx)-playerx+8) * std::sin(playerAngle*3.14159/180)),  playerAngle);
    
    this->_cursor.drawAngle(graphics, this->_rotatedX, this->_rotatedY,  playerAngle);
    
    //this->_cursor.drawAngle(graphics, this->_mapX, this->_mapY,  playerAngle);
    
    
}

double Cursor::getMapX() {
    return this->_mapX;
}

double Cursor::getMapY() {
    return this->_mapY;
}

double Cursor::getRotatedX() {
    return this->_rotatedX;
}

double Cursor::getRotatedY() {
    return this->_rotatedY;
}
