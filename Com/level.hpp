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
//#include "door.hpp"

class Graphics;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level {
public:
	Level();
	Level(std::string mapName, Graphics &graphics);
	~Level();
	void update(int elapsedtime);
	void draw(Graphics &graphics);
	
	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
	//std::vector<Door> checkDoorCollisions(const Rectangle &other);
	
	const Vector2 getPlayerSpawnPoint() const;
	
	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();
	void stopMoving();
	
	void handleTileCollisions();
	
	
private:
	std::string _mapName;
	Vector2 _spawnPoint;
	Vector2 _size;
	Vector2 _tileSize;
	SDL_Texture* _backgroundTexture;
	
	std::vector <Tile> _tileList;
	std::vector <Tileset> _tileSets;
	std::vector <Rectangle> _collisionRects;
	
	//std::vector <Door> _doorList;
	
	void loadMap(std::string mapName, Graphics &graphics);
	
	float _dx, _dy;
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
