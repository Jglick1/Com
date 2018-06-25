#include "tile.hpp"
#include "graphics.hpp"
#include <SDL2/SDL.h>

Tile::Tile() {};

Tile::Tile(SDL_Texture * tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position) :
	_tileset(tileset),
	_size(size),
	_tilesetPosition(tilesetPosition),
	_position(Vector2(position.x * globals::LEVEL_SCALE, position.y * globals::LEVEL_SCALE))
{}

void Tile::update(int elapsedTime, float dx, float dy) {

	//if it is a moving tile
	//if negative, round down
	
    
    
	this->_position.x += std::floor(dx * elapsedTime);
    this->_position.y += std::floor(dy * elapsedTime);
	
    
}

void Tile::draw(Graphics &graphics) {
	SDL_Rect destRect = { this->_position.x, this->_position.y, this->_size.x * globals::LEVEL_SCALE, this->_size.y * globals::LEVEL_SCALE };
	SDL_Rect sourceRect = { this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y };
    
	graphics.blitSurface(this->_tileset, &sourceRect, &destRect);
}


