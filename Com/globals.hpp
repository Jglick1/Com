//
//  globals.hpp
//  Com
//
//  Created by Jonah Glick on 6/20/18.
//
//

#ifndef globals_h
#define globals_h

#include <cmath>
#include <vector>
//#include "rectangle.hpp"

class Rectangle;


namespace globals {
    
    const int SCREEN_WIDTH = 1280; //640;
    const int SCREEN_HEIGHT = 800; //480;
    //const float SPRITE_SCALE = 2.0f;
    
    const int LEVEL_SCALE = 1;
    
    const int SPRITE_SCALE = 1;
    
}

namespace sides {
    enum Side {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        NONE
    };
    
    inline Side getOppositeSide(Side side) {
        return
        side == TOP ? BOTTOM :
        side == BOTTOM ? TOP :
        side == LEFT ? RIGHT :
        side == RIGHT ? LEFT :
        NONE;
    }
}


enum ActionState {
    SLIDE_MOVE, PAUSE, NORMAL, COMMAND, ORGANIZATION
};

enum Rank {
    PVT, PFC, CPL, SGT, SSGT, TSGT, MSGT, FSGT
};

enum Role {
    RIFLEMAN, SCOUT_RIFLEMAN, AUTOMATIC_RIFLEMAN, SQUAD_LEADER, ASSISSTANT_SQUAD_LEADER
};

enum Formation {
    FIRETEAM, SQUAD, PLATOON
};

enum Direction {
    LEFT, RIGHT, UP, DOWN, UPRIGHT, UPLEFT, DOWNRIGHT, DOWNLEFT, NONE
};

enum State {
    CHANGE_ANGLE_UP, CHANGE_ANGLE_DOWN, MOVE_FORWARD, STILL, SCAN
};

enum FoWNodePosition {
    BL, BR, TL, TR, IN      //in is for inner nodes
};

enum Weapon {
    MP40, KAR98, M1GARAND
};

enum StructureType {
    WALL, SANDBAG, BRUSH
};

struct Vector2 {
    int x, y;
    Vector2() : x(0), y(0) {}
    Vector2(int x, int y) : x(x), y(y) {}
    Vector2 zero() {
        return Vector2(0, 0);
    }
};

struct CoverNode {
    int x, y;
    bool occupied;
    int id;
    double angle;
    //Direction shootDirection;
    
    CoverNode() : x(0), y(0), angle(0.0) {}
    CoverNode(int x, int y, double angle) : x(x), y(y), angle(angle) {}
    
    //CoverNode() : x(0), y(0), shootDirection(NONE) {}
    //CoverNode(int x, int y, Direction direction) : x(x), y(y), shootDirection(direction) {}
    
};

struct CornerNode {
    int x, y;
    int id;
    Direction directionOfCorner;
    
    CornerNode() : x(0), y(0), directionOfCorner(NONE) {}
    CornerNode(int x, int y, Direction direction) : x(x), y(y), directionOfCorner(direction) {}
    
};

struct FoWNode {
    int x, y;
    FoWNodePosition positionOnRectangle;
    bool isInnerCorner;
    FoWNode() : x(0), y(0), positionOnRectangle(IN) {}
    FoWNode(int x, int y, FoWNodePosition positionOnRectangle, bool isInnerCorner) : x(x), y(y), positionOnRectangle(positionOnRectangle), isInnerCorner(isInnerCorner) {}
};

struct PolygonCorner {
    int x, y;
    double angle;
    bool isExtension;
    bool isInnerCorner;
    
    PolygonCorner(int x, int y, double angle, bool isExtension, bool isInnerCorner) : x(x), y(y), angle(angle), isExtension(isExtension), isInnerCorner(isInnerCorner) {}
    
    bool operator < (const PolygonCorner& plgnCnr) const {
        if(angle == plgnCnr.angle) {
            return (isExtension > plgnCnr.isExtension); //extensions come first
        }
        else {
            return (angle < plgnCnr.angle);
        }
        
        
    }
    
    
};

struct newRectangle {
    double x, y;
    double width, height;
    double angle;
    newRectangle() : x(0), y(0), width(0), height(0), angle(0.0) {}
    newRectangle(double x, double y, double width, double height, double angle) : x(x), y(y), width(width), height(height), angle(angle) {}
};

struct Structure {
    
    
    //std::vector<std::vector<int>> doors;
    std::vector<Vector2> corners;
    double angle;
    std::vector<Direction> directions;
    int numOutsideWalls;
    int name;
    
    StructureType structureType = WALL;

    Structure() : angle(0.0), numOutsideWalls(0), name(0) {}
    Structure(std::vector<Vector2> corners, double angle, std::vector<Direction> directions, int numOutsideWalls, int name) : angle(angle), numOutsideWalls(numOutsideWalls), name(name) {
        this->corners = corners;
        this->directions = directions;
        
        //for (Vector2 &iter : this->_corners) {
        //    printf("%d, %d\n", iter.x, iter.y);
        //}
        
    }
    //Structure(std::vector<std::vector<int>> doors, int name): angle(0.0), numOutsideWalls(0), name(name) {
    //    this->doors = doors;
    //}
    
};

struct Building {
    newRectangle innerds;
    
    std::vector<Vector2> preferredCover;

    int id;
    
    Building(double x, double y, double width, double height, double angle, int id) : id(id) {
        innerds = newRectangle(x, y, width, height, angle);
    }
    
    void addPreferredCover(Vector2 node) {
        preferredCover.push_back(node);
    }
    
};

struct Door {
    int x1, y1;
    int x2, y2;
    Direction direction;
    
    Door() : x1(0), y1(0), x2(0), y2(0), direction(NONE) {}
    
    Door(int x1, int y1, int x2, int y2, Direction direction) : x1(x1), y1(y1), x2(x2), y2(y2), direction(direction) {}
    
    
};

/*
struct Building {
    std::vector<Rectangle> buildingInside;
    std::vector<CoverNode> preferredCover;
    
    Building();

};
*/

#endif /* globals_h */
