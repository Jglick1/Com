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
    void draw(Graphics &graphics, bool isSelected);
    
    
    void update(int elapsedTime, Graphics &graphics);

    
    
    void stopMoving();
    
    void setPlayerAngle(float playerAngle);
    
    void handleTileCollisions();
    

    
    int getStartX();
    int getStartY();
    

    
    float getStaticX();
    float getStaticY();
    
    float getAngle();

    
    void moveToPosition(int posX, int posY);
    
    State getState();

    void handleMovement();

    
    float getPlayerAngle();
    
    void addToMovementOrders(std::vector<Vector2> pos);
    void addToMovementOrders(Vector2 pos);
    void printMovementOrders();
    
    void moveToNextPosition();
    
    Rectangle getCollisionRect();
    
    void addToAngleOrders(double angle);
    void moveToAngle(double angle);
    
    void printFirstName();
    
private:
    //float _x, _y;
    float _dx, _dy;
    float _angle;
    float _staticdx, _staticdy;
    float _staticx, _staticy;

    float _playerAngle;
    
    int id;
    
    State _state;
    
    bool _isMoving;
    bool _isAngleMoving;
    
    
    float _destinationAngle;
    float _destinationx;
    float _destinationy;
    
    double _distTraveled;
    double _distToNextPoint;
    
    
    Direction _destinationDirection;
    
    std::vector<Vector2> _unitMovementOrders;
    std::vector<double> _unitAngleOrders;
    
    //Rectangle _collisionRect;
    
    Sprite _unitSprite;
    Sprite _unitSelected;
    
    std::string _firstName;
    std::string _lastName;
    
    
};

#endif /* unit_hpp */
