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
    Map(graphics, "/Users/jonahglick/Documents/Com/com_test5.png", 0, 0, 1280, 800, 0, 0),
    //Map(graphics, "/Users/jonahglick/Documents/Com/village-deshast-large.png", 0, 0, 1714, 2400, 0, 0),
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
    _ang(0),
    _drawFovNode(0)
{
    //printf("level contructor\n");
    
	this->loadMap(mapName, graphics);
    
    this->_unit = Unit(graphics, Vector2(0,0), 1);
    
    //std::vector< std::unique_ptr<int> > test;
    
    
    this->_fireteam = Fireteam();
    
    //this->_fireteam.addUnit(new Unit(graphics, Vector2(100,100), 1));
    
    
    //Unit * unitPointer = new Unit(graphics, Vector2(100,100), 1);
    //std::unique_ptr<Unit> tmp(new Unit(graphics, Vector2(100,100), 1) );
    //this->_fireteam.addUnit( std::move(tmp) );                          //need to std::move the unique pointer
    std::shared_ptr<Unit> tmp(new Unit(graphics, Vector2(100,100), 1) );
    this->_fireteam.addUnit( std::move(tmp) );
    
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
	
    std::map<int, Vector2> tempForCoverTable; //in case corners are called before cover
    bool coverHasBeenCalled = 0;
    bool cornerHasBeenCalled = 0;
    
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
                
                cornerHasBeenCalled = 1;
                
                int vertexCount = 0;
                
                //count the total number of vertices
                XMLElement * pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        vertexCount++;
                        pObject = pObject->NextSiblingElement("object");
                    }
                }
                
                this->_graph = Graph(vertexCount+2); //+2 for source and destination nodes
                
                if(coverHasBeenCalled) {
                    this->_graph.initializeCoverTable(tempForCoverTable);
                }
                
                printf("vertex count: %d\n", vertexCount);
                
                //add vertices to the vertex table
                pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        
                        int id = pObject->IntAttribute("name");     //read in direction here
                        int x = pObject->IntAttribute("x");
                        int y = pObject->IntAttribute("y");
                        this->_graph.addToVertexTable(id, x, y);
                        
                        
                        this->_cornerNodes.push_back(CornerNode(x + 8, y + 8, NONE)); //center of the corners
                        
                        //int idNext = pObject->IntAttribute("edge")
                        

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
                
                //add the edges between the vertices
                pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        
                        
                        int id = pObject->IntAttribute("name");         //id
                        XMLElement * pProperty1 = pObject->FirstChildElement("properties");
                        if(pProperty1 != NULL) {
                        
                            XMLElement * pProperty2 = pProperty1->FirstChildElement("property");
                            if(pProperty2 != NULL) {
                                while(pProperty2) {
                                    
                                    std::stringstream ssTemp;
                                    ssTemp << pProperty2->Attribute("name");
                                    
                                    if(ssTemp.str() == "connectedVertices") {
                            
                                        
                                        std::stringstream ssCorner;
                                        
                                        std::vector<int> values;
                                        int n;
                                        ssCorner << pProperty2->Attribute("value");     //string of the form "1 6 12 15"
                                            //assumed to be in "connectedVertices" name
                                        //seperate into ints
                                        
                                        printf("read in from %d: ", id);
                                        while(ssCorner >> n) {
                                            values.push_back(n);
                                            printf("%d ", n);
                                        }
                                        printf("\n");
                                        
                                        
                                        
                                        double weight = 0.0;
                                        for(int otherId : values) {
                                            weight = this->_graph.getWeight(id, otherId);
                                            this->_graph.addEdge(id, otherId, weight);
                                        }
                                        
                                        
                                        
                                        /*
                                        int otherId = pProperty2->IntAttribute("name"); //the vertex to connect to
                                        
                                        double weight = this->_graph.getWeight(id, otherId);
                                        
                                        this->_graph.addEdge(id, otherId, weight);
                                        printf("node: %d to %d weight: %f\n", id, otherId, weight);
                                        */
                                
                                        
                                    }
                                    
                                    else if (ssTemp.str() == "connectedCoverNodes") {
                                        
                                        std::stringstream ssCover;
                                        
                                        std::vector<int> values;
                                        int n;
                                        ssCover << pProperty2->Attribute("value");
                                        
                                        printf("read cover from %d: ", id);
                                        
                                        while(ssCover >> n) {
                                            values.push_back(n);
                                            printf("%d ", n);
                                        }
                                        printf("\n");
                                        
                                        this->_graph.connectCoverNodeToVertex(id, values);
                                        
                                        
                                    }
                                    
                                    
                                    pProperty2 = pProperty2->NextSiblingElement("property");
                                }
                            }
                        
                        }
                        
                        pObject = pObject->NextSiblingElement("object");
                    }
                }
                
                
            
            }
            
            else if (ss.str() == "cover") {                                         //needs to come after graph has been created. graph is created in "corners" so "corner" must come first
                
                coverHasBeenCalled = 1;
                
                //add all cover nodes to cover node table
                XMLElement * pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        
                        //printf("add to cover table\n");
                        
                        int id = pObject->IntAttribute("name");
                        int x = pObject->IntAttribute("x");
                        int y = pObject->IntAttribute("y");
                        
                        if(cornerHasBeenCalled) { //then graph has already been initlized
                            this->_graph.addToCoverTable(id, x, y);
                        }
                        else {
                            tempForCoverTable.insert(std::make_pair(id, Vector2(x, y)));
                        }
                        
                        
                        pObject = pObject->NextSiblingElement("object");
                    }
                }
                
                
            }
            else if (ss.str() == "building") {                                         //needs to come after graph has been created. graph is created in "corners" so "corner" must come first
                
                //add all building rects to cover building table
                XMLElement * pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {

                        
                        int id = pObject->IntAttribute("name");
                        int x = pObject->IntAttribute("x");
                        int y = pObject->IntAttribute("y");
                        int width = pObject->IntAttribute("width");
                        int height = pObject->IntAttribute("height");
                        
                        
                        Rectangle building(x, y, width, height);
                        
                        this->_buildings.push_back(building);
                        
                        
                        
                        if(cornerHasBeenCalled) { //then graph has already been initlized
                            this->_graph.addToCoverTable(id, x, y);
                        }
                        else {
                            tempForCoverTable.insert(std::make_pair(id, Vector2(x, y)));
                        }
                        
                        
                        pObject = pObject->NextSiblingElement("object");
                    }
                }
                
                
            }
            
            else if (ss.str() == "FoWCorners") {
                
                //add all FoWCorners to FoWCorners vector
                XMLElement * pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        int position = pObject->IntAttribute("name");       //maybe don't need id
                        int x = pObject->IntAttribute("x");
                        int y = pObject->IntAttribute("y");
                        
                        //0 - TL
                        //1 - TR
                        //2 - BR
                        //3 - BL
                        //4 - IN
                        
                        FoWNodePosition nodePosition = intToNodePosition(position); //maybe change this to string to node position
                        
                        
                        bool isInnerCorner = 0;
                        
                        
                        XMLElement * pProperty1 = pObject->FirstChildElement("properties");
                        if(pProperty1 != NULL) {
                            
                            XMLElement * pProperty2 = pProperty1->FirstChildElement("property");
                            if(pProperty2 != NULL) {
                                
                                while(pProperty2) {
                                    
                                    std::stringstream ssTemp;
                                    ssTemp << pProperty2->Attribute("name");
                                    
                                    if(ssTemp.str() == "innerCorner") {
                                        
                                        isInnerCorner = pProperty2->IntAttribute("value");
                                         

                                    }
                                    pProperty2 = pProperty2->NextSiblingElement("property");
                                }
                                
                            }
                            
                        }
                        
                        
                        
                        
  
                        this->_FoWNode.push_back(FoWNode(x, y, nodePosition, isInnerCorner));
                        
                        
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

void Level::update(int elapsedTime, Graphics &graphics) {


    for(int i  = 0; i < this->_collisionRects.size(); i++) {
        //this->_collisionRects.at(i).update(elapsedTime, this->_dx, this->_dy);
        this->_collisionRects.at(i).update(elapsedTime, graphics.getCameraDx(), graphics.getCameraDy());
    }
    

    
    this->_unit.update(elapsedTime, this->_angle, graphics);
    this->_slide.update(elapsedTime, graphics);
    
    
    
    
    //update gunshots
    
    std::vector<GunshotPath>::iterator iter = this->_gunShotPaths.begin();//                      THIS IS THE PROBLEM
    while(iter != this->_gunShotPaths.end()) {
        //if(iter->update(elapsedTime) == 1) {
            //iter = this->_gunShotPaths.erase(iter);
        //}
        bool dead = iter->update(elapsedTime);
        if(dead == 1) {
            this->_gunShotPaths.erase(iter);
        }
        else {
            ++iter;
        }
    }
    
    
    /*
    for(GunshotPath &iter : this->_gunShotPaths) {
        iter.update(elapsedTime);
    }
    */
}


void Level::draw(Graphics &graphics) {

    //int posx = std::round(this->_positionx);
    //int posy = std::round(this->_positiony);
    
    
    int posx = std::round(graphics.getCameraX());
    int posy = std::round(graphics.getCameraY());

    
    
    //Map::drawTrans(graphics, posx, posy, this->_angle, posx, posy, this->_cameraMove);
    
    Map::drawTrans(graphics, posx, posy, graphics.getCameraAngle(), posx, posy, this->_cameraMove);
    
    //draw collision rects
    for(int i = 0; i<this->_collisionRects.size(); i++) {
        graphics.drawRect(std::round(this->_collisionRects.at(i).getX()), std::round(this->_collisionRects.at(i).getY()), this->_collisionRects.at(i).getWidth(), this->_collisionRects.at(i).getHeight());
    }
    
    //draw unit collision rects
    Rectangle unitRec = this->_unit.getCollisionRect();

    graphics.drawRect(std::round(unitRec.getX()), std::round(unitRec.getY()), unitRec.getWidth(), unitRec.getHeight());
    
    
    
    
    
    //graphics.drawLine(640, 400, 640 - 100*std::sin(this->_angle*3.14159/180), 400 - 100*std::cos(this->_angle*3.14159/180) );
    
    graphics.drawLine(640, 400, 640 - 100*std::sin(graphics.getCameraAngle()*3.14159/180), 400 - 100*std::cos(graphics.getCameraAngle()*3.14159/180) );
    
    //graphics.drawLine(10,10,800,800);
    
    
    Vector2 coll = checkShotCollision(640, 400, graphics.getCameraAngle());
    
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
    
    this->_fireteam.draw(graphics);
    
    //float xEquivalent = this->_unit.getStaticX() + this->_unit.getX()+8;
    //float yEquivalent = this->_unit.getStaticY() + this->_unit.getY()+12;
    
    float xEquivalent = this->_unit.getStaticX() + graphics.getCameraX() + 8;
    float yEquivalent = this->_unit.getStaticY() + graphics.getCameraY() + 12;
    
    float angleEquivalent = -this->_unit.getAngle();    //the opposite angle of level
    
    //in both cases we change the - sin for x into a + sin
    graphics.drawLine(xEquivalent, yEquivalent, xEquivalent - 100*std::sin(angleEquivalent*3.14159/180), yEquivalent - 100*std::cos(angleEquivalent*3.14159/180) );

    
    coll = checkShotCollision(xEquivalent, yEquivalent, angleEquivalent);
    
    
    if(!((coll.x == 0) && (coll.y == 0))) {                         //CHNAGE THIS TO NOT BE 0!
        graphics.drawRect(coll.x - 5, coll.y - 5, 10, 10);
    }

    
    //slide
    this->_slide.draw(graphics);
    
    //draw all the gunshots
    for(GunshotPath iter : this->_gunShotPaths) {
        iter.draw(graphics);
    }
    
    
    //draw polygon corners
    
    if(this->_drawFovNode) {
        drawFogOfWar(graphics);
    }
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

void Level::handleSlideRelease(Graphics &graphics) {
    centerSlideToZero();
    
    //check to see if the slide is in a building
    //if(this->_level.) { //if slide is in a building
    bool isInBuilding = 0;
    
    for(Rectangle i : this->_buildings) {
        Vector2 center = this->_slide.getCenter();
        if((center.x > i.getStartX()) && (center.x < i.getStartX() + i.getWidth())) {
            if((center.y > i.getStartY()) && (center.y < i.getStartY() + i.getHeight())) {
                
                printf("in building\n");
                //isInBuilding = 1;
            }
        }
    }
    
    
    if(0) { //if is in building. Find preferredNode;
        
    }
    else {
        moveUnitToSlidePosition(graphics);
        moveUnitAngleToSlideAngle(graphics);
    }
    

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



/*
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
*/


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


void Level::handleTileCollisions(std::vector<Rectangle> &others, float elapsedTime, Graphics &graphics) { //other are the level's collision rects
    

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

                    //this->_dy = 0.0f;
                    

                    graphics.setCameraY(-(others.at(i).getStartY() + others.at(i).getHeight()) + 392);

                    
                    break;
                case sides::BOTTOM:
                    this->changeY(-(others.at(i).getStartY()) + 392 + 16, 392 + 16);
                    //this->_dy = 0.0f;
                    
                    graphics.setCameraY(-(others.at(i).getStartY()) + 392 + 16);
                    
                    
                    break;
                case sides::LEFT:
                    this->changeX((-others.at(i).getStartX()) + 632 - others.at(i).getWidth(), 632 - others.at(i).getWidth());
                    //this->_dx = 0.0f;
                    
                    graphics.setCameraX((-others.at(i).getStartX()) + 632 - others.at(i).getWidth());
                    

                    
                    break;
                case sides::RIGHT:
                    this->changeX((-others.at(i).getStartX()) + 632 + 16, 632 + 16);
                    //this->_dx = 0.0f;
                    
                    graphics.setCameraX((-others.at(i).getStartX()) + 632 + 16);
                    

                    
                    break;
            }
        }


        
    }
    
    
    this->_unit.handleTileCollisions(others, elapsedTime);
    this->_slide.handleTileCollisions(others, elapsedTime);
    
    
    
}





void Level::changeY(int newY, int newCollisionY) {
    
     for(int i = 0; i < this->_collisionRects.size(); i++) {
         this->_collisionRects.at(i).changeY(this->_collisionRects.at(i).getStartY() + newY);
     }

}

void Level::changeX(int newX, int newCollisionX) {
    
    
    for(int i = 0; i < this->_collisionRects.size(); i++) {
        this->_collisionRects.at(i).changeX(this->_collisionRects.at(i).getStartX() + newX);
    }
    
}

/*
void Level::changeAngle(float angle) {
    this->_angle += angle;
    if(this->_angle > 180) {
        this->_angle = this->_angle - 360;
    }
    else if(this->_angle < -180) {
        this->_angle = this->_angle + 360;
    }
}
*/
/*
float Level::getAngle() {
    return this->_angle;
}

float Level::getDX() {
    return this->_dx;
}

float Level::getDY() {
    return this->_dy;
}
*/

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
    
    bool temp = checkPathCollisionNew(this->_unit.getStaticX()+8, this->_unit.getStaticY()+4+8, posX+8, posY+8, graphics);
    //printf("temp: %d\n", temp);
    
    //printf("%d, %d\n", posX, posY);
    
    
    //printf("%d, %d\n", temp.x, temp.y);
    
    std::vector<int> vertexPath;
    
    //graphics.drawLine(0,0,500,500);
    
    //printf("%f, %f\n", this->_unit.getStaticX(),this->_unit.getStaticY());
    
    //no collision
    if(temp == 0) {
        this->_unit.addToMovementOrders(Vector2(posX, posY));
        this->_unit.moveToPosition(posX, posY);
        printf("no collision\n");
        return;                                 //CHANGE THIS
    }
    else {
        
        //this maybe should all be in the graph class? Maybe not
        
        
        std::map<int, Vector2> vertices = this->_graph.getVertexTable();
        
        this->_graph.removeEdgesForSourceAndDestinationVertices();
        
        bool tempDestination;
        double weight = 0.0;
        
        for(const auto& iter : vertices) {
            temp = checkPathCollisionNew(this->_unit.getStaticX()+8, this->_unit.getStaticY()+4+8, iter.second.x+8, iter.second.y+8,graphics);
            printf("vertex: %d \t collision:%d\n", iter.first, temp);
            
            
            tempDestination = checkPathCollisionNew(posX+8, posY+8, iter.second.x+8, iter.second.y+8,graphics);
            
            //printf("destination: %d, %d, %d, %d\n", posX, posY, iter.second.x, iter.second.y);
            
            if(temp == 0) {
                weight = std::sqrt(std::pow(this->_unit.getStaticX()-iter.second.x,2)+std::pow(this->_unit.getStaticY()+4-iter.second.y,2));
                
                //printf("%d, %d\n", iter.first, this->_graph.getVertexCount());
                
                this->_graph.addEdge(iter.first, this->_graph.getVertexCount()+1, weight); //source

            }
            
            if(tempDestination == 0) {
                //printf("no collision\n");
                weight = std::sqrt(std::pow(posX-iter.second.x,2)+std::pow(posY-iter.second.y,2));
                
                this->_graph.addEdge(iter.first, this->_graph.getVertexCount(), weight); //destination
                
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

void Level::handleSlideMovement(int xm, int ym, Graphics &graphics) {
    this->_slide.handleSlideMovement(xm, ym, this->_angle, graphics.getCameraX(), graphics.getCameraY(), graphics);
}

void Level::centerSlideToZero(){
    this->_slide.centerSlideToZero();
}

bool Level::checkPathCollisionNew(int beginx, int beginy, int endx, int endy, Graphics &graphics) {
    //we're going from center of vertices;
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
    
    double distanceToEnd = std::sqrt(std::pow(beginx - endx,2) + std::pow(beginy - endy,2));
    

    double topPosX = beginx - 8 * std::cos(angle*3.14159/180);      //radius of unit is 8
    double topPosY = beginy + 8 * std::sin(angle*3.14159/180);
    
    double bottomPosX = beginx + 8 * std::cos(angle*3.14159/180);
    double bottomPosY = beginy - 8 * std::sin(angle*3.14159/180);
    
    
    Vector2 collisionTop = checkShotCollisionNew(topPosX, topPosY, angle);
    if(!(collisionTop.x == 0 && collisionTop.y == 0)) { //if there was a collision
        double distanceToCollision = std::sqrt(std::pow(collisionTop.x - topPosX,2) + std::pow(collisionTop.y - topPosY,2));
        if(distanceToCollision < distanceToEnd) { // then there is no clear path
            return 1;
        }
    }
    
    Vector2 collisionBottom = checkShotCollisionNew(bottomPosX, bottomPosY, angle);
    if(!(collisionBottom.x == 0 && collisionBottom.y == 0)) { //if there was a collision
        double distanceToCollision = std::sqrt(std::pow(collisionBottom.x - bottomPosX,2) + std::pow(collisionBottom.y - bottomPosY,2));
        if(distanceToCollision < distanceToEnd) { // then there is no clear path
            return 1;
        }
    }
    

    //this->_gunShotPaths.push_back(GunshotPath(graphics, topPosX, topPosY, endx, endy, 6000));
    //this->_gunShotPaths.push_back(GunshotPath(graphics, bottomPosX, bottomPosY, endx, endy, 6000));

    
    return 0; //no collision
}



bool Level::checkPathCollision(int beginx, int beginy, int endx, int endy, Graphics &graphics) {

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
    
    //bool collision = checkPathCollisionHelp(beginx, beginy, angle, distanceToEnd);
    
    //(y - y1) = m (x - x1)
    // y = mx + y1 - m * x1
    
    double m = ydiff / xdiff;
    double b = beginy - m * beginx;
    
    //b = b + 8 / std::sin(angle*3.14159/180);
    
    //bool collision = checkPathCollisionHelp2(beginx, beginy, m, b, distanceToEnd, angle);   //checking collision with center
    
    b = b + 8 / std::sin(angle*3.14159/180);
    bool collisionTop = checkPathCollisionHelp2(beginx, beginy, m, b, distanceToEnd, angle);
    //graphics.storeLineDebug(beginx, beginx*m + b, endx, endx*m + b, collisionTop);
    
    
    b = b - 16 / std::sin(angle*3.14159/180);
    bool collisionBottom = checkPathCollisionHelp2(beginx, beginy, m, b, distanceToEnd, angle);
    //graphics.storeLineDebug(beginx, beginx*m + b, endx, endx*m + b, collisionBottom);
    
    
    
    
    
    
    /*
    //graphics.storeLineDebug(beginx, beginy, endx, endy, collision);
    
    //y = m x + b
    //graphics.storeLineDebug(0, b, 1280, 1280*m + b, 0);
    
    b = b + 8 / std::sin(angle*3.14159/180);
    graphics.storeLineDebug(beginx, beginx*m + b, endx, endx*m + b, collision);
    
    b = b - 16 / std::sin(angle*3.14159/180);
    graphics.storeLineDebug(beginx, beginx*m + b, endx, endx*m + b, collision);
    */
    
    
    
    return (collisionTop || collisionBottom);
}

bool Level::checkPathCollisionHelp2(double beginx, double beginy, double m, double b, double distanceToEnd, double angle) {
    
    /*
    float m = (beginy - (beginy - 100*std::cos(angle*3.14159/180)))/(beginx-(beginx - 100*std::sin(angle*3.14159/180)));
    
    
    float b = beginy - m*beginx;
    */
    
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
                
                yhit = i.getStartY()+i.getHeight();
                
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
                
                yhit = i.getStartY();
                
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
                
                xhit = i.getStartX();
                
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
                
                xhit = i.getStartX()+i.getWidth();
                
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

Vector2 Level::checkShotCollisionNew(double beginx, double beginy, double angle) {
    
    
    //float m = (beginy - (beginy - 100*std::cos(angle*3.14159/180)))/(beginx-(beginx - 100*std::sin(angle*3.14159/180)));
    float m = 1.0 / std::tan(angle*3.14159/180);
    
    float b = beginy - m*beginx;
    
    float collisionx = 0.0;
    float collisiony = 0.0;
    
    std::vector<Vector2> collisionPoints;
    
    for(Rectangle i : this->_collisionRects) {
        //check bottom
        collisionx = (i.getStartY()+i.getHeight() - b)/m;
        collisiony = i.getStartX() * m + b;
        if((i.getStartY() < (beginy - i.getHeight()) && std::abs(angle) < 90)) {
            if(collisionx > i.getStartX() && collisionx < i.getStartX()+i.getWidth()) {
                
                //return Vector2(collisionx, i.getY()+i.getHeight());
                collisionPoints.push_back(Vector2(collisionx, i.getStartY()+i.getHeight()));
                
            }
        }
        
        
        
        //check top
        collisionx = (i.getStartY() - b)/m;
        collisiony = i.getStartX() * m + b;
        if((i.getStartY() > beginy && std::abs(angle) > 90)) {
            if(collisionx > i.getStartX() && collisionx < i.getStartX()+i.getWidth()) {
                
                //printf("top ");
                
                //return Vector2(collisionx, i.getY());
                collisionPoints.push_back(Vector2(collisionx, i.getStartY()));
                
            }
        }
        
        
        
        //check left
        collisionx = (i.getStartY() - b)/m;
        collisiony = i.getStartX() * m + b;
        if((i.getStartX() > beginx) && angle < 0) {
            if(collisiony > i.getStartY() && collisiony < i.getStartY()+i.getHeight()) {
                
                //return Vector2(i.getX(),collisiony);
                collisionPoints.push_back(Vector2(i.getStartX(),collisiony));
                
                
            }
        }
        
        //check right
        collisionx = (i.getStartY() - b)/m;
        collisiony = (i.getStartX()+i.getWidth()) * m + b;
        if((i.getStartX() < beginx - i.getWidth()) && angle > 0) {
            if(collisiony > i.getStartY() && collisiony < i.getStartY()+i.getHeight()) {
                
                //return Vector2(i.getX()+i.getWidth(), collisiony);
                collisionPoints.push_back(Vector2(i.getStartX()+i.getWidth(), collisiony));
                
            }
        }
    }
    
    if(collisionPoints.size() > 0) {

        double min = 1000000;
        double weight = 0.0;
        int nearest = -1;
        
        for(int i = 0; i<collisionPoints.size(); i++) {
            //weight = std::sqrt(std::pow(collisionPoints[i].x - 640,2) + std::pow(collisionPoints[i].y - 400,2));
            weight = std::sqrt(std::pow(collisionPoints[i].x - beginx,2) + std::pow(collisionPoints[i].y - beginy,2));
            if(weight < min) {
                min = weight;
                nearest = i;
            }
        }
        
        return collisionPoints[nearest];
        
    }

    
    
    //no collision
    return Vector2(0, 0);                           //maybe change this? maybe it should be negative, then have the corner of the map at (0,0)
    
    
    
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
    return Vector2(0, 0);                           //maybe change this? maybe it should be negative, then have the corner of the map at (0,0)
    

    
}

void Level::moveUnitToSlidePosition(Graphics &graphics) {
    moveUnitToPosition(std::round(this->_slide.getStaticX() + 75 - 8), std::round(this->_slide.getStaticY() + 12 - 8), graphics);
    
    //printf("%f, %f\n", std::round(this->_slide.getStaticX() + 75 - 8), std::round(this->_slide.getStaticY() + 12 - 8));
    
    
}

void Level::moveUnitToNearestCover(Graphics &graphics) {
    
    printf("c key pressed\n");
    
    //find nearest visible vertex
    std::map<int, Vector2> vertices = this->_graph.getVertexTable();
    
    bool temp;
    
    std::vector<int> visibleVertices;
    
    int minVertex = -1;
    double minDistance = 100000.0;
    
    double distance = 0.0;
    
    //find nearest visible vertex
    for(const auto& iter : vertices) {
        temp = checkPathCollision(this->_unit.getStaticX()+8, this->_unit.getStaticY()+4+8, iter.second.x+8, iter.second.y+8,graphics);
        
        if(temp == 0) {
            
            distance = std::sqrt(std::pow(this->_unit.getStaticX()-iter.second.x,2) + std::pow(this->_unit.getStaticY()-iter.second.y,2));
            if(distance < minDistance) {
                minVertex = iter.first;
                minDistance = distance;
            }

        }
    }
    printf("closest Vertex: %d\n", minVertex);
    
    
    //now find the nearest cover connected to that vertex
    
    minDistance = 1000000.0;
    int minCoverVertex = -1;
    
    if(minVertex != -1) {
        
        std::vector<int> coverNodes = this->_graph.getCoverNodes(minVertex);
        std::map<int, Vector2> coverNodeTable = this->_graph.getCoverNodeTable();
        
        //printf("cover node test: %d, %d\n", coverNodeTable[1].x, coverNodeTable[1].y);
        
        //printf("coverNodeTable size: %ld\n", coverNodeTable.size());
        
        
        
        for(int iter : coverNodes) {
            
            //printf("%d, ", iter);
            
            distance = std::sqrt(std::pow(this->_unit.getStaticX() - coverNodeTable[iter].x,2) + std::pow(this->_unit.getStaticY() - coverNodeTable[iter].y,2));
            printf("%f\n", distance);
            
            if(distance < minDistance) {
                minCoverVertex = iter;
                minDistance = distance;
            }
        }
        //printf("\n");
        
        printf("minCoverVertex: %d\n", minCoverVertex);             //MAKE SURE TO FREE WHEN THE UNIT MOVES AGAIN
        if(!this->_graph.isCoverOccupied(minCoverVertex)) { //if no other unit is occupying that vertex
            this->_graph.setCoverToOccupied(minCoverVertex);
            moveUnitToPosition(coverNodeTable[minCoverVertex].x, coverNodeTable[minCoverVertex].y, graphics);
        }
        else {
            //find the next nearest cover Vertex
        }

        
    }
    
    
    
    
}

void Level::playerFireShot(Graphics &graphics) {
    
    double playerX = -graphics.getCameraX() + graphics.getPlayerCenterX();
    double playerY = -graphics.getCameraY() + graphics.getPlayerCenterY();
    
    //Vector2 shotCollision = checkShotCollision(playerX, playerY, -graphics.getCameraAngle());
    
    Vector2 shotCollision = checkShotCollisionNew(playerX, playerY, graphics.getCameraAngle());
    
    //printf("shotX: %d\t shotY: %d\n", shotCollision.x, shotCollision.y);
    
    //graphics.storeMapLineDebug(graphics.getPlayerCenterX(), graphics.getPlayerCenterY(), 0,0,0);
    
    //calculate distance to unit
    //find m
    
    double mShot = 1.0 / std::tan(graphics.getCameraAngle()*3.14159/180);
    //needs to go through player position
    
    //printf("mShot: %f\n", mShot);
    
    //y = mx + b
    //playerY = m * playerX + b
    //b = - m * playerX + playerY
    
    double bShot = mShot * (-playerX) + playerY;
    
    //now calculate y = mx + b for the perpindicular line
    
    double mPerpindicular = - 1.0 / mShot;
    
    //y = mx + b
    //yUnit = m xUnit + b
    //b = - m xUnit + yUnit
    
    double bPerpindicular = -mPerpindicular * (this->_unit.getStaticX()+8) + (this->_unit.getStaticY() + 12);
    
    //print player shot
    if(shotCollision.x == 0 && shotCollision.y == 0) {   //if the shot did not collide with anything
        //graphics.storeMapLineDebug(0, bShot, 1280, mShot * 1280 + bShot, 0);
        
        //make sure shot only goes 400 pixels
        
        this->_gunShotPaths.push_back(GunshotPath(graphics, std::round(playerX), std::round(playerY), std::round(playerX) - 400 * std::sin(graphics.getCameraAngle()*3.14159/180), std::round(playerY) - 400 * std::cos(graphics.getCameraAngle()*3.14159/180), 1000));
            //lifetime is 1 second
    }
    else {          //if it did collide with a surface

        this->_gunShotPaths.push_back(GunshotPath(graphics, std::round(playerX), std::round(playerY), shotCollision.x, shotCollision.y, 1000)); //lifetime is 1 second

    }
    
    //print perpindicular shot
    
    
    
    //this->_gunShotPaths.push_back(GunshotPath(graphics, 0, bPerpindicular, 1280, mPerpindicular * 1280 + bPerpindicular, 1000));
    
    
    //graphics.storeLineDebug(0, bPerpindicular, 1280, mPerpindicular * 1280 + bPerpindicular,0);
    //printf("b: %f \t m: %f\n",bPerpindicular,mPerpindicular);
    
    //find (x, y) where lines collide
    // y = mPerpindicular * x + bPerpindicular
    // y = mShot * x + bShot
    
    //x = (-Bperp + Bshot) / (Mperp - Mshot)
    //y = (Bshot*Mperp - Bperp*Mshot)/(Mperp - Mshot)
    
    double closestPointX = (-bPerpindicular + bShot) / (mPerpindicular - mShot);
    double closestPointY = (bShot*mPerpindicular - bPerpindicular*mShot)/(mPerpindicular - mShot);
    
    double shotCollisionDistance = std::sqrt(std::pow(shotCollision.x - playerX,2) + std::pow(shotCollision.y - playerY,2));
    double closestColllisionDistance = std::sqrt(std::pow(closestPointX - playerX,2) + std::pow(closestPointY - playerY,2));
    
    if((shotCollision.x == 0 && shotCollision.y == 0) || (shotCollisionDistance > closestColllisionDistance)) { //if no collision, or shot collision is farther from player than closest point
        this->_gunShotPaths.push_back(GunshotPath(graphics, this->_unit.getStaticX()+8, this->_unit.getStaticY()+12, closestPointX, closestPointY, 1000));
    }
    //this->_gunShotPaths.push_back(GunshotPath(graphics, this->_unit.getStaticX()+8, this->_unit.getStaticY()+12, closestPointX, closestPointY, 1000));
    
}

void Level::clearGunshotPaths() {
    this->_gunShotPaths.clear();
}

void Level::moveUnitAngleToSlideAngle(Graphics &graphics) {
    this->_unit.addToAngleOrders(this->_slide.getAngle());
}

void Level::drawFogOfWar(Graphics &graphics) {
    //erase last frame's polygon corners       // only if unit moves, TODO
    this->_polygonCorners.clear();
    
    
    //first check camera corners
    double cameraX = -graphics.getCameraX();
    double cameraY = -graphics.getCameraY();
    std::vector<Vector2> corners = {Vector2(cameraX+1,cameraY+1), Vector2(cameraX + 1280, cameraY), Vector2(cameraX + 1280, cameraY + 800), Vector2(cameraX, cameraY + 800)};   //change the +1 of the top left corner
    
    std::vector<PolygonCorner> polygonEdges;
    
    double playerX = 0.0;
    double playerY = 0.0;
    
    for(Vector2 iter : corners) {
        playerX = -graphics.getCameraX() + graphics.getPlayerCenterX();
        playerY = -graphics.getCameraY() + graphics.getPlayerCenterY();
        
        //find angle to point           /I shoudl for sure make this its own function. I use it a lot
        double xdiff = iter.x - (playerX);
        double ydiff = iter.y - (playerY);
        
        graphics.drawLine(playerX + graphics.getCameraX(), playerY + graphics.getCameraY(), iter.x + graphics.getCameraX(), iter.y + graphics.getCameraY());
        
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
        
        bool view = checkShotCollisionFoWCorner(playerX, playerY, angle, iter.x, iter.y);
        
        if(view) {
            //this->_polygonCorners.push_back(PolygonCorner(iter.x, iter.y, angle - 180, 0));
            polygonEdges.push_back(PolygonCorner(iter.x, iter.y, angle, 0, 0));
        }
        
        /*
        //don't return a corner
        //Vector2 view = checkShotCollisionNew(playerX, playerY, angle);                 //may be way faster to do this with mx + b. Since atan takes a lot of cycles
        
        double distanceToNode = std::sqrt(std::pow(xdiff,2) + pow(ydiff,2));
        
        Vector2 view = checkShotCollisionFoW(playerX, playerY, angle, iter.x, iter.y);
        if(!(view.x == 0 && view.y == 0)) { // if there was a collision
            double distanceToCollision = std::sqrt(std::pow(playerX - view.x,2) + pow(playerY - view.y,2));
            
            if(distanceToNode > distanceToCollision) {
                //then corner is visible
                //add corner to polygon corner
                this->_polygonCorners.push_back(PolygonCorner(iter.x, iter.y, angle, 0));
                
                
                
            }
            
        }
         */
        
    }
    
    //int count = 0;
    //Get corners of polygon
    for(FoWNode iter : this->_FoWNode) {
        //check if corner is visible
        
        //iter = this->_FoWNode.at(6);
        
        double playerX = -graphics.getCameraX() + graphics.getPlayerCenterX();
        double playerY = -graphics.getCameraY() + graphics.getPlayerCenterY();
        
        //find angle to point           /I shoudl for sure make this its own function. I use it a lot
        double xdiff = iter.x - (playerX);
        double ydiff = iter.y - (playerY);
        
        graphics.drawLine(playerX + graphics.getCameraX(), playerY + graphics.getCameraY(), iter.x + graphics.getCameraX(), iter.y + graphics.getCameraY());
        
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
        
        //don't return a corner
        //Vector2 view = checkShotCollisionNew(playerX, playerY, angle);                 //may be way faster to do this with mx + b. Since atan takes a lot of cycles
        
        Vector2 view = checkShotCollisionFoW(playerX, playerY, angle, iter.x, iter.y);
        
        double distanceToNode = std::sqrt(std::pow(xdiff,2) + std::pow(ydiff,2));

        
        if(!(view.x == 0 && view.y == 0)) { // if there was a collision
            double distanceToCollision = std::sqrt(std::pow(playerX - view.x,2) + pow(playerY - view.y,2));
            
            if(distanceToNode < distanceToCollision) {
                //then corner is visible
                //add corner to polygon corner
                this->_polygonCorners.push_back(PolygonCorner(iter.x, iter.y, angle, 0, iter.isInnerCorner));

                bool lookOver = shouldNextCollisionBeIncluded(playerX, playerY, iter);
                
                if(lookOver) {
                    this->_polygonCorners.push_back(PolygonCorner(view.x, view.y, angle, 1, iter.isInnerCorner));
                }
                
            }

        }
        /*              //there will always be a collision, if only with the camera bounds
        else { //no collision. Corner is visible
            //add corner to polygon corner
            this->_polygonCorners.push_back(PolygonCorner(iter.x, iter.y, angle, 0));
            
            //add edge of screen to polygon corner                                          //TODO
            bool lookOver = shouldNextCollisionBeIncluded(playerX, playerY, iter);
            //printf("no collision\n");
            
            count++;
            
            
            
        }
         */
    }
    
    //sort polygon nodes
    
    //std::vector<PolygonCorner> sortedPolygonCorners;
    
    
    std::sort(this->_polygonCorners.begin(), this->_polygonCorners.end());
    
    //bool shouldSwap = 0;
    
    //for(int i = 0; i < this->_polygonCorners.size(); i++) {
    //    printf("%f \t %d \n",this->_polygonCorners.at(i).angle, this->_polygonCorners.at(i).isExtension);
    //}
    //printf("\n\n");
    
    /*
    for(int i = 0; i < this->_polygonCorners.size(); i++) {
        if(this->_polygonCorners.at(i).isExtension) {
            //printf("%f \t",this->_polygonCorners.at(i).angle);
            if(shouldSwap) {
                std::iter_swap(this->_polygonCorners.begin() + i, this->_polygonCorners.begin() + i + 1);
            }
            shouldSwap = !shouldSwap;
        }
        //printf("%f \t %d \n",this->_polygonCorners.at(i).angle, this->_polygonCorners.at(i).isExtension);
    }
    //printf("\n\n");
    */
    
    
    
    
    
    /*
    int i = 0;
    
    
    double distanceToPrevExtension = 0.0;
    double distanceToCurrentNode = 0.0;
    
    
    while(i < this->_polygonCorners.size()) {
        
        if(this->_polygonCorners.at(i).isExtension) {
            
            distanceToCurrentNode = std::sqrt(std::pow( playerX - this->_polygonCorners.at(i+1).x    ,2) + std::pow(    playerY - this->_polygonCorners.at(i+1).y             ,2));
            
            if(distanceToCurrentNode > distanceToPrevExtension) {
                //don't swap
            }
            else {
                //swap
                std::iter_swap(this->_polygonCorners.begin() + i, this->_polygonCorners.begin() + i + 1);
                
            }
            
            distanceToPrevExtension = std::sqrt(std::pow( playerX - this->_polygonCorners.at(i).x        ,2) + std::pow(   playerY -  this->_polygonCorners.at(i).y      ,2));
            
            i++;
        }
        //if distance to next 0 in (0,1) pair is farther from next 1 in (0,1) pair, then go 0 -> 1, 0 -> 1

        
        
        i++;
    }
    */
    
    
    
    /*
    int totalDoubles = 0;
    for(PolygonCorner iter : this->_polygonCorners) {
        totalDoubles += iter.isExtension;
    }
    */
    
    
    //rotate so we start with a 1
    int firstOneLocation = 0;
    for(int i = 0; i < this->_polygonCorners.size(); i++) {     //always going to be at least 2 1s so long as there is never a closed building
        if(this->_polygonCorners.at(i).isExtension) {
            break;
        }
        firstOneLocation++;
    }
    
    //std::rotate(this->_polygonCorners.begin(), this->_polygonCorners.begin() + firstOneLocation, this->_polygonCorners.end());
   
    
    
    
    //std::rotate(this->_polygonCorners.begin(), this->_polygonCorners.begin()+this->_polygonCorners.size()-firstOneLocation, this->_polygonCorners.end());
    
    
    
    
    
    
    //bool shouldSwap = 0;
    
    int i = 0;
    while(i < this->_polygonCorners.size()) {
        
        if(this->_polygonCorners.at(i).isExtension) {

            //the next one will share an x or y coordinate with the previous
            
            //check to see which of the next two share an x or y with the previous and start with that one
            
            if(i > 0) {
                if((this->_polygonCorners.at(i).x == this->_polygonCorners.at(i-1).x) || (this->_polygonCorners.at(i).y == this->_polygonCorners.at(i-1).y)) {
                    //then don't swap
                    i++;
                }
                else  {
                    std::iter_swap(this->_polygonCorners.begin() + i, this->_polygonCorners.begin() + i + 1);
                    i++;
                }
                
            }
        
            
            
            
            /*
            if(this->_polygonCorners.at(i).isInnerCorner) {
                std::iter_swap(this->_polygonCorners.begin() + i, this->_polygonCorners.begin() + i + 1);
                i++;
            }
            else {
                if(shouldSwap) {
                    std::iter_swap(this->_polygonCorners.begin() + i, this->_polygonCorners.begin() + i + 1);
                    i++;
                }
                shouldSwap = !shouldSwap;
            }
            */

        }

        
        i++;
    }
    
    
    //depending on size of array
    
    
    
    
    
    
    
    for(int i = 0; i < this->_polygonCorners.size(); i++) {
        printf("%f \t %d \n",this->_polygonCorners.at(i).angle, this->_polygonCorners.at(i).isExtension);
    }
    printf("\n\n");
    
    
    std::sort(polygonEdges.rbegin(), polygonEdges.rend());  //reverse sorting
    
    
    /*
    //then add the invisible corners
    for(int i = 0; i < polygonEdges.size(); i++) {
        this->_polygonCorners.push_back(polygonEdges.at(i));
    }
    */
    
    
    
    
    
    //draw polygon
    drawPolygonCorners(graphics);
    
    
    
    //put the corners through a rotation matrix
    
    
    //then draw them
    //if(totalDoubles % 2 == 0) {
        graphics.drawPolygon(this->_polygonCorners);
   //}
    
    
}

bool Level::arePointsVeryClose(Vector2 node1, Vector2 node2) {
    double distance = std::sqrt(std::pow(node1.x - node2.x,2) + std::pow(node1.y - node2.y,2));
    if(distance < 4.0) {
        return 1;
    }
    return 0;
}

void Level::drawPolygonCorners(Graphics &graphics) {
    //printf("polygonCorners size: %d\n", this->_polygonCorners.size());
    for(PolygonCorner iter : this->_polygonCorners) {
        graphics.drawCircle(iter.x + graphics.getCameraX(), iter.y + graphics.getCameraY());
    }
}

bool Level::checkShotCollisionFoWCorner(double beginx, double beginy, double angle, double endx, double endy) {
    
    
    float collisionx = 0.0;
    float collisiony = 0.0;
    
    float m = 1.0 / std::tan(angle*3.14159/180);
    float b = beginy - m*beginx;
    
    
    for(Rectangle i : this->_collisionRects) {
        //check bottom
        collisionx = (i.getStartY()+i.getHeight() - b)/m;
        collisiony = i.getStartX() * m + b;
        if((i.getStartY() < (beginy - i.getHeight()) && std::abs(angle) < 90)) {
            if(collisionx > i.getStartX() && collisionx < i.getStartX()+i.getWidth()) {
                
                
                return 1;
                
                
                
            }
        }
        
        
        
        //check top
        collisionx = (i.getStartY() - b)/m;
        collisiony = i.getStartX() * m + b;
        if((i.getStartY() > beginy && std::abs(angle) > 90)) {
            if(collisionx > i.getStartX() && collisionx < i.getStartX()+i.getWidth()) {
                
                return 1;
                
            }
        }
        
        
        
        //check left
        collisionx = (i.getStartY() - b)/m;
        collisiony = i.getStartX() * m + b;
        if((i.getStartX() > beginx) && angle < 0) {
            if(collisiony > i.getStartY() && collisiony < i.getStartY()+i.getHeight()) {
                
                return 1;
                
                
            }
        }
        
        //check right
        collisionx = (i.getStartY() - b)/m;
        collisiony = (i.getStartX()+i.getWidth()) * m + b;
        if((i.getStartX() < beginx - i.getWidth()) && angle > 0) {
            if(collisiony > i.getStartY() && collisiony < i.getStartY()+i.getHeight()) {
                
                return 1;
                
                
            }
        }
    }
    
    
    return 0;
    
    
}


Vector2 Level::checkShotCollisionFoW(double beginx, double beginy, double angle, double endx, double endy) {
    
    
    //need to also include boundaries of the camera
    
    
    //printf("%f\n",angle);
    
    float m = 1.0 / std::tan(angle*3.14159/180);
    
    float b = beginy - m*beginx;
    
    float collisionx = 0.0;
    float collisiony = 0.0;
    
    std::vector<Vector2> collisionPoints;
    
    Vector2 cornerVector(endx, endy);
    
    //first do camera boudaries
    
    //angle will be -180 < angle < 180
    
    double tmp = 180.0 / 3.14159 * std::atan(640.0/400.0);
    
    
    //check top
    //y = mx + b
    //x =
    
    collisionx = ((beginy-400)-b)/m;
    collisiony = beginy + 400;                          //based on where the player is located on screen. Same as cameraY
    if(std::abs(angle) < tmp) {
        collisionPoints.push_back(Vector2(collisionx, beginy - 400));
        //return Vector2(300, 300);
    }
    //printf("top: %f\n", 180.0 / 3.14159 * std::atan(640.0/400.0));
    
    double tmp1 = (360.0 - tmp*2.0) / 2.0;
    
    //check right                                                                   //only one of these 4 works. probably not the one i expect. dont use 1280 and 400, use the static location values
    collisionx = beginx + 640;
    collisiony = m * (beginx + 640) + b;
    if(-angle > tmp && -angle < tmp1) {
        collisionPoints.push_back(Vector2(beginx + 640, collisiony));
        //return Vector2(400, 400);
        
    }
    
    //check left
    collisionx = beginx - 640;
    collisiony = m*(beginx - 640) + b;
    if(angle > -tmp && angle < (tmp1)) {
        collisionPoints.push_back(Vector2(beginx - 640, collisiony));
        //return Vector2(200, 400);
    }
    
    //check bottom
    collisionx = ((beginy + 400) - b) / m;
    collisiony = beginy + 400;
    if(std::abs(angle) > tmp1) {
        //printf("printing\n");
        collisionPoints.push_back(Vector2(collisionx, beginy + 400));
        //return Vector2(300, 500);
    }
    
    
    
    //printf("angle: %f\n", angle);
    
    
    for(Rectangle i : this->_collisionRects) {
        //check bottom
        collisionx = (i.getStartY()+i.getHeight() - b)/m;
        collisiony = i.getStartX() * m + b;
        if((i.getStartY() < (beginy - i.getHeight()) && std::abs(angle) < 90)) {
            if(collisionx > i.getStartX() && collisionx < i.getStartX()+i.getWidth()) {
                

                
                
                Vector2 collisionVector(collisionx, i.getStartY()+i.getHeight());
                if(!arePointsVeryClose(cornerVector, collisionVector)) {        //if the collision is not the corner we are testing
                    collisionPoints.push_back(collisionVector);
                }
                

                
            }
        }
        
        
        
        //check top
        collisionx = (i.getStartY() - b)/m;
        collisiony = i.getStartX() * m + b;
        if((i.getStartY() > beginy && std::abs(angle) > 90)) {
            if(collisionx > i.getStartX() && collisionx < i.getStartX()+i.getWidth()) {
                
                Vector2 collisionVector(collisionx, i.getStartY());
                if(!arePointsVeryClose(cornerVector, collisionVector)) {        //if the collision is not the corner we are testing
                    collisionPoints.push_back(collisionVector);
                }
                //collisionPoints.push_back(Vector2(collisionx, i.getStartY()));
                
            }
        }
        
        
        
        //check left
        collisionx = (i.getStartY() - b)/m;
        collisiony = i.getStartX() * m + b;
        if((i.getStartX() > beginx) && angle < 0) {
            if(collisiony > i.getStartY() && collisiony < i.getStartY()+i.getHeight()) {
                
                //collisionPoints.push_back(Vector2(i.getStartX(),collisiony));
                
                Vector2 collisionVector(i.getStartX(), collisiony);
                
                if(!arePointsVeryClose(cornerVector, collisionVector)) {        //if the collision is not the corner we are testing
                    collisionPoints.push_back(collisionVector);
                }
                
                
            }
        }
        
        //check right
        collisionx = (i.getStartY() - b)/m;
        collisiony = (i.getStartX()+i.getWidth()) * m + b;
        if((i.getStartX() < beginx - i.getWidth()) && angle > 0) {
            if(collisiony > i.getStartY() && collisiony < i.getStartY()+i.getHeight()) {
                
                //collisionPoints.push_back(Vector2(i.getStartX()+i.getWidth(), collisiony));
                
                
                Vector2 collisionVector(i.getStartX()+i.getWidth(), collisiony);
                
                if(!arePointsVeryClose(cornerVector, collisionVector)) {        //if the collision is not the corner we are testing
                    collisionPoints.push_back(collisionVector);
                }
                
                
            }
        }
    }
    
    if(collisionPoints.size() > 0) {
        
        double min = 1000000;
        double weight = 0.0;
        int nearest = -1;
        
        for(int i = 0; i<collisionPoints.size(); i++) {
            //weight = std::sqrt(std::pow(collisionPoints[i].x - 640,2) + std::pow(collisionPoints[i].y - 400,2));
            weight = std::sqrt(std::pow(collisionPoints[i].x - beginx,2) + std::pow(collisionPoints[i].y - beginy,2));
            if(weight < min) {
                min = weight;
                nearest = i;
            }
        }
        
        return collisionPoints[nearest];
        
    }
    
    
    
    //no collision
    return Vector2(0, 0);                           //maybe change this? maybe it should be negative, then have the corner of the map at (0,0)
    
    
    
}

FoWNodePosition Level::intToNodePosition(int position) {
    switch(position) {
        case 0:
            return TL;
            break;
        case 1:
            return TR;
            break;
        case 2:
            return BR;
            break;
        case 3:
            return BL;
            break;
        case 4:
            return IN;
            break;
        default:
            return IN;
            break;
    }
}

bool Level::shouldNextCollisionBeIncluded(double playerX, double playerY, FoWNode iter) {
    
    switch(iter.positionOnRectangle) {
        case(TL):
            if(((playerX < iter.x) && (playerY > iter.y)) || ((playerX > iter.x) && (playerY < iter.y))) {
                return 1;
            }
            return 0;
            break;
        case(TR):
            if(((playerX > iter.x) && (playerY > iter.y)) || ((playerX < iter.x) && (playerY < iter.y))) {
                return 1;
            }
            return 0;
            break;
        case(BR):
            if(((playerX < iter.x) && (playerY > iter.y)) || ((playerX > iter.x) && (playerY < iter.y))) {  //same as TL
                return 1;
            }
            return 0;
            break;
        case(BL):
            if(((playerX > iter.x) && (playerY > iter.y)) || ((playerX < iter.x) && (playerY < iter.y))) {  //same as TR
                return 1;
            }
            return 0;
            break;
        case(IN):
            return 0;
            break;
        default:
            return 0;
            break;
    }
}

void Level::changeDrawFoVNode() {
    this->_drawFovNode = !this->_drawFovNode;
}

