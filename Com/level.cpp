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
#include <math.h>

using namespace tinyxml2;

namespace player_constants {
    const float WALK_SPEED = 0.2f;
}

Level::Level() {
    //printf("level default constructor\n");
}

Level::Level(std::string mapName, Graphics &graphics) :
    Map(graphics, "/Users/jonahglick/Documents/Com/com_test2.png", 0, 0, 1280, 800, 0, 0),
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
    //printf("level contructor\n");
    
	this->loadMap(mapName, graphics);
    
    this->_unit = Unit(graphics, Vector2(0,0));
    

    
    
    
    //this->_unit.moveToPosition(600, 800);

    
    //this->_map = Map(graphics, "/Users/jonahglick/Documents/Com/com_test1.png", 0, 0, 320, 200, 0, 0);
    
    
    //vector of unit types <enemies>
    //vector of unit types <allies>
    //printf("test\n");
    
    
    this->_slide = ControlSlide(graphics);
    
    //this->_building.push_back(Vector2(1,2));
    
    //printf("test\n");
    

}

Level::~Level() {//printf("level destructor\n");
}

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
            
            //this->_graph = Graph(4);
            
            //Graph test = Graph(4);
            
            //Graph test(1);
            
            
            
            else if (ss.str() == "corners") {
                
                int vertexCount = 0;
                
                XMLElement * pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        vertexCount++;
                        pObject = pObject->NextSiblingElement("object");
                    }
                }
                
                this->_graph = Graph(vertexCount+2); //+2 for source and destination nodes
                
                printf("vertex count: %d\n", vertexCount);
                
                pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        
                        int id = pObject->IntAttribute("name");
                        int x = pObject->IntAttribute("x");
                        int y = pObject->IntAttribute("y");
                        this->_graph.addToVertexTable(id, x, y);
                        
                        
                        //int idNext = pObject->IntAttribute("edge")
                        

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
                
                pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        
                        
                        int id = pObject->IntAttribute("name");
                        XMLElement * pProperty1 = pObject->FirstChildElement("properties");
                        XMLElement * pProperty2 = pProperty1->FirstChildElement("property");
                        int otherId = pProperty2->IntAttribute("name"); //the vertex to connect to
                        
                        //this->_map[id].y - this->_map[otherId].y
                        
                        double weight = this->_graph.getWeight(id, otherId);
                        
                        //double weight = 0.0;
                        
                        this->_graph.addEdge(id, otherId, weight);
                        printf("node: %d to %d weight: %f\n", id, otherId, weight);
                        
                        pObject = pObject->NextSiblingElement("object");
                    }
                }
                
                
            }
            
			
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
    
    //printf("end load map\n");
    
    //this->_graph.printAdjacencyMatrix();
    
    
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
    this->_slide.update(elapsedTime);
    
    
}


void Level::draw(Graphics &graphics) {

    int posx = std::round(this->_positionx);
    int posy = std::round(this->_positiony);
    
    //printf("x: %f, y: %f\n", this->_unit.getStaticX(), this->_unit.getStaticY());
    
    
    Map::drawTrans(graphics, posx, posy, this->_angle, posx, posy, this->_cameraMove);
    
    //draw collision rects
    for(int i = 0; i<this->_collisionRects.size(); i++) {
        graphics.drawRect(std::round(this->_collisionRects.at(i).getX()), std::round(this->_collisionRects.at(i).getY()), this->_collisionRects.at(i).getWidth(), this->_collisionRects.at(i).getHeight());
    }
    
    //draw unit collision rects
    Rectangle unitRec = this->_unit.getCollisionRect();

    graphics.drawRect(std::round(unitRec.getX()), std::round(unitRec.getY()), unitRec.getWidth(), unitRec.getHeight());
    
    
    
    
    
    graphics.drawLine(640, 400, 640 - 100*std::sin(this->_angle*3.14159/180), 400 - 100*std::cos(this->_angle*3.14159/180) );
    
    //graphics.drawLine(10,10,800,800);
    
    
    Vector2 coll = checkShotCollision(640, 400, this->_angle);
    
    if(!((coll.x == 0) && (coll.y == 0))) {                         //CHNAGE THIS TO NOT BE 0!
        graphics.drawRect(coll.x - 5, coll.y - 5, 10, 10);
    }
    
    
    /*
    std::map<int, Vector2> vertices = this->_graph.getVertexTable();
    Vector2 temp;
    for(const auto& iter : vertices) {
        temp = checkPathCollision(this->_unit.getStaticX()+this->_unit.getX(), this->_unit.getStaticY()+this->_unit.getY(), iter.second.x, iter.second.y);
    
        //if(!(temp.x == 0)&&(temp.y == 0)) {

            graphics.drawRect(temp.x-5, temp.y-5, 10, 10);
            
        //}
    }
    */
    
    
    
    
    //unit
    this->_unit.draw(graphics);
    
    float xEquivalent = this->_unit.getStaticX() + this->_unit.getX()+8;
    float yEquivalent = this->_unit.getStaticY() + this->_unit.getY()+12;
    float angleEquivalent = -this->_unit.getAngle();    //the opposite angle of level
    
    //in both cases we change the - sin for x into a + sin
    graphics.drawLine(xEquivalent, yEquivalent, xEquivalent - 100*std::sin(angleEquivalent*3.14159/180), yEquivalent - 100*std::cos(angleEquivalent*3.14159/180) );

    
    coll = checkShotCollision(xEquivalent, yEquivalent, angleEquivalent);
    
    
    if(!((coll.x == 0) && (coll.y == 0))) {                         //CHNAGE THIS TO NOT BE 0!
        graphics.drawRect(coll.x - 5, coll.y - 5, 10, 10);
    }

    
    //slide
    this->_slide.draw(graphics);
    
    
}


std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) { //other is player
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
    //printf("%d, %d\n", this->_collisionRects.at(0).getY() + this->_collisionRects.at(0).getHeight()*0, other.getY());
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
    this->_slide.moveForwardParallax();
}

void Level::moveBackward() {
    this->_dx = player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
    this->_dy = player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
    this->_unit.moveBackwardParallax();
    this->_slide.moveBackwardParallax();
}


void Level::moveRight() {
    this->_dx = player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
    this->_dy = -player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
    this->_unit.moveRightParallax();
    this->_slide.moveRightParallax();
}

void Level::moveLeft() {
    this->_dx = -player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180);
    this->_dy = player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180);
    this->_unit.moveLeftParallax();
    this->_slide.moveLeftParallax();
}

//  dxForward/1.41421 + dxRight/1.41421
//  dyForward/1.41421 + dyRight/1.41421
void Level::moveUpRight() {                 //should have said forwardRight and backwardRight instead
    this->_dx = -player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180)/1.41421;
    this->_dy = -player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180)/1.41421 - player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180)/1.41421;
    this->_unit.moveUpRightParallax();
    this->_slide.moveUpRightParallax();
}
void Level::moveUpLeft() {
    this->_dx = -player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180)/1.41421 + -player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180)/1.41421;
    this->_dy = -player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180)/1.41421;
    this->_unit.moveUpLeftParallax();
    this->_slide.moveUpLeftParallax();
}
void Level::moveDownRight() {
    this->_dx = player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180)/1.41421;
    this->_dy = player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180)/1.41421 + -player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180)/1.41421;
    this->_unit.moveDownRightParallax();
    this->_slide.moveDownRightParallax();
}
void Level::moveDownLeft() {
    this->_dx = player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180)/1.41421 + -player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180)/1.41421;
    this->_dy = player_constants::WALK_SPEED * std::cos(this->_angle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::sin(this->_angle * 3.14159 / 180)/1.41421;
    this->_unit.moveDownLeftParallax();
    this->_slide.moveDownLeftParallax();
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
    this->_slide.stopMovingParallax();
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
    
    //Rectangle playerRec = Rectangle(625, 385, 30, 30);
    Rectangle playerRec = Rectangle(640 - 8, 400 - 8, 16, 16);
    
    //y: 392
    
    for (int i = 0; i < others.size(); i++) {
        
        //printf("y: %d, height: %d\n", others.at(i).getY(), others.at(i).getHeight());
        
        sides::Side collisionSide = playerRec.getCollisionSide(others.at(i));
        
        //printf("map: %d, player: %d",others.at(i).getHeight(),playerRec.getHeight());
        
        /*
        if(collisionSide == sides::Side::TOP) {
            printf("top\n");
        }
        else if(collisionSide == sides::Side::BOTTOM) {
            printf("bottom\n");
        }
        else if(collisionSide == sides::Side::LEFT) {
            printf("left\n");
        }
        else if(collisionSide == sides::Side::RIGHT) {
            printf("right\n");
        }
        */
        
        
        
        if (collisionSide != sides::NONE) {
            switch (collisionSide) {
                case sides::TOP:                    //top refers to player's box
                    this->changeY(-(others.at(i).getStartY() + others.at(i).getHeight()) + 392, 392 - others.at(i).getHeight());
                    //std::cout << others.at(i).getStartY() << " " << others.at(i).getHeight() <<std::endl;
                    this->_dy = 0.0f;
                    
                    //unit.setY(unit.getStartY() - 385 + 66); //maybe 65
                    //unit.setDY(0.0f);
                    //unit.moveForward();

                    
                    break;
                case sides::BOTTOM:
                    this->changeY(-(others.at(i).getStartY()) + 392 + 16, 392 + 16);
                    this->_dy = 0.0f;
                    

                    
                    //unit.setY(unit.getStartY() - 385 + 128); //also not exactly right
                    //unit.setDY(0.0f);
                    
                    break;
                case sides::LEFT:
                    this->changeX((-others.at(i).getStartX()) + 632 - others.at(i).getWidth(), 632 - others.at(i).getWidth());
                    this->_dx = 0.0f;
                    

                    
                    //unit.setX(unit.getStartX() - 48);
                    //unit.setDX(0.0f);
                    
                    break;
                case sides::RIGHT:
                    this->changeX((-others.at(i).getStartX()) + 632 + 16, 632 + 16);
                    this->_dx = 0.0f;
                    
                    
                    //unit.setX(unit.getStartX() + 48);
                    //unit.setDX(0.0f);
                    
                    break;
            }
        }
        //printf("y: %d, height: %d\n", others.at(i).getY(), others.at(i).getHeight());

        
    }
    
    
    this->_unit.handleTileCollisions(others, elapsedTime);
    this->_slide.handleTileCollisions(others, elapsedTime);
    
    
    
}





void Level::changeY(int newY, int newCollisionY) {
    this->_positiony = newY;
    
    //392 - others.at(i).getHeight()
    
    
     for(int i = 0; i < this->_collisionRects.size(); i++) {
         this->_collisionRects.at(i).changeY(this->_collisionRects.at(i).getStartY() + newY);
     }
     
    
    
    //this->_collisionRects.at(0).changeY(newCollisionY);
    
    //printf("x: %d, y: %d\n", this->_collisionRects.at(1).getX(), this->_collisionRects.at(1).getY());
    
    
    
    //this->_collisionRects.at(1).changeY(this->_collisionRects.at(1).getStartY() + 56);
    //this->_collisionRects.at(2).changeY(this->_collisionRects.at(2).getStartY() + 56);
    
    //printf("y: %f", this->_positiony); //this is 56
    
    

    //printf("newCollisionY: %d, newY: %d , startY: %d\n", newCollisionY, newY, this->_collisionRects.at(0).getStartY());
    

    

}

void Level::changeX(int newX, int newCollisionX) {
    this->_positionx = newX;
    
    //this->_collisionRects.at(0).changeX(newCollisionX);
    
    
    for(int i = 0; i < this->_collisionRects.size(); i++) {
        this->_collisionRects.at(i).changeX(this->_collisionRects.at(i).getStartX() + newX);
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
    this->_slide.setPlayerAngle(this->_angle);
}

void Level::handleUnitMovement() {
    this->_unit.setPlayerAngle(this->_angle);
    this->_unit.handleMovement();
}

void Level::moveUnitToPosition(int posX, int posY, Graphics &graphics) { //graphics for testing
    //check if the path is clear
    
    bool temp = checkPathCollision(this->_unit.getStaticX(), this->_unit.getStaticY()+4, posX, posY);
    
    //printf("%d, %d\n", temp.x, temp.y);
    
    std::vector<int> vertexPath;
    
    //graphics.drawLine(0,0,500,500);
    
    //no collision
    if(temp == 0) {
        this->_unit.addToMovementOrders(Vector2(posX, posY));
        this->_unit.moveToPosition(posX, posY);
        printf("no collision\n");
        return;                                 //CHANGE THIS
    }
    else {
        
        std::map<int, Vector2> vertices = this->_graph.getVertexTable();
        
        this->_graph.removeEdgesForSourceAndDestinationVertices();
        
        bool tempDestination;
        double weight = 0.0;
        
        for(const auto& iter : vertices) {
            temp = checkPathCollision(this->_unit.getStaticX(), this->_unit.getStaticY()+4, iter.second.x, iter.second.y);
            
            tempDestination = checkPathCollision(posX, posY, iter.second.x, iter.second.y);
            
            //printf("destination: %d, %d, %d, %d\n", posX, posY, iter.second.x, iter.second.y);
            
            if(temp == 0) {
                weight = std::sqrt(std::pow(this->_unit.getStaticX()-iter.second.x,2)+std::pow(this->_unit.getStaticY()+4-iter.second.y,2));
                
                printf("%d, %d\n", iter.first, this->_graph.getVertexCount());
                
                this->_graph.addEdge(iter.first, this->_graph.getVertexCount(), weight); //source

            }
            
            if(tempDestination == 0) {
                //printf("no collision\n");
                weight = std::sqrt(std::pow(posX-iter.second.x,2)+std::pow(posY-iter.second.y,2));
                
                this->_graph.addEdge(iter.first, this->_graph.getVertexCount()+1, weight); //destination
                
            }
            
        }
        
        
        
        this->_graph.printAdjacencyMatrix();
        
        //find h values for all vertices
        std::map<int, double> h;
        
        for(const auto& iter : vertices) {
            double manhattan = std::sqrt(std::pow(posX-iter.second.x,2) + std::pow(posY-iter.second.y,2));
            h.insert(std::make_pair(iter.first, manhattan));
        }

        
        
        vertexPath = this->_graph.getAStarVertexPath(h);
        
        
        for(int i = 0; i < vertexPath.size(); i++) {
            printf("%d, ", vertexPath[i]);
        }
        printf("\n");
        
        /*
        //pick starting vertex (vertexCount)
        for(int i = 0; i < this->_graph.getVertexCount(); i++) { //no need to check connections with destination
            double min = this->_
            
        }
        */
        
        
        
        
    }

    
    std::vector<Vector2> movementOrders = this->_graph.convertToMovementOrders(vertexPath);
    
    movementOrders.push_back(Vector2(posX, posY)); //put desination in there
    
    this->_unit.addToMovementOrders(movementOrders);
    
    printf("movement orders:\n");
    this->_unit.printMovementOrders();
    
    
    
    //this->_unit.moveToPosition(posX, posY);
    this->_unit.moveToPosition(movementOrders[0].x, movementOrders[0].y);
}

bool Level::checkSlideCollision(int xm, int ym) {
    
    return this->_slide.checkSlideCollision(xm, ym);
    
}

void Level::handleSlideMovement(int xm, int ym) {
    this->_slide.handleSlideMovement(xm, ym, this->_angle, this->_positionx, this->_positiony);
}

void Level::centerSlideToZero(){
    this->_slide.centerSlideToZero();
}


bool Level::checkPathCollision(int beginx, int beginy, int endx, int endy) {
    

    //double xdiff = endx - (this->_unit.getStaticX());
    //double ydiff = endy - (this->_unit.getStaticY()+4);
    double xdiff = endx - (beginx);
    double ydiff = endy - (beginy);
    
    double angle = 0.0;
    
    if(ydiff < 0) {
        angle = (-std::atan(xdiff/ydiff)*180/3.14159);
    }
    else {
        angle = (-std::atan(xdiff/ydiff) - 3.14159)*180/3.14159;
    }
    
    angle = -angle;
    
    if(angle > 180) {
        angle -= 360;
    }
    else if(angle < -180) {
        angle += 360;
    }

    
    //printf("%f, %f\n", angle, this->_angle);
    
    //Vector2 temp = checkPathCollision(beginx, beginy, angle);
    
    //printf("%d, %d\n", temp.x, temp.y);
    
    double distanceToEnd = std::sqrt(std::pow(beginx - endx,2) + std::pow(beginy - endy,2));
    
    //double distanceToEnd = 0;
    
    return checkPathCollisionHelp(beginx, beginy, angle, distanceToEnd);
}


bool Level::checkPathCollisionHelp(double beginx, double beginy, double angle, double distanceToEnd) {
    
    //printf("begin: %f, %f, %f\n",beginx, beginy,angle);
    
    
    float m = (beginy - (beginy - 100*std::cos(angle*3.14159/180)))/(beginx-(beginx - 100*std::sin(angle*3.14159/180)));
    
    
    float b = beginy - m*beginx;
    //printf("m: %f, b: %f\n", m, b);
    
    //printf("size: %d\n", this->_collisionRects.size());
    
    float collisionx = 0.0;
    float collisiony = 0.0;
    float yhit = 0.0;
    float xhit = 0.0;
           
    for(Rectangle &i : this->_collisionRects) {
        
    
        //check bottom
        collisionx = (i.getStartY()+i.getHeight() - b)/m;
        collisiony = i.getStartX() * m + b;
        if((i.getStartY() <= (beginy - i.getHeight()) && std::abs(angle) <= 90)) {
            //printf("I'm satisfied\n");
            //printf("\n");
            
            //printf("collisionx: %f\n", collisionx);
            
            
            //printf("\n");
            
            if((collisionx >= i.getStartX()) && (collisionx <= (i.getStartX()+i.getWidth()))) {       //need ">=" to account for corners
                
                //yes collision
                //printf("bottom\n");
                
                yhit = i.getY()+i.getHeight();
                
                if (std::sqrt(std::pow(beginx - collisionx,2) + std::pow(beginy - yhit,2)) < distanceToEnd) {
                    return 1;
                }
                
                
                
                //return 1;
                
                //return Vector2(collisionx, this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight());
                
            }
        }
        
        
        
        //check top
        collisionx = (i.getStartY() - b)/m;
        collisiony = i.getStartX() * m + b;
        
        if((i.getStartY() >= beginy && std::abs(angle) >= 90)) {
            if(collisionx >= i.getStartX() && collisionx <= i.getStartX()+i.getWidth()) {
                
                //printf("top\n");
                //return 1;
                
                //return Vector2(collisionx,this->_collisionRects.at(0).getY());
                
                yhit = i.getY();
                
                if (std::sqrt(std::pow(beginx - collisionx,2) + std::pow(beginy - yhit,2)) < distanceToEnd) {
                    return 1;
                }
                
            }
        }
        
        
        
        
        //check left
        collisionx = (i.getStartY() - b)/m;
        collisiony = i.getStartX() * m + b;
        if((i.getStartX() >= beginx) && angle <= 0) {
            if(collisiony >= i.getStartY() && collisiony <= i.getStartY()+i.getHeight()) {
                
                //printf("left\n");
                
                //return 1;
                
                xhit = i.getX();
                
                if (std::sqrt(std::pow(beginx - xhit,2) + std::pow(beginy - collisiony,2)) < distanceToEnd) {
                    return 1;
                }
                
                //return Vector2(this->_collisionRects.at(0).getX(),collisiony);
                
            }
        }
        
        //check right
        collisionx = (i.getStartY() - b)/m;
        collisiony = (i.getStartX()+i.getWidth()) * m + b;
        if((i.getStartX() <= beginx - i.getWidth()) && angle >= 0) {
            if(collisiony >= i.getStartY() && collisiony <= i.getStartY()+i.getHeight()) {
                
                //printf("right\n");
                
                //return 1;
                
                xhit = i.getX()+i.getWidth();
                
                if (std::sqrt(std::pow(beginx - xhit,2) + std::pow(beginy - collisiony,2)) < distanceToEnd) {
                    return 1;
                }
                
                //return Vector2(this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth(), collisiony);
                
            }
        }
    
    
    
    }
    

    
    //no collision
    return 0;
    //return Vector2(0, 0);
    
    
    
}



Vector2 Level::checkShotCollision(double beginx, double beginy, double angle) {
    
    
    float m = (beginy - (beginy - 100*std::cos(angle*3.14159/180)))/(beginx-(beginx - 100*std::sin(angle*3.14159/180)));

    
    float b = beginy - m*beginx;
    
    float collisionx = 0.0;
    float collisiony = 0.0;
    
    std::vector<Vector2> collisionPoints;
    
    for(Rectangle i : this->_collisionRects) {
        //check bottom
        collisionx = (i.getY()+i.getHeight() - b)/m;
        collisiony = i.getX() * m + b;
        if((i.getY() < (beginy - i.getHeight()) && std::abs(angle) < 90)) {
            if(collisionx > i.getX() && collisionx < i.getX()+i.getWidth()) {
                
                //return Vector2(collisionx, i.getY()+i.getHeight());
                collisionPoints.push_back(Vector2(collisionx, i.getY()+i.getHeight()));
                
            }
        }
        
        
        
        //check top
        collisionx = (i.getY() - b)/m;
        collisiony = i.getX() * m + b;
        if((i.getY() > beginy && std::abs(angle) > 90)) {
            if(collisionx > i.getX() && collisionx < i.getX()+i.getWidth()) {
                
                //printf("top ");
                
                //return Vector2(collisionx, i.getY());
                collisionPoints.push_back(Vector2(collisionx, i.getY()));
                
            }
        }
        
        
        
        
        //check left
        collisionx = (i.getY() - b)/m;
        collisiony = i.getX() * m + b;
        if((i.getX() > beginx) && angle < 0) {
            if(collisiony > i.getY() && collisiony < i.getY()+i.getHeight()) {
                
                //return Vector2(i.getX(),collisiony);
                collisionPoints.push_back(Vector2(i.getX(),collisiony));
                
                
            }
        }
        
        //check right
        collisionx = (i.getY() - b)/m;
        collisiony = (i.getX()+i.getWidth()) * m + b;
        if((i.getX() < beginx - i.getWidth()) && angle > 0) {
            if(collisiony > i.getY() && collisiony < i.getY()+i.getHeight()) {
                
                //return Vector2(i.getX()+i.getWidth(), collisiony);
                collisionPoints.push_back(Vector2(i.getX()+i.getWidth(), collisiony));
                
            }
        }
    }
    
    if(collisionPoints.size() > 0) {
        
        double min = 1000000;
        double weight = 0.0;
        int nearest = -1;
        
        for(int i = 0; i<collisionPoints.size(); i++) {
            weight = std::sqrt(std::pow(collisionPoints[i].x - 640,2) + std::pow(collisionPoints[i].y - 400,2));
            if(weight < min) {
                min = weight;
                nearest = i;
            }
        }
        
        return collisionPoints[nearest];
        
    }
        
        
    /*
    //check bottom
    float collisionx = (this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight() - b)/m;
    float collisiony = this->_collisionRects.at(0).getX() * m + b;
    if((this->_collisionRects.at(0).getY() < (beginy - this->_collisionRects.at(0).getHeight()) && std::abs(angle) < 90)) {
        if(collisionx > this->_collisionRects.at(0).getX() && collisionx < this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth()) {
            
            return Vector2(collisionx, this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight());

        }
    }
    

    
    //check top
    collisionx = (this->_collisionRects.at(0).getY() - b)/m;
    collisiony = this->_collisionRects.at(0).getX() * m + b;
    if((this->_collisionRects.at(0).getY() > beginy && std::abs(angle) > 90)) {
        if(collisionx > this->_collisionRects.at(0).getX() && collisionx < this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth()) {
            
            //printf("top ");
            
            return Vector2(collisionx,this->_collisionRects.at(0).getY());

        }
    }


    
    
    //check left
    collisionx = (this->_collisionRects.at(0).getY() - b)/m;
    collisiony = this->_collisionRects.at(0).getX() * m + b;
    if((this->_collisionRects.at(0).getX() > beginx) && angle < 0) {
        if(collisiony > this->_collisionRects.at(0).getY() && collisiony < this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight()) {
            
            return Vector2(this->_collisionRects.at(0).getX(),collisiony);
            
        }
    }
    
    //check right
    collisionx = (this->_collisionRects.at(0).getY() - b)/m;
    collisiony = (this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth()) * m + b;
    if((this->_collisionRects.at(0).getX() < beginx - this->_collisionRects.at(0).getWidth()) && angle > 0) {
        if(collisiony > this->_collisionRects.at(0).getY() && collisiony < this->_collisionRects.at(0).getY()+this->_collisionRects.at(0).getHeight()) {
            
            return Vector2(this->_collisionRects.at(0).getX()+this->_collisionRects.at(0).getWidth(), collisiony);
            
        }
    }
    */
    
    //now check the collisions with other units
    Rectangle unitRec = this->_unit.getCollisionRect();
    
    //check bottom
    collisionx = (unitRec.getY()+unitRec.getHeight() - b)/m;
    collisiony = unitRec.getX() * m + b;
    if((unitRec.getY() < (beginy - unitRec.getHeight()) && std::abs(angle) < 90)) {
        if(collisionx > unitRec.getX() && collisionx < unitRec.getX()+unitRec.getWidth()) {
            
            return Vector2(collisionx, unitRec.getY()+unitRec.getHeight());
            
        }
    }
    
    //check top
    collisionx = (unitRec.getY() - b)/m;
    collisiony = unitRec.getX() * m + b;
    if((unitRec.getY() > beginy && std::abs(angle) > 90)) {
        if(collisionx > unitRec.getX() && collisionx < unitRec.getX()+unitRec.getWidth()) {
            
            return Vector2(collisionx, unitRec.getY());
            
        }
    }

    //check left
    collisionx = (unitRec.getY() - b)/m;
    collisiony = unitRec.getX() * m + b;
    if((unitRec.getX() > beginx) && angle < 0) {
        if(collisiony > unitRec.getY() && collisiony < unitRec.getY()+unitRec.getHeight()) {
            
            return Vector2(unitRec.getX(),collisiony);
            
        }
    }
    
    //check right
    collisionx = (unitRec.getY() - b)/m;
    collisiony = (unitRec.getX()+unitRec.getWidth()) * m + b;
    if((unitRec.getX() < beginx - unitRec.getWidth()) && angle > 0) {
        if(collisiony > unitRec.getY() && collisiony < unitRec.getY()+unitRec.getHeight()) {
            
            return Vector2(unitRec.getX()+unitRec.getWidth(), collisiony);
            
        }
    }
    
    
    
    //no collision
    return Vector2(0, 0);
    

    
}

void Level::moveUnitToSlidePosition(Graphics &graphics) {
    moveUnitToPosition(std::round(this->_slide.getStaticX() + 75 - 8), std::round(this->_slide.getStaticY() + 12 - 8), graphics);
    
    //printf("%f, %f\n", std::round(this->_slide.getStaticX() + 75 - 8), std::round(this->_slide.getStaticY() + 12 - 8));
    
    
}
