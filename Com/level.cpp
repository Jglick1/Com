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
    Map(graphics, "/Users/jonahglick/Documents/Com/com_test6.png", 0, 0, 1280, 800, 0, 0),
    //Map(graphics, "/Users/jonahglick/Documents/Com/dehast.png", 0, 0, 1280, 1920, 0, 0),
	_mapName(mapName),
	_size(Vector2(0,0)),
    _drawFovNode(0)
{
    
	this->loadMap(mapName, graphics);
    
    this->_unit = Unit(graphics, Vector2(0,0), 1);
    

    this->_fireteam = Fireteam(graphics, 1, Vector2(100,100));
    
    this->_fireteam2 = Fireteam(graphics, 1, Vector2(640, 50));
    
    this->_fireteam3 = Fireteam(graphics, 1, Vector2(800, 400));
    
    //Unit * unitPointer = new Unit(graphics, Vector2(100,100), 1);
    //std::unique_ptr<Unit> tmp(new Unit(graphics, Vector2(100,100), 1) );
    //this->_fireteam.addUnit( std::move(tmp) );                          //need to std::move the unique pointer
    std::shared_ptr<Unit> tmp(new Unit(graphics, Vector2(100, 100), 1) );
    this->_fireteam.addUnit( std::move(tmp) );
    
    
    std::shared_ptr<Unit> tmp2(new Unit(graphics, Vector2(200, 200), 1) );
    this->_fireteam.addUnit( std::move(tmp2) );
    
    
    std::shared_ptr<Unit> tmp3(new Unit(graphics, Vector2(300, 300), 1) );
    this->_fireteam.addUnit( std::move(tmp3) );
    
    
    //this->_unit.moveToPosition(600, 800);

    
    //this->_map = Map(graphics, "/Users/jonahglick/Documents/Com/com_test1.png", 0, 0, 320, 200, 0, 0);
    
    
    //vector of unit types <enemies>
    //vector of unit types <allies>
    //printf("test\n");
    
    this->_enemyFireteam = Fireteam(graphics, 0, Vector2(700, 700));
    
    
    
    
    
    std::shared_ptr<Unit> tmp4(new Unit(graphics, Vector2(100 + 400, 100+ 400), 0) );
    this->_enemyFireteam.addUnit( std::move(tmp4) );
    
    std::shared_ptr<Unit> tmp5(new Unit(graphics, Vector2(200+ 400, 200+ 400), 0) );
    this->_enemyFireteam.addUnit( std::move(tmp5) );
    
    std::shared_ptr<Unit> tmp6(new Unit(graphics, Vector2(300+ 400, 300+ 400), 0) );
    this->_enemyFireteam.addUnit( std::move(tmp6) );
    
    
    
    //fireteam2
    std::shared_ptr<Unit> tmp7(new Unit(graphics, Vector2(500, 100), 1) );
    this->_fireteam2.addUnit( std::move(tmp7) );
    
    std::shared_ptr<Unit> tmp8(new Unit(graphics, Vector2(600, 100), 1) );
    this->_fireteam2.addUnit( std::move(tmp8) );
    
    std::shared_ptr<Unit> tmp9(new Unit(graphics, Vector2(700, 100), 1) );
    this->_fireteam2.addUnit( std::move(tmp9) );
    
    
    
    //fireteam3
    std::shared_ptr<Unit> tmp10(new Unit(graphics, Vector2(900, 400), 1) );
    this->_fireteam3.addUnit( std::move(tmp10) );
    
    std::shared_ptr<Unit> tmp11(new Unit(graphics, Vector2(1000, 400), 1) );
    this->_fireteam3.addUnit( std::move(tmp11) );
    
    //std::shared_ptr<Unit> tmp12(new Unit(graphics, Vector2(1100, 700), 1) );
    //this->_fireteam3.addUnit( std::move(tmp12) );
    
    
    this->_slide = ControlSlide(graphics,Vector2(500, 500), 1);
    
    

}

Level::~Level() {
}



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

    
    //std::map<int, Vector2> tempForPreferredCover;
    std::vector< std::vector<double> > tempForPreferredCover;
    
	
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
                        
                        
                        Building building(x, y, width, height, 0.0, id);
                        
                        this->_buildings.push_back(building);
                        
                    
                        
                        
                        pObject = pObject->NextSiblingElement("object");
                    }
                }
                
                
            }
            
            else if (ss.str() == "preferredCover") {                                         //needs to come after graph has been created. graph is created in "corners" so "corner" must come first
                
                //add all building rects to cover building table
                XMLElement * pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        
                        int buildingID = pObject->IntAttribute("name");
                        int x = pObject->IntAttribute("x");
                        int y = pObject->IntAttribute("y");
                        
                        
                        printf("size: %d\n", tempForPreferredCover.size());
                        //tempForPreferredCover.insert(std::make_pair(buildingID, Vector2(x, y)));
                        tempForPreferredCover.push_back({double(buildingID), double(x), double(y)});    //cast as doubles
                        
                        
                        printf("size: %d\n", tempForPreferredCover.size());
                        
                        printf("\n");
                        
                        
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
            if (ss.str() == "structure") {
                XMLElement * pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
                        
                        //float x, y, width, height;
                        
                        double startx = pObject->FloatAttribute("x");
                        double starty = pObject->FloatAttribute("y");
                        
                        double name = pObject->IntAttribute("name");
                        
                        std::vector<Vector2> tmp;
                        std::vector<Direction> tmp2;
                        
                        //tmp.push_back(Vector2(startx, starty)); //the first position
                        
                        
                        XMLElement * polyLine = pObject->FirstChildElement("polyline");     //only 1 polyline per structure object
                        if(polyLine != NULL) {
                         
                            std::stringstream polyLineSS;
                            
                            polyLineSS << polyLine->Attribute("points");
                            
                            //printf("string stream:\n");
                            //std::cout << polyLineSS.str() << std::endl;
                            
                            double oldN = -1, oldT = -1;
                            double n, t;
                            char tmp1;
                            while(polyLineSS >> n) {
                                
                                polyLineSS >> tmp1;    //ignore comma and spaces
                                polyLineSS >> t;
                                
                                if(oldN != -1) {
                                    tmp2.push_back(findWallDirection(n, t, oldN, oldT));
                                }
                                
                                
                                tmp.push_back(Vector2(startx + n, starty + t));
                                oldN = n;
                                oldT = t;
                                
                                //printf("%f\n", n);
                            }
                            
                            
                            //for (Vector2 &iter : tmp) {
                            //    printf("%d, %d\n", iter.x, iter.y);
                            //}
                            
                            
                            /*
                            //check to see if this structure has already been constructed
                            bool alreadyConstructed = 0;
                            for(Structure &iter : this->_structures) {
                                if(iter.name == name) {
                                    alreadyConstructed = 1;
                                }
                            }
                            if(alreadyConstructed) {
                                
                            }
                            else {
                                
                            }
                            */
                            //door does not need to be tied to structure
                            
                            
                            this->_structures.push_back(Structure(tmp, 0.0, tmp2, 5, name));
                            
                            
                            
                            
                        }
                        
                        
                        
                    
                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }
            
            if (ss.str() == "door") {
                XMLElement * pObject = pObjectGroup->FirstChildElement("object");
                if (pObject != NULL) {
                    while (pObject) {
        
                        double startx = pObject->FloatAttribute("x");
                        double starty = pObject->FloatAttribute("y");
                        
                        double name = pObject->IntAttribute("name");
                        
                        std::vector<Vector2> tmp;
                        Direction doorDirection;

                        XMLElement * polyLine = pObject->FirstChildElement("polyline");
                        if(polyLine != NULL) {
                            
                            std::stringstream polyLineSS;
                            
                            polyLineSS << polyLine->Attribute("points");

                            double oldN = -1, oldT = -1;
                            double n, t;
                            char tmp1;
                            while(polyLineSS >> n) {
                                
                                polyLineSS >> tmp1;    //ignore comma and spaces
                                polyLineSS >> t;
                                
                                if(oldN != -1) {
                                    doorDirection = findWallDirection(n, t, oldN, oldT);
                                }
                                
                                
                                tmp.push_back(Vector2(startx + n, starty + t));
                                oldN = n;
                                oldT = t;

                            }

                            this->_doors.push_back(Door(tmp[0].x, tmp[0].y, tmp[1].x, tmp[1].y, doorDirection));

                            printf("x1: %d, y1: %d, x2: %d, y2: %d \n", tmp[0].x, tmp[0].y, tmp[1].x, tmp[1].y);
                            
                        
                        }

                        pObject = pObject->NextSiblingElement("object");
                    }
                }
            }
            
            
            
			
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
    
    
    //so all buildings have been created
    //now insert preferred cover nodes into the appropriate building struct
    
    //printf("preferredCover : %d\n", tempForPreferredCover.size());
    
    for(auto const& coverIter : tempForPreferredCover) {
        
        //printf("preferredCover\n");
        
        for(Building & buildingIter : this->_buildings) {
            if(buildingIter.id == coverIter[0]) {
                buildingIter.addPreferredCover(Vector2(coverIter[1], coverIter[2]));
            }
        }

        
        //iter.first; //building id
        //iter.second; //Vector2 location

        
    }
    
    
    
    
    
    
    
}

void Level::update(int elapsedTime, Graphics &graphics) {


    for(int i  = 0; i < this->_collisionRects.size(); i++) {
        this->_collisionRects.at(i).update(elapsedTime, graphics.getCameraDx(), graphics.getCameraDy());
    }
    

    
    this->_unit.update(elapsedTime, graphics);
    this->_slide.update(elapsedTime, graphics);
    
    this->_fireteam.update(elapsedTime, graphics);
    this->_fireteam2.update(elapsedTime, graphics);
    this->_fireteam3.update(elapsedTime, graphics);
    this->_enemyFireteam.update(elapsedTime, graphics);
    
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
    
    Map::drawTrans(graphics, posx, posy, graphics.getCameraAngle(), posx, posy);
    
    //draw collision rects
    for(int i = 0; i<this->_collisionRects.size(); i++) {
        graphics.drawRect(std::round(this->_collisionRects.at(i).getX()), std::round(this->_collisionRects.at(i).getY()), this->_collisionRects.at(i).getWidth(), this->_collisionRects.at(i).getHeight());
    }
    
    //draw collision polygon
    for (Structure &structureIter : this->_structures) {
        for(int i = 0; i < structureIter.corners.size() - 1; i++) {
            double x3 = structureIter.corners.at(i).x;
            double y3 = structureIter.corners.at(i).y;
            double x4 = structureIter.corners.at(i+1).x;
            double y4 = structureIter.corners.at(i+1).y;
            
            graphics.drawLine(x3, y3, x4, y4);
            
            
        }
    }
    

    
    
    //draw unit collision rects
    //Rectangle unitRec = this->_unit.getCollisionRect();

    //graphics.drawRect(std::round(unitRec.getX()), std::round(unitRec.getY()), unitRec.getWidth(), unitRec.getHeight());
    
    
    
    
    
    //graphics.drawLine(640, 400, 640 - 100*std::sin(this->_angle*3.14159/180), 400 - 100*std::cos(this->_angle*3.14159/180) );
    
    graphics.drawLine(640, 400, 640 - 100*std::sin(graphics.getCameraAngle()*3.14159/180), 400 - 100*std::cos(graphics.getCameraAngle()*3.14159/180) );
    
    //graphics.drawLine(10,10,800,800);
    
    
    
    /*
    Vector2 coll = checkShotCollision(640, 400, graphics.getCameraAngle());
    
    if(!((coll.x == 0) && (coll.y == 0))) {                         //CHNAGE THIS TO NOT BE 0!
        graphics.drawRect(coll.x - 5, coll.y - 5, 10, 10);
    }
    */


    
    //unit
    this->_unit.draw(graphics);
    
    this->_fireteam.draw(graphics);
    this->_fireteam2.draw(graphics);
    this->_fireteam3.draw(graphics);
    this->_enemyFireteam.draw(graphics);
    
    //float xEquivalent = this->_unit.getStaticX() + this->_unit.getX()+8;
    //float yEquivalent = this->_unit.getStaticY() + this->_unit.getY()+12;
    
    float xEquivalent = this->_unit.getStaticX() + graphics.getCameraX() + 8;
    float yEquivalent = this->_unit.getStaticY() + graphics.getCameraY() + 12;
    
    float angleEquivalent = -this->_unit.getAngle();    //the opposite angle of level
    
    //in both cases we change the - sin for x into a + sin
    graphics.drawLine(xEquivalent, yEquivalent, xEquivalent - 100*std::sin(angleEquivalent*3.14159/180), yEquivalent - 100*std::cos(angleEquivalent*3.14159/180) );

    /*
    coll = checkShotCollision(xEquivalent, yEquivalent, angleEquivalent);
    
    
    if(!((coll.x == 0) && (coll.y == 0))) {                         //CHNAGE THIS TO NOT BE 0!
        graphics.drawRect(coll.x - 5, coll.y - 5, 10, 10);
    }
*/
    
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



const Vector2 Level::getPlayerSpawnPoint() const {
	return this->_spawnPoint;
}

void Level::handleSlideRelease(int xm, int ym, Graphics &graphics) {
    
    //find the slide position
    //this changes this->_unitMovePositions to find the appropriate positions we want the selected units to move to
    handleSlideMovement(xm, ym, graphics, 1);

    //find the slide in question
    
    if(this->_fireteam.isSelected()) {
        if(this->_fireteam.isCenterSelected()) {
            //this->_fireteam.moveToSlidePosition(this->_graph, graphics);
            this->_fireteam.moveToSlidePosition(this->_graph, graphics, this->_unitMovePositions);
        }
        else {
            //printf("center not selected\n");
            this->_fireteam.moveToSlideAngle();
        }
            
        this->_fireteam.centerSlideToZero();
        
        
        
    }
    else if(this->_enemyFireteam.isSelected()) {
        if(this->_enemyFireteam.isCenterSelected()) {
            this->_enemyFireteam.moveToSlidePosition(this->_graph, graphics);
        }
        else {
            //printf("center not selected\n");
            this->_enemyFireteam.moveToSlideAngle();
        }
        
        
        this->_enemyFireteam.centerSlideToZero();
        
        
    }
    else if(this->_slide.isSelected()) {
        centerSlideToZero();
        
        moveUnitToSlidePosition(graphics);
        //moveUnitAngleToSlideAngle(graphics);
        
        
    }
    else if(this->_fireteam2.isSelected()) {
        if(this->_fireteam2.isCenterSelected()) {
            //this->_fireteam.moveToSlidePosition(this->_graph, graphics);
            this->_fireteam2.moveToSlidePosition(this->_graph, graphics, this->_unitMovePositions);
        }
        else {
            //printf("center not selected\n");
            this->_fireteam2.moveToSlideAngle();
        }
        
        this->_fireteam2.centerSlideToZero();
    }
    else if(this->_fireteam3.isSelected()) {
        if(this->_fireteam3.isCenterSelected()) {
            //this->_fireteam.moveToSlidePosition(this->_graph, graphics);
            this->_fireteam3.moveToSlidePosition(this->_graph, graphics, this->_unitMovePositions);
        }
        else {
            //printf("center not selected\n");
            this->_fireteam3.moveToSlideAngle();
        }
        
        this->_fireteam3.centerSlideToZero();
    }
    


}



//http://www.jeffreythompson.org/collision-detection/line-circle.php
void Level::handlePlayerCollisions(double elapsedTime, Graphics &graphics) {
    
    double playerX = -graphics.getCameraX() + graphics.getPlayerCenterX();
    double playerY = -graphics.getCameraY() + graphics.getPlayerCenterY();
    
    double closestX = -1;
    double closestY = -1;
    
    std::vector< std::vector<double> > collisionLines;
    std::vector<Direction> collisionDirections;
    
    for (Structure &structureIter : this->_structures) {

        
        for(int i = 0; i < structureIter.corners.size() - 1; i++) {
            double x1 = structureIter.corners.at(i).x;
            double y1 = structureIter.corners.at(i).y;
            double x2 = structureIter.corners.at(i+1).x;
            double y2 = structureIter.corners.at(i+1).y;
            
            if(lineCircle(x1, y1, x2, y2, playerX, playerY, 8, closestX, closestY)) { //radius is 8
                //collisionLines.push_back({x3, y3, x4, y4});
                //graphics.storeLineDebug(x1, y1, x2, y2, 1);
                //printf("collision\n");
                //printDirection(structureIter.directions.at(i));
                
                double xdiff = playerX - closestX;
                double ydiff = playerY - closestY;
                
                double dist = std::sqrt(    (xdiff*xdiff) + (ydiff*ydiff)  );
                
                collisionLines.push_back({x1, y1, x2, y2, closestX, closestY, dist});
                collisionDirections.push_back(structureIter.directions.at(i));
                
            }
        }
    }

    
    if(collisionLines.size() == 1) {
        
        if(collisionDirections.at(0) == UP) {
            graphics.setCameraX(-collisionLines[0][4] + 640);
            graphics.setCameraY(-collisionLines[0][5] + 400 + 8);
            
            //graphics.setCameraX(-200.0);
            //graphics.setCameraY(-200.0);
            
            //printf("%f, %f\n",collisionLines[0][4], collisionLines[0][5]);
            
            //move player to closestX
            //move player to closestY + 16
        }
        else if(collisionDirections.at(0) == DOWN) {
            graphics.setCameraX(-collisionLines[0][4] + 640);
            graphics.setCameraY(-collisionLines[0][5] + 400 - 8);
        }
        else if(collisionDirections.at(0) == RIGHT) {
            graphics.setCameraX(-collisionLines[0][4] + 640 - 8);
            graphics.setCameraY(-collisionLines[0][5] + 400);
        }
        else if(collisionDirections.at(0) == LEFT) {
            graphics.setCameraX(-collisionLines[0][4] + 640 + 8);
            graphics.setCameraY(-collisionLines[0][5] + 400);
        }
        
        
        
    }
    else if(collisionLines.size() == 2) {
        
        //printf("twosies\n");
        //printf("\n");
        //for (Direction iter : collisionDirections) {
        //    printDirection(iter);
        //}
        
        int first = -1;
        int second = -1;
        //find which one to handle first
        
        //if one closest point is on the line, start with that one
        
        if(linePoint(collisionLines[0][0], collisionLines[0][1], collisionLines[0][2], collisionLines[0][3], collisionLines[0][4], collisionLines[0][5])) {
            //then do this one
            first = 0;
            second = 1;
        }
        else if (linePoint(collisionLines[1][0], collisionLines[1][1], collisionLines[1][2], collisionLines[1][3], collisionLines[1][4], collisionLines[1][5])) {
            first = 1;
            second = 0;
        }
        else {
            //if they are both off, choose one based on distance
            if(collisionLines[0][6] >= collisionLines[1][6]) {
                first = 0;
                second = 1;
            }
            else {
                first = 1;
                second = 0;
            }
        }
        
        
        
        
        //playerX = -graphics.getCameraX() + graphics.getPlayerCenterX();
        //playerY = -graphics.getCameraY() + graphics.getPlayerCenterY();
        //printf("ZEROTH \t playerX: %f, playerY: %f\n", playerX, playerY);
        
        //handle first one
        if(collisionDirections.at(first) == UP) {
            graphics.setCameraX(-collisionLines[first][4] + 640);
            graphics.setCameraY(-collisionLines[first][5] + 400 + 8);
        }
        else if(collisionDirections.at(first) == DOWN) {
            graphics.setCameraX(-collisionLines[first][4] + 640);
            graphics.setCameraY(-collisionLines[first][5] + 400 - 8);
        }
        else if(collisionDirections.at(first) == RIGHT) {
            graphics.setCameraX(-collisionLines[first][4] + 640 - 8);
            graphics.setCameraY(-collisionLines[first][5] + 400);
        }
        else if(collisionDirections.at(first) == LEFT) {
            graphics.setCameraX(-collisionLines[first][4] + 640 + 8);
            graphics.setCameraY(-collisionLines[first][5] + 400);
        }
        
        
        
        //then, if still colliding, handle second
        playerX = -graphics.getCameraX() + graphics.getPlayerCenterX();
        playerY = -graphics.getCameraY() + graphics.getPlayerCenterY();
        //printf("FIRST \t playerX: %f, playerY: %f\n", playerX, playerY);
        
        closestX = -1;
        closestY = -1;
        
        //printf("%d\n", second);
        //graphics.storeLineDebug(collisionLines[first][0], collisionLines[first][1], collisionLines[first][2], collisionLines[first][3], 1);
        //graphics.storeLineDebug(collisionLines[second][0], collisionLines[second][1], collisionLines[second][2], collisionLines[second][3], 1);
        //printf("%f, %f, %f, %f\n",collisionLines[second][0], collisionLines[second][1], collisionLines[second][2], collisionLines[second][3]);
        if(lineCircle(collisionLines[second][0], collisionLines[second][1], collisionLines[second][2], collisionLines[second][3], playerX, playerY, 8, closestX, closestY)) {
            
            //graphics.storeLineDebug(collisionLines[1][0], collisionLines[1][1], collisionLines[1][2], collisionLines[1][3], 1);
            
            //printf("%f, %f, %f, %f\n", collisionLines[1][0], collisionLines[1][1], collisionLines[1][2], collisionLines[1][3] );
            
            //printf("second\n");
            
            if(collisionDirections.at(second) == UP) {
                graphics.setCameraX(-closestX + 640);
                graphics.setCameraY(-closestY + 400 + 8);
            }
            else if(collisionDirections.at(second) == DOWN) {
                graphics.setCameraX(-closestX + 640);
                graphics.setCameraY(-closestY + 400 - 8);
            }
            else if(collisionDirections.at(second) == RIGHT) {
                graphics.setCameraX(-closestX + 640 - 8);
                graphics.setCameraY(-closestY + 400);
            }
            else if(collisionDirections.at(second) == LEFT) {
                graphics.setCameraX(-closestX + 640 + 8);
                graphics.setCameraY(-closestY + 400);
            }
            
            
            
        }
        
        //playerX = -graphics.getCameraX() + graphics.getPlayerCenterX();
        //playerY = -graphics.getCameraY() + graphics.getPlayerCenterY();
        //printf("SECOND \t playerX: %f, playerY: %f\n", playerX, playerY);
        
        
        
        
    }
    
    
    
    //printf("%f, %f\n", graphics.getPlayerX(), graphics.getPlayerY());
    
    
    
    
}

bool Level::closestPointOnLine(double x1, double y1, double x2, double y2, double px, double py, double &closestx, double &closesty, Direction direction, int fireteamSize) {
    
    // get length of the line
    double distX = x1 - x2;
    double distY = y1 - y2;
    double len = std::sqrt( (distX*distX) + (distY*distY) );
    
    
    // get dot product of the line and circle
    double dot = ( ((px-x1)*(x2-x1)) + ((py-y1)*(y2-y1)) ) / std::pow(len,2);
    
    // find the closest point on the line
    double closestX = x1 + (dot * (x2-x1));
    double closestY = y1 + (dot * (y2-y1));
    
    closestx = closestX;
    closesty = closestY;

    

    bool onSegment = linePoint(x1, y1, x2, y2, closestX, closestY);
    if (!onSegment) return false;
    
    
    //change closest points if on edge of line
    
    bool inside1 = pointCircle(x1, y1, closestX, closestY, 8 + 8*(fireteamSize-1));
    bool inside2 = pointCircle(x2, y2, closestX, closestY, 8 + 8*(fireteamSize-1));
    
    if(inside1) {
        //put on one side of the edge
        switch(direction) {                         //change this with changing building angle
            case RIGHT:
                closestx = x1;
                closesty = y1 - (8 + 8*(fireteamSize-1));
                break;
            case UP:
                closestx = x1 - (8 + 8*(fireteamSize-1));
                closesty = y1;
                break;
            case LEFT:
                closestx = x1;
                closesty = y1 + (8 + 8*(fireteamSize-1));
                break;
            case DOWN:
                closestx = x1 + (8 + 8*(fireteamSize-1));
                closesty = y1;
                break;
        }
    }
    else if(inside2) {
        switch(direction) {                         //change this with changing building angle
            case RIGHT:
                closestx = x2;
                closesty = y2 + (8 + 8*(fireteamSize-1));
                break;
            case UP:
                closestx = x2 + (8 + 8*(fireteamSize-1));
                closesty = y2;
                break;
            case LEFT:
                closestx = x2;
                closesty = y2 - (8 + 8*(fireteamSize-1));
                break;
            case DOWN:
                closestx = x2 - (8 + 8*(fireteamSize-1));
                closesty = y2;
                break;
        }
    }
    
    return true;
    
    
}


// LINE/CIRCLE
bool Level::lineCircle(double x1, double y1, double x2, double y2, double cx, double cy, double r, double &closestx, double &closesty) {
    
    
    // is either end INSIDE the circle?
    // if so, return true immediately
    bool inside1 = pointCircle(x1,y1, cx,cy,r);
    bool inside2 = pointCircle(x2,y2, cx,cy,r);
    //if (inside1 || inside2) return true;
    
    
    
    // get length of the line
    double distX = x1 - x2;
    double distY = y1 - y2;
    double len = std::sqrt( (distX*distX) + (distY*distY) );
    
    // get dot product of the line and circle
    double dot = ( ((cx-x1)*(x2-x1)) + ((cy-y1)*(y2-y1)) ) / std::pow(len,2);
    
    // find the closest point on the line
    double closestX = x1 + (dot * (x2-x1));
    double closestY = y1 + (dot * (y2-y1));
    
    closestx = closestX;
    closesty = closestY;
    
    
    
    if (inside1 || inside2) return true;
    
    
    
    
    
    
    // is this point actually on the line segment?
    // if so keep going, but if not, return false
    bool onSegment = linePoint(x1,y1,x2,y2, closestX,closestY);
    if (!onSegment) return false;
    
    
    /*
    // optionally, draw a circle at the closest
    // point on the line
    fill(255,0,0);
    noStroke();
    ellipse(closestX, closestY, 20, 20);
    */
    
    
    // get distance to closest point
    distX = closestX - cx;
    distY = closestY - cy;
    float distance = std::sqrt( (distX*distX) + (distY*distY) );
    
    if (distance <= r) {
        return true;
    }
    return false;
    
    
    
}

// POINT/CIRCLE
bool Level::pointCircle(double px, double py, double cx, double cy, double r) {
    
    // get distance between the point and circle's center
    // using the Pythagorean Theorem
    double distX = px - cx;
    double distY = py - cy;
    double distance = std::sqrt( (distX*distX) + (distY*distY) );
    
    // if the distance is less than the circle's
    // radius the point is inside!
    return (distance <= r);
}

double Level::dist(double x1, double y1, double x2, double y2) {
    
    double distX = x1 - x2;
    double distY = y1 - y2;
    
    return std::sqrt( (distX*distX) + (distY*distY) );
    
}

// LINE/POINT
bool Level::linePoint(double x1, double y1, double x2, double y2, double px, double py) {
    
    // get distance from the point to the two ends of the line
    double d1 = dist(px,py, x1,y1);
    double d2 = dist(px,py, x2,y2);
    
    // get the length of the line
    double lineLen = dist(x1,y1, x2,y2);
    
    // since floats are so minutely accurate, add
    // a little buffer zone that will give collision
    double buffer = 0.1;    // higher # = less accurate
    
    // if the two distances are equal to the line's
    // length, the point is on the line!
    // note we use the buffer here to give a range,
    // rather than one #
    if (d1+d2 >= lineLen-buffer && d1+d2 <= lineLen+buffer) {
        return true;
    }
    return false;
}



/*
void Level::handleTileCollisions(std::vector<Rectangle> &others, float elapsedTime, Graphics &graphics) { //other are the level's collision rects
    

    Rectangle playerRec = Rectangle(640 - 8, 400 - 8, 16, 16);
    
    //y: 392
    
    for (int i = 0; i < others.size(); i++) {
        
        //printf("y: %d, height: %d\n", others.at(i).getY(), others.at(i).getHeight());
        
        sides::Side collisionSide = playerRec.getCollisionSide(others.at(i));
        
        //printf("map: %d, player: %d",others.at(i).getHeight(),playerRec.getHeight());
        

        
        
        
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
    
    
    //this->_unit.handleTileCollisions(others, elapsedTime);
    this->_slide.handleTileCollisions(others, elapsedTime);
    
    
    
}
*/




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
    
    bool temp = checkPathCollision(this->_unit.getStaticX()+8, this->_unit.getStaticY()+4+8, posX+8, posY+8, graphics);
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
            temp = checkPathCollision(this->_unit.getStaticX()+8, this->_unit.getStaticY()+4+8, iter.second.x+8, iter.second.y+8,graphics);
            printf("vertex: %d \t collision:%d\n", iter.first, temp);
            
            
            tempDestination = checkPathCollision(posX+8, posY+8, iter.second.x+8, iter.second.y+8,graphics);
            
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
    
    bool collisions = 0;
    
    collisions += this->_slide.checkSlideCollision(xm, ym);
    
    
    
    
    //for each fireteam
    collisions += this->_fireteam.checkSlideCollision(xm, ym);
    
    collisions += this->_enemyFireteam.checkSlideCollision(xm, ym);
    
    collisions += this->_fireteam2.checkSlideCollision(xm, ym);
    
    collisions += this->_fireteam3.checkSlideCollision(xm, ym);
    
    return collisions;       //can only have 1 collision so this is 0 or 1
    
    
}



void Level::handleSlideMovement(int xm, int ym, Graphics &graphics, bool once) {
    this->_slide.handleSlideMovement(xm, ym, this->_angle, graphics.getCameraX(), graphics.getCameraY(), graphics);
    
    
    //find closest wall and light it up
    
    
    //if not in building
    bool inBuilding = 0;
    
    
    //need to know which fireteam is selected
    Vector2 slideCenter = this->_fireteam.getSlideCenter();
    int fireteamSize = this->_fireteam.getSize();
    
    //Vector2 slideCenter = selectedFireteam->getSlideCenter();
    //int fireteamSize = selectedFireteam->getSize();
    
    if(this->_fireteam.isSelected()) {
        slideCenter = this->_fireteam.getSlideCenter();
        fireteamSize = this->_fireteam.getSize();
    }
    else if(this->_fireteam2.isSelected()) {
        slideCenter = this->_fireteam2.getSlideCenter();
        fireteamSize = this->_fireteam2.getSize();
    }
    else if(this->_fireteam3.isSelected()) {
        slideCenter = this->_fireteam3.getSlideCenter();
        fireteamSize = this->_fireteam3.getSize();
    }
    
    
    for(Building &iter : this->_buildings) {
        
        if(slideCenter.x > iter.innerds.x && slideCenter.x < iter.innerds.x + iter.innerds.width) {
            if(slideCenter.y > iter.innerds.y && slideCenter.y < iter.innerds.y + iter.innerds.height) {
                inBuilding = 1;
                //printf("inBuilding\n");
            }
        }
        
    }
    
    
    /*
    for(Rectangle &iter : this->_buildings) {
        
        if(slideCenter.x > iter.getX() && slideCenter.x < iter.getX() + iter.getWidth()) {
            if(slideCenter.y > iter.getY() && slideCenter.y < iter.getY() + iter.getHeight()) {
                inBuilding = 1;
                //printf("inBuilding\n");
            }
        }
        
    }
    */
    
    std::vector<Vector2> positions;
    
    //check angle of slider
    
    if(!inBuilding) {
        
        //get structure outside bounds function
        
        
        //add this number to structure struct
        
        double minDist = 1000000;
        double disToLine = 0.0;
        double closestX = 0.0;
        double closestY = 0.0;
        
        double closestOfClosestX = -1;
        double closestOfClosestY = -1;
        
        int linePosition = -1;
        
        bool atLeastOne = 0;
        
        bool isDoor = 0;
        
        Direction closestLineDirection;
        
        Door closestDoor = Door();
        
        int doorLength = 0;
        Direction doorDirection = NONE;
        
        std::vector<double> closestLine;
        
        
        //find closest line
        
        //look over structure lines
        for (Structure &structureIter : this->_structures) {
            for(int i = 0; i < 5; i++) {                        //5 is the number of outside walls
                double x3 = structureIter.corners.at(i).x;
                double y3 = structureIter.corners.at(i).y;
                double x4 = structureIter.corners.at(i+1).x;
                double y4 = structureIter.corners.at(i+1).y;
                
                //graphics.storeLineDebug(x3, y3, x4, y4, 1);
                
                if(closestPointOnLine(x3, y3, x4, y4, slideCenter.x, slideCenter.y, closestX, closestY, structureIter.directions.at(i), fireteamSize)) {
                    //graphics.storeLineDebug(x3, y3, x4, y4, 1);
                    
                    atLeastOne = 1;
                    
                    
                    //disToLine = std::sqrt(closestX*closestX + closestY*closestY);
                    
                    disToLine = std::sqrt(std::pow(slideCenter.x - closestX,2) + std::pow(slideCenter.y - closestY,2));
                    
                    if(disToLine < minDist) {
                        minDist = disToLine;
                        closestOfClosestX = closestX;
                        closestOfClosestY = closestY;
                        linePosition = i;
                        isDoor = 0;
                        closestLine = {x3, y3, x4, y4};
                        
                    }
                }
            }
        }
        
        //look over door lines
        for(Door iter : this->_doors) {
            
            if(closestPointOnLine(iter.x1, iter.y1, iter.x2, iter.y2, slideCenter.x, slideCenter.y, closestX, closestY, NONE, 0)) { //change this
                //graphics.storeLineDebug(iter.x1, iter.y1, iter.x2, iter.y2, 1);
                
                atLeastOne = 1;
                
                
                //disToLine = std::sqrt(closestX*closestX + closestY*closestY);
                
                disToLine = std::sqrt(std::pow(slideCenter.x - closestX,2) + std::pow(slideCenter.y - closestY,2));
                
                if(disToLine < minDist) {
                    minDist = disToLine;
                    //closestOfClosestX = closestX;
                    //closestOfClosestY = closestY;
                    //linePosition = -1;
                    isDoor = 1;
                    //doorLength = std::abs(iter.x1 - iter.x2) + std::abs(iter.y1 - iter.y2);
                    closestDoor = iter;
                    
                }
            }
            
            
            
            
        }
        
        
        //draw the appropriate circle
        
        if(atLeastOne) {
            //graphics.storeDebugCircle(closestOfClosestX, closestOfClosestY, 8);
            
            //printf("%d \t", linePosition);
            //printf("X: %f \t Y: %f\n", closestOfClosestX, closestOfClosestY);
            
            //std::vector<Vector2> positions;
            positions.clear();
            
            if(isDoor) {
                switch(closestDoor.direction) {
                    case DOWN:
                        graphics.storeRotatedDebugCircle(closestDoor.x1 - 8, closestDoor.y1 + 8, 8);
                        graphics.storeRotatedDebugCircle(closestDoor.x2 + 8, closestDoor.y2 + 8, 8);
                        break;
                    case UP:
                        graphics.storeRotatedDebugCircle(closestDoor.x1 - 8, closestDoor.y1 - 8, 8);
                        graphics.storeRotatedDebugCircle(closestDoor.x2 + 8, closestDoor.y2 - 8, 8);
                        break;
                    case RIGHT:
                        graphics.storeRotatedDebugCircle(closestDoor.x1 + 8, closestDoor.y1 + 8, 8);
                        graphics.storeRotatedDebugCircle(closestDoor.x2 + 8, closestDoor.y2 - 8, 8);
                        break;
                    case LEFT:
                        graphics.storeRotatedDebugCircle(closestDoor.x1 - 8, closestDoor.y1 + 8, 8);
                        graphics.storeRotatedDebugCircle(closestDoor.x2 - 8, closestDoor.y2 - 8, 8);
                        break;
                }
                //previously storeDebugCircle

            }
            else {
                
                
                //this is close
                
                //graphics.storeDebugCircle(closestOfClosestX + (y2 - y1)/8, closestOfClosestY + (x1 - x2)/8, 8);
                
                
                //this is right
                //https://stackoverflow.com/questions/1243614/how-do-i-calculate-the-normal-vector-of-a-line-segment
                double lineLength = dist(closestLine[0], closestLine[1], closestLine[2], closestLine[3]);
                
                //graphics.storeDebugCircle(closestOfClosestX - 8 * (closestLine[3] - closestLine[1])/lineLength, closestOfClosestY - 8 * (closestLine[0] - closestLine[2])/lineLength, 8);
                
                double centerX = closestOfClosestX - 8 * (closestLine[3] - closestLine[1])/lineLength;
                double centerY = closestOfClosestY - 8 * (closestLine[0] - closestLine[2])/lineLength;
                
                
                
                
                //graphics.storeDebugCircle(centerX, centerY, 8);
                
                //graphics.storeDebugCircle(centerX + 8 * (y2 - y1)/dist, centerY + 8 * (x2 - x1)/dist, 8);
                //graphics.storeDebugCircle(centerX + 8 * (closestLine[2] - closestLine[0])/lineLength, centerY + 8 * (closestLine[3] - closestLine[1])/lineLength, 8);
                

                for(int i = 0; i < fireteamSize; i++) {
                    positions.push_back(Vector2(centerX + (-8*(fireteamSize-1) + 16*i) * (closestLine[2] - closestLine[0])/lineLength, centerY + (-8*(fireteamSize-1) + 16*i) * (closestLine[3] - closestLine[1])/lineLength));
                }
                
                
                
                /*
                for(int i = 0; i < fireteamSize; i++) {
                    positions.push_back(Vector2(closestOfClosestX - 4 * fireteamSize + 16 * i, closestOfClosestY + 8));
                }
                */

                
                
                /*
                if(linePosition == 0) {  //go down
                    
                    for(int i = 0; i < fireteamSize; i++) {
                        positions.push_back(Vector2(closestOfClosestX - 4 * fireteamSize + 16 * i, closestOfClosestY + 8));
                    }

                    
                }
                else if (linePosition == 1) {   //go right
                    graphics.storeDebugCircle(closestOfClosestX + 8, closestOfClosestY, 8);
                    
                }
                else if(linePosition == 2) {    //go up
                    graphics.storeDebugCircle(closestOfClosestX, closestOfClosestY - 8, 8);
                    
                }
                else if (linePosition == 3) {   //go left
                    graphics.storeDebugCircle(closestOfClosestX - 8, closestOfClosestY, 8);
                    
                }
                else if(linePosition == 4) {    //go down
                    graphics.storeDebugCircle(closestOfClosestX, closestOfClosestY + 8, 8);
                }
                 
                 */
            }

            
            for (Vector2 i : positions) {
                graphics.storeRotatedDebugCircle(i.x, i.y, 8);
            }

                 
                 
        }
        
    }
    
    
    
    else {      //it is in a building
        //printf("f\n");
        
        for(Vector2 &it : this->_buildings.at(0).preferredCover) {
            //printf("hello\n");
            graphics.storeDebugCircle(it.x + 8, it.y + 8, 8);
            
        }
        
        
        //graphics.storeDebugCircle(closestOfClosestX + 8, closestOfClosestY, 8);
        
        
    }
    
    
    
    //this is the fireteam slide
    this->_fireteam.handleSlideMovement(xm, ym, this->_angle, graphics.getCameraX(), graphics.getCameraY(), graphics);
    
    this->_fireteam2.handleSlideMovement(xm, ym, this->_angle, graphics.getCameraX(), graphics.getCameraY(), graphics);
    
    this->_fireteam3.handleSlideMovement(xm, ym, this->_angle, graphics.getCameraX(), graphics.getCameraY(), graphics);
    
    this->_enemyFireteam.handleSlideMovement(xm, ym, this->_angle, graphics.getCameraX(), graphics.getCameraY(), graphics);
    
    //find the closest, most appropriate positions
    
    
    //shift the positions
    for(int i = 0; i < positions.size(); i++) {
        positions[i].x -= 8;
        positions[i].y -= 8;
    }
    
    if(once) {
        this->_unitMovePositions = positions;
    }
    
    
    
}



void Level::centerSlideToZero(){
    this->_slide.centerSlideToZero();
}


bool Level::checkPathCollision(int beginx, int beginy, int endx, int endy, Graphics &graphics) {
    
    //GET ANGLE TO PATH
    
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
    
    
    //GET STARTING TOP AND BOTTOM POINTS
    
    double topBeginPosX = beginx - 8 * std::cos(angle*3.14159/180);      //radius of unit is 8
    double topBeginPosY = beginy + 8 * std::sin(angle*3.14159/180);
    
    double bottomBeginPosX = beginx + 8 * std::cos(angle*3.14159/180);
    double bottomBeginPosY = beginy - 8 * std::sin(angle*3.14159/180);
    
    
    
    double topEndPosX = endx - 8 * std::cos(angle*3.14159/180);      //radius of unit is 8
    double topEndPosY = endy + 8 * std::sin(angle*3.14159/180);
    
    double bottomEndPosX = endx + 8 * std::cos(angle*3.14159/180);
    double bottomEndPosY = endy - 8 * std::sin(angle*3.14159/180);
    
    
    //this->_gunShotPaths.push_back(GunshotPath(graphics, topBeginPosX, topBeginPosY, topEndPosX, topEndPosY, 6000));
    //this->_gunShotPaths.push_back(GunshotPath(graphics, bottomBeginPosX, bottomBeginPosY, bottomEndPosX, bottomEndPosY, 6000));

    bool collision = 0;
    
    
    //check collisions with all structure lines
    for (Structure &structureIter : this->_structures) {
        for(int i = 0; i < structureIter.corners.size() - 1; i++) {
            double x3 = structureIter.corners.at(i).x;
            double y3 = structureIter.corners.at(i).y;
            double x4 = structureIter.corners.at(i+1).x;
            double y4 = structureIter.corners.at(i+1).y;
            
            if(isLineLineCollision(topBeginPosX, topBeginPosY, topEndPosX, topEndPosY, x3, y3, x4, y4) || isLineLineCollision(bottomBeginPosX, bottomBeginPosY, bottomEndPosX, bottomEndPosY, x3, y3, x4, y4)) {
                
                
                
                collision = 1;
                
                
                
                
            }
        }
    }
    
    
    
    
    
    
    return collision;
    
    
}


Vector2 Level::checkShotCollisionNewNew(double beginx, double beginy, double endx, double endy, Graphics &graphics) {
    
    std::vector<Vector2> collisionPoints;
    std::vector< std::vector<double> > collisionLines;
    
    
    for (Structure &structureIter : this->_structures) {
        for(int i = 0; i < structureIter.corners.size() - 1; i++) {
            double x3 = structureIter.corners.at(i).x;
            double y3 = structureIter.corners.at(i).y;
            double x4 = structureIter.corners.at(i+1).x;
            double y4 = structureIter.corners.at(i+1).y;
            
            if(isLineLineCollision(beginx, beginy, endx, endy, x3, y3, x4, y4)) {
                collisionLines.push_back({x3, y3, x4, y4});
                //graphics.storeMapLineDebug(x3, y3, x4, y4, 1);
            }
        }
    }
    
    //printf("size: %lu\n", collisionLines.size());
    
    for (int i = 0; i < collisionLines.size(); i++) {

        Vector2 loc = whereLineLineCollision(beginx, beginy, endx, endy, collisionLines.at(i).at(0), collisionLines.at(i).at(1), collisionLines.at(i).at(2), collisionLines.at(i).at(3));
        
        collisionPoints.push_back(loc);
        //graphics.storeDebugCircle(loc.x, loc.y, 10);
        
    }

    
    
    
    if(collisionPoints.size() > 0) {
        
        double min = 1000000;
        double weight = 0.0;
        int nearest = -1;
        
        for(int i = 0; i<collisionPoints.size(); i++) {
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
    
    //Vector2 shotCollision = checkShotCollision(playerX, playerY, graphics.getCameraAngle());
    
    
    double shotDistance = 400;
    double shotEndX = std::round(playerX) - shotDistance * std::sin(graphics.getCameraAngle()*3.14159/180);
    double shotEndY = std::round(playerY) - shotDistance * std::cos(graphics.getCameraAngle()*3.14159/180);
    
    
    
    
    //check collision with surfaces
    Vector2 newShotCollision = checkShotCollisionNewNew(playerX, playerY, shotEndX, shotEndY, graphics);
    
    //check collision with enemies
    Vector2 enemyCollision = this->_enemyFireteam.checkUnitCollision(playerX, playerY, shotEndX, shotEndY);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //print player shot
    

    if(newShotCollision.x == 0 && newShotCollision.y == 0) {   //if the shot did not collide with anything
        //graphics.storeMapLineDebug(0, bShot, 1280, mShot * 1280 + bShot, 0);
        
        //make sure shot only goes 400 pixels

        
        this->_gunShotPaths.push_back(GunshotPath(graphics, std::round(playerX), std::round(playerY), shotEndX, shotEndY, 1000));
        
        
    }
    else {          //if it did collide with a surface

        this->_gunShotPaths.push_back(GunshotPath(graphics, std::round(playerX), std::round(playerY), newShotCollision.x, newShotCollision.y, 1000)); //lifetime is 1 second

    }
    
    
    
    
    //for calculating experience gain. is this incentivizing wasting bullets?
    

    
    
    
    
    
    
    
    
    
    
    
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
    
    
    for (Structure &structureIter : this->_structures) {
        for(int i = 0; i < structureIter.corners.size() - 1; i++) {
            double x3 = structureIter.corners.at(i).x;
            double y3 = structureIter.corners.at(i).y;
            double x4 = structureIter.corners.at(i+1).x;
            double y4 = structureIter.corners.at(i+1).y;
            
            if(isLineLineCollision(beginx, beginy, endx, endy, x3, y3, x4, y4)) {
                return 1;
            }
               
               
        }
    }
    
               
    return 0;
    

    
    
}


Vector2 Level::checkShotCollisionFoW(double beginx, double beginy, double angle, double endx, double endy) {
    
    
    std::vector<Vector2> collisionPoints;
    
    
    for (Structure &structureIter : this->_structures) {
        for(int i = 0; i < structureIter.corners.size() - 1; i++) {
            double x3 = structureIter.corners.at(i).x;
            double y3 = structureIter.corners.at(i).y;
            double x4 = structureIter.corners.at(i+1).x;
            double y4 = structureIter.corners.at(i+1).y;
            
            if(isLineLineCollision(beginx, beginy, endx, endy, x3, y3, x4, y4)) {
                
                
                
                
                
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

//http://www.jeffreythompson.org/collision-detection/poly-line.php
bool Level::isLineLineCollision(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    
    double uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    double uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    
    return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
    
    
}

//http://flassari.is/2008/11/line-line-intersection-in-cplusplus/
Vector2 Level::whereLineLineCollision(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {

    
    // If d is zero, there is no intersection
    double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    /*
    if(d == 0) {
        return Vector2(-1, -1);     //no collision. Check!!
    }
    */
    
    //get x and y
    double pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
    double x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
    double y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
    
    /*
    // Check if the x and y coordinates are within both lines
    if ( x < std::min(x1, x2) || x > std::max(x1, x2) ||
        x < std::min(x3, x4) || x > std::max(x3, x4) ) return Vector2(-1,-1);
    if ( y < std::min(y1, y2) || y > std::max(y1, y2) ||
        y < std::min(y3, y4) || y > std::max(y3, y4) ) return Vector2(-1,-1);
    */
    //printf("%f, %f\n", x, y);
    
    return Vector2(int(x), int(y));
    
    
    
}

double Level::distToLine(double playerX, double playerY, double x1, double y1, double x2, double y2) {
    double m = (y2 - y1) / (x2 - x1);
    
    
    //y = (1/m) x + b
    double bPerp = playerY - (1/m) * playerX;
    
    
    
    
    return 0.0;
    
}

bool Level::approxEqual(double x1, double x2) {
    
    return (x1 - x2 < 0.1);
    
}

Direction Level::findWallDirection(double x1, double y1, double x2, double y2) {
    
    double xdiff = x2 - x1;
    double ydiff = y2 - y1;
    
    if(std::abs(ydiff) > std::abs(xdiff)) {
        if(ydiff > 0) {
            return RIGHT;
        }
        else {
            return LEFT;
        }
    }
    else {
        if(xdiff > 0) {
            return UP;
        }
        else {
            return DOWN;
        }
    }
    
    return NONE;
    
    
}

void Level::printDirection(Direction direction) {
    switch(direction) {
        case UP:
            printf("UP\n");
            break;
        case DOWN:
            printf("DOWN\n");
            break;
        case RIGHT:
            printf("RIGHT\n");
            break;
        case LEFT:
            printf("LEFT\n");
            break;
        case UPRIGHT:
            printf("UPRIGHT\n");
            break;
        case UPLEFT:
            printf("UPLEFT\n");
            break;
        case DOWNRIGHT:
            printf("DOWNRIGHT\n");
            break;
        case DOWNLEFT:
            printf("DOWNLEFT\n");
            break;
        case NONE:
            printf("NONE\n");
            break;
    }
}



void Level::moveUnitAssignment() {
    //move unit from this->_fireteam3
    //to this->_fireteam1;
    
    //this works!
    
    std::shared_ptr<Unit> tmp = this->_fireteam3.getUnitPointer();
    tmp->printFirstName();
    
    this->_fireteam.addUnit(tmp);
    
}

Fireteam Level::returnFireteam(int fireteamNumber) {
    switch(fireteamNumber) {
        case 0:
            return this->_fireteam;
            break;
        case 1:
            return this->_fireteam2;
            break;
        case 2:
            return this->_fireteam3;
            break;
        default:
            return this->_fireteam;
            break;
    }
}
