//
//  unit.hpp
//  Com
//
//  Created by Jonah Glick on 6/23/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef unit_hpp
#define unit_hpp

#include "sprite.hpp"
#include "globals.hpp"
//#include "level.hpp"


class Graphics;

class Unit : public Sprite {
    
public:
    Unit();
    Unit(Graphics &graphics, Vector2 spawnPoint);
    void draw(Graphics &graphics);
    void update(float elapsedTime, int mx, int my);
    
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();
    
    void moveUpAndRight();
    void moveUpAndLeft();
    void moveDownAndRight();
    void moveDownAndLeft();
    
    void stopMoving();
    
    
    //virtual void animationDone(std::string currentAnimation);
    //virtual void setupAnimations();
    
    //void handleTileCollisions(std::vector<Rectangle> &others);
    //void handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics);
    
    const float getX() const;
    const float getY() const;
    
    const float getMX() const;
    const float getMY() const;
    

    
private:
    float _dx, _dy;
    int _mx, _my; //mouse x and mouse y
    Sprite _fov;
    
    
};

#endif /* unit_hpp */