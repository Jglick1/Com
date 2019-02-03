//
//  level.hpp
//  Io
//
//  Created by Jonah Glick on 6/4/17.
//  Copyright © 2017 Jonah Glick. All rights reserved.
//

#ifndef level_hpp
#define level_hpp

#include "globals.hpp"
#include <string>
#include "tile.hpp"
#include <vector>
#include "rectangle.hpp"
#include "sprite.hpp"
#include "map.hpp"
#include "unit.hpp"
//#include "door.hpp"
#include "controlSlide.hpp"
#include "graph.hpp"
#include "gunshotPath.hpp"
#include "fireteam.hpp"

class Graphics;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level : public Map{
public:
	Level();
	Level(std::string mapName, Graphics &graphics);
	~Level();
	void update(int elapsedtime, Graphics &graphics);
	void draw(Graphics &graphics);
	
    
    
	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
	
	const Vector2 getPlayerSpawnPoint() const;
	

	
    void handleTileCollisions(std::vector<Rectangle> &others, float elapsedTime, Graphics &graphics);
    
    void changeAngle(float angle);

    
    
    
    
    float getAngle();
    
    void changeY(int newY, int newCollisionY);
    void changeX(int newX, int newCollisionX);
    
    
    
    void setUnitAngle();
    
    void handleUnitMovement();
    
    void moveUnitToPosition(int posX, int posY, Graphics &graphics);

    
    
    
    bool checkSlideCollision(int xm, int ym);
    void handleSlideMovement(int xm, int ym, Graphics &graphics, bool once);
    
    void centerSlideToZero();
    
    //bool checkPathCollisionHelp(double beginx, double beginy, double angle, double distanceToEnd);
    //bool checkPathCollisionHelp2(double beginx, double beginy, double m, double b, double distanceToEnd, double angle);
    //bool checkPathCollision(int beginx, int beginy, int endx, int endy, Graphics &graphics);
    
    //Vector2 checkShotCollision(double beginx, double beginy, double angle);

    void moveUnitToSlidePosition(Graphics &graphics);
    void moveUnitToNearestCover(Graphics &graphics);
    
    
    
    
    
    
    
    void playerFireShot(Graphics &graphics);
    
    //Vector2 checkShotCollisionNew(double beginx, double beginy, double angle);
    
    void clearGunshotPaths();
    
    void moveUnitAngleToSlideAngle(Graphics &graphics);
    
    
    
    
    
    
    bool checkPathCollision(int beginx, int beginy, int endx, int endy, Graphics &graphics);
    
    
    
    
    
    
    void handleSlideRelease(int x, int y, Graphics &graphics);
    
    void drawFogOfWar(Graphics &graphics);
    
    void drawPolygonCorners(Graphics &graphics);
    
    bool arePointsVeryClose(Vector2 node1, Vector2 node2);
    
    Vector2 checkShotCollisionFoW(double beginx, double beginy, double angle, double endx, double endy);
    
    FoWNodePosition intToNodePosition(int position);
    
    bool shouldNextCollisionBeIncluded(double playerX, double playerY, FoWNode iter);
    
    bool checkShotCollisionFoWCorner(double beginx, double beginy, double angle, double endx, double endy);
    
    void changeDrawFoVNode();
    
    
    
    bool isLineLineCollision(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
    Vector2 whereLineLineCollision(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
    double distToLine(double playerX, double playerY, double x1, double y1, double x2, double y2);
    
    
    
    
    
    Vector2 checkShotCollisionNewNew(double beginx, double beginy, double endx, double endy, Graphics &graphics);
    
    
    
    
    
    bool approxEqual(double x1, double x2);
    
    
    bool closestPointOnLine(double x1, double y1, double x2, double y2, double px, double py, double &closestx, double &closesty, Direction direction, int fireteamSize);
    
    
    void handlePlayerCollisions(double elapsedTime, Graphics &graphics);
    bool lineCircle(double x1, double y1, double x2, double y2, double cx, double cy, double r, double &closestX, double &closestY);
    bool pointCircle(double px, double py, double cx, double cy, double r);
    bool linePoint(double x1, double y1, double x2, double y2, double px, double py);
    double dist(double x1, double y1, double x2, double y2);
    Direction findWallDirection(double x1, double y1, double x2, double y2);
    void printDirection(Direction direction);
    
    
    
    
    
    
    
    
    
private:
    //Map _map;
	std::string _mapName;
	Vector2 _spawnPoint;
	Vector2 _size;
	Vector2 _tileSize;
	SDL_Texture* _backgroundTexture;
	
	std::vector <Tile> _tileList;
	std::vector <Tileset> _tileSets;
	std::vector <Rectangle> _collisionRects;
	

	
	void loadMap(std::string mapName, Graphics &graphics);
	

    float _angle;

    
    Unit _unit;
    
    std::vector<Unit> _units;
    
    ControlSlide _slide;
    
    Graph _graph;

    std::vector<GunshotPath> _gunShotPaths;
    
    std::vector<Building> _buildings;
    
    
    
    
    std::vector<CornerNode> _cornerNodes;
    std::vector<PolygonCorner> _polygonCorners;
    //need to know the angle for each one
    //need to know if they are extensions of a FoWNode
        //start the sorting at those points
    std::vector<FoWNode> _FoWNode;
    bool _drawFovNode;
    
    
    
    
    
    
    Fireteam _fireteam;
    Fireteam _fireteam2;
    Fireteam _fireteam3;
    
    Fireteam _enemyFireteam;
    
    std::vector<Structure> _structures;
    std::vector<Door> _doors;
    
    
    std::vector<Vector2> _unitMovePositions;

    
    
};



struct Tileset {
	SDL_Texture * Texture;
	int FirstGid;
	
	Tileset() {
		this->FirstGid = -1;
	}
	
	Tileset(SDL_Texture * texture, int firstGid) {
		this->Texture = texture;
		this->FirstGid = firstGid;
	}
};
 


#endif /* level_hpp */
