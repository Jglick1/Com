//
//  gunshotPath.cpp
//  Com
//
//  Created by Jonah Glick on 10/15/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "gunshotPath.hpp"
#include <cmath>

GunshotPath::GunshotPath() {}

GunshotPath::GunshotPath(Graphics &graphics, int beginX, int beginY, int endX, int endY, int lifeTime) :
_beginX(beginX),
_beginY(beginY),
_endX(endX),
_endY(endY),
_lifeTime(lifeTime)
{
    
}

bool GunshotPath::update(int elapsedTime) {
    this->_age += elapsedTime;
    if(this->_age > this->_lifeTime) {
        return 1;       //the gunshot has expired
    }
    return 0;
}

void GunshotPath::draw(Graphics &graphics) {
    
    //double playerX = graphics.getCameraX() + graphics.getPlayerCenterX();
    //double playerY = graphics.getCameraY() + graphics.getPlayerCenterY();
    
    double cameraBeginX = this->_beginX + graphics.getCameraX();
    double cameraBeginY = this->_beginY + graphics.getCameraY();
    double cameraEndX = this->_endX + graphics.getCameraX();
    double cameraEndY = this->_endY + graphics.getCameraY();
    
    
    
    double rotatedBeginX = std::cos(graphics.getCameraAngle()*3.14159/180)*(cameraBeginX - 640) - std::sin(graphics.getCameraAngle()*3.14159/180)*(cameraBeginY - 400) + 640;
    double rotatedBeginY = std::sin(graphics.getCameraAngle()*3.14159/180)*(cameraBeginX - 640) + std::cos(graphics.getCameraAngle()*3.14159/180)*(cameraBeginY - 400) + 400;
    
    double rotatedEndX = std::cos(graphics.getCameraAngle()*3.14159/180)*(cameraEndX - 640) - std::sin(graphics.getCameraAngle()*3.14159/180)*(cameraEndY - 400) + 640;
    double rotatedEndY = std::sin(graphics.getCameraAngle()*3.14159/180)*(cameraEndX - 640) + std::cos(graphics.getCameraAngle()*3.14159/180)*(cameraEndY - 400) + 400;
    
    /*
    double rotatedBeginX = std::cos(graphics.getCameraAngle()*3.14159/180)*(this->_beginX - 640) - std::sin(graphics.getCameraAngle()*3.14159/180)*(this->_beginY - 400) + 640;
    double rotatedBeginY = std::sin(graphics.getCameraAngle()*3.14159/180)*(this->_beginX - 640) + std::cos(graphics.getCameraAngle()*3.14159/180)*(this->_beginY - 400) + 400;
    
    double rotatedEndX = std::cos(graphics.getCameraAngle()*3.14159/180)*(this->_endX - 640) - std::sin(graphics.getCameraAngle()*3.14159/180)*(this->_endY - 400) + 640;
    double rotatedEndY = std::sin(graphics.getCameraAngle()*3.14159/180)*(this->_endX - 640) + std::cos(graphics.getCameraAngle()*3.14159/180)*(this->_endY - 400) + 400;

    //graphics.drawLine(rotatedBeginX, rotatedBeginY, rotatedEndX, rotatedEndY);
    */
    
    
    int opacity = std::round((this->_lifeTime*1.0 - this->_age) / (this->_lifeTime*1.0) * 255.0);
    //opacity = 255;
    //printf("%d\n",opacity);
    
    graphics.drawGunshotLine(rotatedBeginX, rotatedBeginY, rotatedEndX, rotatedEndY, opacity);
    
}
