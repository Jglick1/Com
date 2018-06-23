//
//  level.cpp
//  Io
//
//  Created by Jonah Glick on 6/4/17.
//  Copyright © 2017 Jonah Glick. All rights reserved.
//

#include "level.hpp"
#include "graphics.hpp"
#include <SDL2/SDL.h>
#include "globals.hpp"
#include "tinyxml2.h"
#include "rectangle.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Graphics &graphics) :
	_mapName(mapName),
	//_spawnPoint(spawnPoint),
	_size(Vector2(0,0)),
	_dx(0.0),
	_dy(0.0)
{
	//std::cout << "loading map for " + mapName << std::endl;
	this->loadMap(mapName, graphics);
	//std::cout << "end of loading map for" + mapName << std::endl;
}

Level::~Level() {}

/*
void Level::loadMap(std::string mapName, Graphics &graphics) {
	this->_backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("/Users/jonahglick/Documents/Io/test.png"));
	this->_size = Vector2(1280, 960);
}
*/

void Level::loadMap(std::string mapName, Graphics &graphics) {
	//this->_backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("C:\\Users\\Jglick\\Documents\\Io\\Test.png"));
	//this->_size = Vector2(1280, 960);
	
	XMLDocument doc;
	std::stringstream ss;
	ss << mapName << ".tmx";
	doc.LoadFile(ss.str().c_str());
	
	XMLElement * mapNode = doc.FirstChildElement("map");
	
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	//std::cout << "width: " << width << std::endl;
	mapNode->QueryIntAttribute("height", &height);
	//std::cout << "height: " << height << std::endl;
	this->_size = Vector2(width, height);
	
	int tileWidth, tileHeight;

	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	//std::cout << "tileWidth: " << tileWidth << std::endl;
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	//std::cout << "tileHeight: " << tileHeight << std::endl;
	
	this->_tileSize = Vector2(tileWidth, tileHeight);
	
	XMLElement * pTileset = mapNode->FirstChildElement("tileset");
	
	//loading the tileset
	if (pTileset != NULL) {
		while (pTileset) {
			int firstgid;
			//const char * source = pTileset->FirstChildElement("image")->Attribute("source");
			const char * tsxSource = pTileset->Attribute("source");
			
			XMLDocument doc1;
			std::stringstream ss1;
			ss1 << tsxSource;
			doc1.LoadFile(ss1.str().c_str());
            
            //std::cout << ss1.str() << std::endl;
			
			
			//std::cout << "	tileset stuff:" + ss1.str() << std::endl;
			
			XMLElement * mapNode1 = doc1.FirstChildElement("tileset");
            
            
			
			const char * source = mapNode1->FirstChildElement("image")->Attribute("source");
			
			//std::cout << "	tileset stuff source:";
			//std::cout << source << std::endl;
			
			//char * path;
			std::stringstream ss2;
			ss2 << source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			
			//std::cout << "first gid: ";
			//std::cout << firstgid << std::endl;
			
			SDL_Texture * tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss2.str()));
			this->_tileSets.push_back(Tileset(tex, firstgid));
			
			//std::cout << "tileSets size: ";
			//std::cout << _tileSets.size() << std::endl;
			
			pTileset = pTileset->NextSiblingElement("tileset"); //come back to this (32:30, ep. 9)

		}
	}
	
	//loading the layers
	XMLElement *pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			//loading the data element
			XMLElement * pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					//loading the tile element
					XMLElement * pTile = pData->FirstChildElement("tile");
					
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							//build each tile, if gid is 0, no tile is drawn
							
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}
							//get the tileset for this gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							/*												THIS I CHANGED.
							for (int i = 0; i < this->_tileSets.size(); i++) {
								if (this->_tileSets[i].FirstGid <= gid) {
									std::cout << "tileSet for gid ";
									std::cout << gid;
									std::cout << " = ";
									std::cout << i << std::endl;
									
									
									//this is the tileset we want
									tls = this->_tileSets.at(i);
									break;
								}
							}
							*/
							for (int i = (this->_tileSets.size()-1); i >= 0; i--) {
								if (this->_tileSets[i].FirstGid <= gid) {
									//std::cout << "tileSet for gid ";
									//std::cout << gid;
									//std::cout << " = ";
									//std::cout << i << std::endl;
									
									
									//this is the tileset we want
									tls = this->_tileSets.at(i);
									break;
								}
							}
							
							if (tls.FirstGid == -1) {
								//no tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}
							
							
							//get the position of the tile in the level
							
							int xx = 0;
							int yy = 0;
							
							xx = tileCounter % width;
							xx *= tileWidth;
							
							yy += tileHeight * (tileCounter / width);
							
							Vector2 finalTilePosition = Vector2(xx, yy);
							
							//std::cout << "xx: ";
							//std::cout << xx << std::endl;
							//std::cout << "yy: ";
							//std::cout << yy << std::endl;
							//std::cout << std::endl;
							
							//Calculate the position of the tile in the tileset (the png file)
							
							int tilesetWidth, tilesetHeight;						//CHANGE THIS!!! (56:00)
							SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
							
							//std::cout << "tilesetWidth: ";
							//std::cout << tileWidth << std::endl;
							//std::cout << "tilesetHeight: ";
							//std::cout << tileHeight << std::endl;
							//std::cout << std::endl;
							
							int tsxx = (gid - (tls.FirstGid - 1)) % (tilesetWidth / tileWidth) - 1;
							
							//std::cout << "tsxx: ";
							//std::cout << tsxx << std::endl;
							
							tsxx *= tileWidth;
							
							int tsyy = 0;
							int amt = ((gid - (tls.FirstGid - 1)) / (tilesetWidth / tileWidth));
							tsyy = tileHeight * amt;
							
							//std::cout << "tsxx: ";
							//std::cout << tsxx << std::endl;
							//std::cout << "tsyy: ";
							//std::cout << tsyy << std::endl;
							//std::cout << std::endl;
							
							Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);
							
							//build the actual tile and add it to the level's tile list
							Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
							
							this->_tileList.push_back(tile);
							tileCounter++;
							
							pTile = pTile->NextSiblingElement("tile");
						}
						
					}
					
					pData = pData->NextSiblingElement("data");
				}
			}
			
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}
	
	//collisions
	
	XMLElement * pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			
			const char * name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			
			if (ss.str() == "collisions") {
				XMLElement * pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						
						float x, y, width, height;
						
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						
						this->_collisionRects.push_back(Rectangle(int(x) * globals::LEVEL_SCALE, int(y) * globals::LEVEL_SCALE, int(width) * globals::LEVEL_SCALE, int(height) * globals::LEVEL_SCALE));
						
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "spawn points") {
				XMLElement * pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char * name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "player") {
							this->_spawnPoint = Vector2(int(x) * globals::LEVEL_SCALE, int(y) * globals::LEVEL_SCALE);
						}
						
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			/*
			else if (ss.str() == "doors") {
				XMLElement * pObject = pObjectGroup->FirstChildElement("object");
				if(pObject != NULL) {
					while(pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float w = pObject->FloatAttribute("width");
						float h = pObject->FloatAttribute("height");
						
						Rectangle rect = Rectangle(x, y, w, h);
						
						XMLElement* pProperties = pObject->FirstChildElement("properties");
						if(pProperties != NULL) {
							while(pProperties) {
								XMLElement* pProperty = pProperties->FirstChildElement("property");
								while(pProperty) {
									const char* name = pProperty->Attribute("name");
									std::stringstream ss;
									ss << name;
									if(ss.str() == "destination") {
										const char* value = pProperty->Attribute("value");
										std::stringstream ss2;
										ss2 << value;
										Door door = Door(rect, ss2.str());
										this->_doorList.push_back(door);
									}
									
									
									pProperty = pProperty->NextSiblingElement("property");
								}
								pProperties = pProperties->NextSiblingElement("properties");
							}
						}
						
						
						pObject = pObject->NextSiblingElement("object");
						
					}
				}
					
			}
			*/
			
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
	
	
}

void Level::update(int elapsedTime) {
	//std::cout << "dx: " << this->_dx << " dy: " << this->_dy << std::endl;

    for(int i = 0; i < this->_tileList.size(); i++) {
        this->_tileList.at(i).update(elapsedTime, this->_dx, this->_dy);
    }
	
    for(int i  = 0; i < this->_collisionRects.size(); i++) {
        this->_collisionRects.at(i).update(elapsedTime, this->_dx, this->_dy);
    }
    /*
    for(int i = 0; i < this->_doorList.size(); i++) {
        this->_doorList.at(i).update(elapsedTime, this->_dx, this->_dy);
    }
    */
	

}


void Level::draw(Graphics &graphics) {
    //std::cout << this->_tileList.size() << std::endl;
    
	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}
	
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;
}

/*
std::vector<Door> Level::checkDoorCollisions(const Rectangle &other) {
	std::vector<Door> others;
	for(int i = 0; i<this->_doorList.size(); i++) {
		if(this->_doorList.at(i).collidesWith(other)) {
			others.push_back(this->_doorList.at(i));
		}
	}
	return others;
}
*/


const Vector2 Level::getPlayerSpawnPoint() const {
	return this->_spawnPoint;
}

void Level::moveUp() {
	this->_dy = -0.2f;
	this->_dx = 0.0f;
}

void Level::moveDown() {
	this->_dy = 0.2f;
	this->_dx = 0.0f;
}

void Level::moveLeft() {
	this->_dx = -0.2f;
	this->_dy = 0.0f;
}

void Level::moveRight() {
	this->_dx = 0.2f;
	this->_dy = 0.0f;
}

void Level::stopMoving() {
	this->_dx = 0.0f;
	this->_dy = 0.0f;
}

void Level::handleTileCollisions() {
	this->_dx = 0;
	this->_dy = 0;
}

