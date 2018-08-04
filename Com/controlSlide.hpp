//
//  controlSlide.hpp
//  Com
//
//  Created by Jonah Glick on 7/15/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef controlSlide_hpp
#define controlSlide_hpp

#include "graphics.hpp"
#include "sprite.hpp"

class Graphics;

class ControlSlide : public Sprite {
public:
    ControlSlide();
    ControlSlide(Graphics &graphics);

    void draw(Graphics &graphics);
    void update(float elapsedTime);
    
    bool checkSlideCollision(int xm, int ym);
    void handleSlideMovement(int xm, int ym);
    
    void centerSlideToZero();
    
    void setPlayerAngle(float playerAngle);
    
    void moveForwardParallax();
    void moveBackwardParallax();
    void moveRightParallax();
    void moveLeftParallax();
    void stopMovingParallax();
    
    void moveUpRightParallax();
    void moveUpLeftParallax();
    void moveDownRightParallax();
    void moveDownLeftParallax();
    
private:
    float _x, _y;
    float _dx, _dy;
    float _angle;
    float _width;
    int _height;
    float _staticx, _staticy;
    bool _centerHold;
    bool _sideHoldR;
    bool _sideHoldL;
    float _playerAngle;
    
    float _camerax, _cameray;
    
    float _RrotatedX, _RrotatedY;
    float _LrotatedX, _LrotatedY;
    
};


#endif /* controlSlide_hpp */