//
//  player.hpp
//  Com
//
//  Created by Jonah Glick on 6/20/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef player_hpp
#define player_hpp

#include "sprite.hpp"
#include "globals.hpp"
//#include "level.hpp"


class Graphics;

class Player : public Sprite {
    
public:
    Player();
    Player(Graphics &graphics, Vector2 spawnPoint);
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
    
    //const float getDX() const;
    //const float getDY() const;
    
private:
    float _dx, _dy;
    int _mx, _my; //mouse x and mouse y
    Sprite _fov;
    
    
    //Direction _facing;
    
};

#endif /* player_hpp */
