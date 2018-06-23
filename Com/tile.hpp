//
//  tile.hpp
//  Io
//
//  Created by Jonah Glick on 12/22/17.
//  Copyright © 2017 Jonah Glick. All rights reserved.
//

#ifndef tile_h
#define tile_h

#include "globals.hpp"
#include <cmath>
//#include "player.hpp"


struct SDL_Texture;
class Graphics;
//class Player;

class Tile {
public:
	Tile();
	Tile(SDL_Texture * _tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);
	void update(int elapsedTime, float dx, float dy);
	void draw(Graphics &graphics);
private:
	SDL_Texture * _tileset;
	Vector2 _size;
	Vector2 _tilesetPosition;
	Vector2 _position;
	
};


#endif /* tile_h */
