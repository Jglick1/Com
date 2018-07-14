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
#include "map.hpp"
#include "sprite.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>
//#include "unit.hpp"

using namespace tinyxml2;

namespace player_constants {
    const float WALK_SPEED = 0.2f;
}

Level::Level() {}

Level::Level(std::string mapName, Graphics &graphics) :
    Map(graphics, "/Users/jonahglick/Documents/Com/com_test1.png", 0, 0, 320, 200, 0, 0),
	_mapName(mapName),
	//_spawnPoint(spawnPoint),
	_size(Vector2(0,0)),
	_dx(0.0),
	_dy(0.0),
    _angle(0.0),
    //_position(Vector2(0,0)),
    _positionx(0),
    _positiony(0),
    _transx(0),
    _transy(0),
    _ang(0)
{

	this->loadMap(mapName, graphics);
    
    this->_unit = Unit(graphics, Vector2(0,0));
    
    //this->_unit.moveToPosition(600, 800);

    
    //this->_map = Map(graphics, "/Users/jonahglick/Documents/Com/com_test1.png", 0, 0, 320, 200, 0, 0);
    
    
    //vector of unit types <enemies>
    //vector of unit types <allies>

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
    /*
    for(int i = 0; i < this->_tileList.size(); i++) {
        this->_tileList.at(i).update(elapsedTime, this->_dx, this->_dy);
    }
	*/
    
    //std::cout << this->_dx << " " << this->_dy << std::endl;
    

    //this->_position.x += std::round(this->_dx * elapsedTime);   //the ROUND here very important
    //this->_position.y += std::round(this->_dy * elapsedTime);
    
    this->_positionx += this->_dx * elapsedTime;
    this->_positiony += this->_dy * elapsedTime;
    
    //std::cout << this->_dx << " " << this->_dy << " ";
    
    
    for(int i  = 0; i < this->_collisionRects.size(); i++) {
        this->_collisionRects.at(i).update(elapsedTime, this->_dx, this->_dy);
    }
    /*
    for(int i = 0; i < this->_doorList.size(); i++) {
        this->_doorList.at(i).update(elapsedTime, this->_dx, this->_dy);
    }
    */
    
    //std::cout << this->_angle << std::endl;
    //std::cout << this->_dy << std::endl;

    
    //std::cout << this->_collisionRects.at(0).getX() << " " << this->_collisionRects.at(0).getY() << std::endl;
    
    this->_unit.update(elapsedTime, this->_angle);
    
    
}


void Level::draw(Graphics &graphics) {
    
    
    //this->_transx = this->_position.x;
    //this->_transy = this->_position.y;
    
    
    int posx = std::round(this->_positionx);
    int posy = std::round(this->_positiony);
    

    //this->_map.drawAngle(graphics, this->_position.x, this->_position.y, this->_angle);
    //this->_map.drawTrans(graphics, this->_transx, this->_transy, this->_angle, this->_position.x, this->_position.y, this->_cameraMove);
    
    //Map::drawTrans(graphics, this->_transx, this->_transy, this->_angle, this->_position.x, this->_position.y, this->_cameraMove);
    
    Map::drawTrans(graphics, posx, posy, this->_angle, posx, posy, this->_cameraMove);
    
    
    for(int i = 0; i<this->_collisionRects.size(); i++) {
        graphics.drawRect(std::round(this->_collisionRects.at(i).getX()), std::round(this->_collisionRects.at(i).getY()), this->_collisionRects.at(i).getWidth(), this->_collisionRects.at(i).getHeight());
    }
    
    //std::cout <<this->_collisionRects.at(0).getX() << " " << this->_collisionRects.at(0).getY() << std::endl;
    
    //std::cout << this->_positionx << " " << this->_positiony << std::endl;
    
    //graphics.drawLine(-this->_positionx + 640, -this->_positiony + 400, -this->_positionx + 640 + 100*std::sin(this->_angle*3.14159/180), -this->_positiony + 400 - 100*std::cos(this->_angle*3.14159/180) );
    
    graphics.drawLine(640, 400, 640 - 100*std::sin(this->_angle*3.14159/180), 400 - 100*std::cos(this->_angle*3.14159/180) );
    
    float m = (400 - (400 - 100*std::cos(this->_angle*3.14159/180)))/(640-(640 - 100*std::sin(this->_angle*3.14159/180)));
    
    float b = 400 - m*640;
    
    
    
    //std::cout << m << std::endl;
    
    //std::cout << this->_angle << std::endl;
    
    //graphics.drawPoint(400, 400);
    
    //float collisionx = (this->_collisionRects.at(0).getY() - b)/m;
    
    //float collisiony = this->_collisionRects.at(0).getX() * m + b;
    
    //graphics.drawRect(400 - 5, 400-5, 10, 10);
    
    //graphics.drawRect(this->_collisionRects.at(0).getX() - 5, collisiony-5, 10, 10);
    
    /*
    //the top
    if(collisionx > this->_collisionRects.at(0).getX() && collisionx < this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth()) {
        graphics.drawRect(collisionx - 5, this->_collisionRects.at(0).getY()-5, 10, 10);
    }
    */
    /*
    if((this->_collisionRects.at(0).getY() + this->_positiony < 0) && this->_angle < 0) {
        //player is below the rectangle
    }
     */
    
    //check bottom
    float collisionx = (this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight() - b)/m;
    float collisiony = this->_collisionRects.at(0).getX() * m + b;
    if((this->_collisionRects.at(0).getY() < 400-this->_collisionRects.at(0).getHeight() && std::abs(this->_angle) < 90)) {
        if(collisionx > this->_collisionRects.at(0).getX() && collisionx < this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth()) {
            graphics.drawRect(collisionx - 5, this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight()-5, 10, 10);
        }
    }
    
    //std::cout << this->_collisionRects.at(0).getY() << " " << this->_positiony <<std::endl;
    
    //check top                                         //if angle is over 2 pi, subtract 2 pi
    collisionx = (this->_collisionRects.at(0).getY() - b)/m;
    collisiony = this->_collisionRects.at(0).getX() * m + b;
    if((this->_collisionRects.at(0).getY() > 400 && std::abs(this->_angle) > 90)) {
        if(collisionx > this->_collisionRects.at(0).getX() && collisionx < this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth()) {
            graphics.drawRect(collisionx - 5, this->_collisionRects.at(0).getY()-5, 10, 10);
        }
    }
    
    //std::cout << this->_collisionRects.at(0).getY() << " " << this->_positiony <<std::endl;
    
    //check left
    collisionx = (this->_collisionRects.at(0).getY() - b)/m;
    collisiony = this->_collisionRects.at(0).getX() * m + b;
    //if((this->_collisionRects.at(0).getX() + this->_positionx > 0 && std::abs(this->_angle) > 90)) {
    if((this->_collisionRects.at(0).getX() > 640) && this->_angle < 0) {
        if(collisiony > this->_collisionRects.at(0).getY() && collisiony < this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight()) {
            graphics.drawRect(this->_collisionRects.at(0).getX() - 5, collisiony-5, 10, 10);
        }
    }
    
    //std::cout << this->_collisionRects.at(0).getX() <<std::endl;
    //std::cout << this->_angle << std::endl;
    
    //check right
    collisionx = (this->_collisionRects.at(0).getY() - b)/m;
    collisiony = (this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth()) * m + b;
    //if((this->_collisionRects.at(0).getX() + this->_positionx > 0 && std::abs(this->_angle) > 90)) {
    if((this->_collisionRects.at(0).getX() < 640 - this->_collisionRects.at(0).getWidth()) && this->_angle > 0) {
        if(collisiony > this->_collisionRects.at(0).getY() && collisiony < this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight()) {
            graphics.drawRect(this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth() - 5, collisiony-5, 10, 10);
        }
    }
    
    //std::cout << this->_collisionRects.at(0).getX() << " " << this->_positionx <<std::endl;
    
    
    //graphics.drawLine(0, b, -b/m, 0);
    
    this->_unit.draw(graphics);
    
    float xEquivalent = this->_unit.getStaticX() + this->_unit.getX()+15;
    float yEquivalent = this->_unit.getStaticY() + this->_unit.getY()+30;
    float angleEquivalent = this->_unit.getAngle();
    
    //in both cases we change the - sin for x into a + sin
    graphics.drawLine(xEquivalent, yEquivalent, xEquivalent + 100*std::sin(angleEquivalent*3.14159/180), yEquivalent - 100*std::cos(angleEquivalent*3.14159/180) );
    
    m = (yEquivalent - (yEquivalent - 100*std::cos(angleEquivalent*3.14159/180)))/(xEquivalent-(xEquivalent + 100*std::sin(angleEquivalent*3.14159/180)));
    
    b = yEquivalent - m*xEquivalent;
    
    //graphics.drawLine(0, b, -b/m, 0);

    collisionx = (this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight() - b)/m;
    collisiony = this->_collisionRects.at(0).getX() * m + b;
    if((this->_collisionRects.at(0).getY() < yEquivalent-this->_collisionRects.at(0).getHeight() && std::abs(angleEquivalent) < 90)) {
        if(collisionx > this->_collisionRects.at(0).getX() && collisionx < this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth()) {
            graphics.drawRect(collisionx - 5, this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight()-5, 10, 10);
        }
    }
    
    //check top                                         //if angle is over 2 pi, subtract 2 pi
    collisionx = (this->_collisionRects.at(0).getY() - b)/m;
    collisiony = this->_collisionRects.at(0).getX() * m + b;
    if((this->_collisionRects.at(0).getY() > yEquivalent && std::abs(angleEquivalent) > 90)) {
        if(collisionx > this->_collisionRects.at(0).getX() && collisionx < this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth()) {
            graphics.drawRect(collisionx - 5, this->_collisionRects.at(0).getY()-5, 10, 10);
        }
    }
    
    //check left
    collisionx = (this->_collisionRects.at(0).getY() - b)/m;
    collisiony = this->_collisionRects.at(0).getX() * m + b;
    if((this->_collisionRects.at(0).getX() > xEquivalent) && angleEquivalent < 0) {
        if(collisiony > this->_collisionRects.at(0).getY() && collisiony < this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight()) {
            graphics.drawRect(this->_collisionRects.at(0).getX() - 5, collisiony-5, 10, 10);
        }
    }

    
    //check right
    collisionx = (this->_collisionRects.at(0).getY() - b)/m;
    collisiony = (this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth()) * m + b;
    if((this->_collisionRects.at(0).getX() < xEquivalent - this->_collisionRects.at(0).getWidth()) && angleEquivalent > 0) {
        if(collisiony > this->_collisionRects.at(0).getY() && collisiony < this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight()) {
            graphics.drawRect(this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth() - 5, collisiony-5, 10, 10);
        }
    }
    
    
    
}


std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) { //other is player
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;                                                          //returning level collision rects
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


/*
void Level::moveForward() {
    this->_dx = 0.0;
    this->_dy = -player_constants::WALK_SPEED;
}

void Level::moveBackward() {
    this->_dx = 0.0;
    this->_dy = player_constants::WALK_SPEED;
}


void Level::moveRight() {
    this->_dx = player_constants::WALK_SPEED;
    this->_dy = 0.0;
}

void Level::moveLeft() {
    this->_dx = -player_constants::WALK_SPEED;
    this->_dy = 0.0;
}
*/




void Level::moveForward() {
    this->_dx = -player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
    this->_dy = -player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
    this->_unit.moveForwardParallax();
}

void Level::moveBackward() {
    this->_dx = player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
    this->_dy = player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
    this->_unit.moveBackwardParallax();
}


void Level::moveRight() {
    this->_dx = player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
    this->_dy = -player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
    this->_unit.moveRightParallax();
}

void Level::moveLeft() {
    this->_dx = -player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
    this->_dy = player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
    this->_unit.moveLeftParallax();
}


void Level::cameraMove() {
    this->_cameraMove = 1;
}

void Level::cameraStill() {
    this->_cameraMove = 0;
}



void Level::stopMoving() {
	this->_dx = 0.0f;
	this->_dy = 0.0f;
    this->_unit.stopMovingParallax();
}


/*
void Level::handleTileCollisions(std::vector<Rectangle> &others, Direction &inPower) {
	//this->_dx = 0.0;
	//this->_dy = 0.0;
    
    
    //std::cout << this->_position.x << " " << this->_position.y << std::endl;
    //collide up y = -328
    //collid down y = -257
    
    //collide right x = 47
    //collide left x = -46
}
*/


void Level::handleTileCollisions(std::vector<Rectangle> &others, float elapsedTime) { //other are the level's collision rects
    //rectangle for the player
    //Rectangle playerRec = Rectangle(625, 375, 30, 40);          //collsion rectangle for player
    
    Rectangle playerRec = Rectangle(625, 385, 30, 30);
    
    //this->_position.x -= std::round(elapsedTime * this->_dx);
    //this->_position.y -= std::round(elapsedTime * this->_dy);
    
    //this->_dx = 0.0;
    //this->_dy = 0.0;
    
    //std::cout << this->_dx << " " << this->_dy << std::endl;
    
    for (int i = 0; i < others.size(); i++) {
        sides::Side collisionSide = playerRec.getCollisionSide(others.at(i));
        
        if (collisionSide != sides::NONE) {
            switch (collisionSide) {
                case sides::TOP:                    //top refers to player's box
                    this->changeY(-(others.at(i).getStartY() + others.at(i).getHeight()) + 385, 385 - others.at(i).getHeight());
                    //std::cout << others.at(i).getStartY() << " " << others.at(i).getHeight() <<std::endl;
                    this->_dy = 0.0f;
                    
                    //unit.setY(unit.getStartY() - 385 + 66); //maybe 65
                    //unit.setDY(0.0f);
                    //unit.moveForward();

                    
                    break;
                case sides::BOTTOM:
                    this->changeY(-(others.at(i).getStartY()) + 385 + 30, 385 + 30);
                    this->_dy = 0.0f;
                    

                    
                    //unit.setY(unit.getStartY() - 385 + 128); //also not exactly right
                    //unit.setDY(0.0f);
                    
                    break;
                case sides::LEFT:
                    this->changeX((-others.at(i).getStartX()) + 625 - others.at(i).getWidth(), 625 - others.at(i).getWidth());
                    this->_dx = 0.0f;
                    

                    
                    //unit.setX(unit.getStartX() - 48);
                    //unit.setDX(0.0f);
                    
                    break;
                case sides::RIGHT:
                    this->changeX((-others.at(i).getStartX()) + 625 + 30, 625 + 30);
                    this->_dx = 0.0f;
                    
                    
                    //unit.setX(unit.getStartX() + 48);
                    //unit.setDX(0.0f);
                    
                    break;
            }
        }

        
    }
    
    
    this->_unit.handleTileCollisions(others, elapsedTime);
    
    
    
}





void Level::changeY(int newY, int newCollisionY) {
    this->_positiony = newY;
    for(int i = 0; i < this->_collisionRects.size(); i++) {
        this->_collisionRects.at(i).changeY(newCollisionY);
    }
}

void Level::changeX(int newX, int newCollisionX) {
    this->_positionx = newX;
    for(int i = 0; i < this->_collisionRects.size(); i++) {
        this->_collisionRects.at(i).changeX(newCollisionX);
    }
}


void Level::changeAngle(float angle) {
    this->_angle += angle;
    if(this->_angle > 180) {
        this->_angle = this->_angle - 360;
    }
    else if(this->_angle < -180) {
        this->_angle = this->_angle + 360;
    }
}

float Level::getAngle() {
    return this->_angle;
}

float Level::getDX() {
    return this->_dx;
}

float Level::getDY() {
    return this->_dy;
}

void Level::setUnitAngle() {
    this->_unit.setPlayerAngle(this->_angle);
}

void Level::handleUnitMovement() {
    this->_unit.handleMovement();
}

void Level::moveUnitToPosition(int posX, int posY) {
    this->_unit.moveToPosition(posX, posY);
}

