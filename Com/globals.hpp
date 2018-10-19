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
    CHANGE_ANGLE_UP, CHANGE_ANGLE_DOWN, MOVE_FORWARD, STILL
};

enum FoWNodePosition {
    BL, BR, TL, TR, IN      //in is for inner nodes
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
    FoWNode() : x(0), y(0), positionOnRectangle(IN) {}
    FoWNode(int x, int y, FoWNodePosition positionOnRectangle) : x(x), y(y), positionOnRectangle(positionOnRectangle) {}
};

struct PolygonCorner {
    int x, y;
    double angle;
    bool isExtension;
    
    PolygonCorner(int x, int y, double angle, bool isExtension) : x(x), y(y), angle(angle), isExtension(isExtension) {}
    
};

/*
struct Edge;

struct Vertex {
    
    Edge * _edges;
    
};

struct Edge {
    
    Vertex * _vertex;
    
};
*/

#endif /* globals_h */
