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
#include <vector>


class Graphics;

class Unit {
    
public:
    Unit();
    ~Unit();
    Unit(Graphics &graphics, Vector2 spawnPoint, bool allie);
    void draw(Graphics &graphics);
    void update(int elapsedTime, Graphics &graphics);
    
    Sprite _unitSprite;
    
    
    
    
    void moveForward();
    void moveBackward();
    void moveRight();
    void moveLeft();
    
    
    /*
    void moveForwardParallax();
    void moveBackwardParallax();
    void moveRightParallax();
    void moveLeftParallax();
    void stopMovingParallax();
    */
    
    
    void moveUpAndRight();
    void moveUpAndLeft();
    void moveDownAndRight();
    void moveDownAndLeft();
    
    
    
    void stopMoving();
    
    void setPlayerAngle(float playerAngle);
    
    void handleTileCollisions();
    
    //float getDX();
    //float getDY();
    
    //void setDXDY(float dx, float dy);
    
    //void setDX(float dx);
    //void setDY(float dy);
    
    //void setX(float x);
    //void setY(float y);
    
    int getStartX();
    int getStartY();
    
    //int getX();
    //int getY();
    
    //void handleTileCollisions(std::vector<Rectangle> &others, float elapsedTime);
    
    //virtual void animationDone(std::string currentAnimation);
    //virtual void setupAnimations();
    
    //void handleTileCollisions(std::vector<Rectangle> &others);
    //void handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics);
    
    //const float getX() const;
    //const float getY() const;
    
    //const float getMX() const;
    //const float getMY() const;
    
    float getStaticX();
    float getStaticY();
    
    float getAngle();
    
    //void changeX(int newX);
    //void changeY(int newY);
    
    void moveToPosition(int posX, int posY);
    
    State getState();

    void handleMovement();
    
    /*
    void moveUpRightParallax();
    void moveUpLeftParallax();
    void moveDownRightParallax();
    void moveDownLeftParallax();
    */
    
    float getPlayerAngle();
    
    void addToMovementOrders(std::vector<Vector2> pos);
    void addToMovementOrders(Vector2 pos);
    void printMovementOrders();
    
    void moveToNextPosition();
    
    Rectangle getCollisionRect();
    
    void addToAngleOrders(double angle);
    void moveToAngle(double angle);
    
private:
    //float _x, _y;
    float _dx, _dy;
    float _angle;
    float _staticdx, _staticdy;
    float _staticx, _staticy;

    float _playerAngle;
    
    int id;
    
    State _state;
    
    float _destinationAngle;
    float _destinationx;
    float _destinationy;
    
    Direction _destinationDirection;
    
    std::vector<Vector2> _unitMovementOrders;
    std::vector<double> _unitAngleOrders;
    
    Rectangle _collisionRect;
    
    
};

#endif /* unit_hpp */
