//
//  fireteam.cpp
//  Com
//
//  Created by Jonah Glick on 7/15/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "fireteam.hpp"

template <typename T>
void delete_pointed_to(T* const ptr)
{
    delete ptr;
}

Fireteam::Fireteam() {}

Fireteam::Fireteam(Graphics &graphics, bool allied, Vector2 spawnPoint) :
_numUnits(0)
{
    if(allied) {
        this->_controlSlide = ControlSlide(graphics, spawnPoint, allied);
    }
    else {
        this->_controlSlide = ControlSlide(graphics, spawnPoint, allied);
    }
    
    printf("spawnPoint, x: %d, y %d\n", spawnPoint.x, spawnPoint.y);
    
}

Fireteam::~Fireteam() {                             //MEMORY LEAKS?
    //delete all units
    
    /*
    for (std::vector< Unit * >::iterator it = this->_units.begin() ; it != this->_units.end(); ++it)
    {
        delete (*it);
    }
    this->_units.clear();
    */
    //std::for_each(this->_units.begin(), this->_units.end(), delete_pointed_to<Unit>);
    
    
}


void Fireteam::draw(Graphics &graphics) {
    //draw all units


    if(this->_controlSlide.isSelected()) {
        for (std::shared_ptr<Unit>& iter : this->_units) {
            iter->draw(graphics, 1);
            
        }
    }
    else {
        for (std::shared_ptr<Unit>& iter : this->_units) {
            iter->draw(graphics, 0);
            
        }
    }
    
    
    //draw control slide
    
    this->_controlSlide.draw(graphics);

    
}



void Fireteam::update(int elapsedTime, Graphics &graphics) {
    //update all units
    for (std::shared_ptr<Unit> &iter : this->_units) {
        iter->handleMovement();
        iter->update(elapsedTime, graphics);

        
    }
    
    
    
    //update controlSlide
    this->_controlSlide.update(elapsedTime, graphics);
    
    
}



void Fireteam::addUnit(std::shared_ptr<Unit> unit) {
    this->_units.push_back(std::move(unit));
    //this->_units.push_back(unit);
    
    this->_numUnits++;
    
}



int Fireteam::checkSlideCollision(int xm, int ym) {
    return this->_controlSlide.checkSlideCollision(xm, ym);
}

void Fireteam::handleSlideMovement(int xm, int ym, double angle, float levelx, float levely, Graphics &graphics) {

    
    this->_controlSlide.handleSlideMovement(xm, ym, angle, levelx, levely, graphics);

    
}

double Fireteam::getSlideAngle() {
    return this->_controlSlide.getAngle();
}

void Fireteam::centerSlideToZero() {
    this->_controlSlide.centerSlideToZero();
}

bool Fireteam::isSelected() {
    return this->_controlSlide.isSelected();
}

void Fireteam::moveToSlideAngle() {
    
    for (std::shared_ptr<Unit> &iter : this->_units) {
        iter->addToAngleOrders(this->_controlSlide.getAngle());
        //printf("unit move\n");
    }

}


void Fireteam::moveToSlidePosition(Graph &graph, Graphics &graphics) {                      //ignore all obsticles for now
    
    if(this->_numUnits == 1) {
        //this->_units.at(0)->moveToPosition(this->_controlSlide.getStaticX() + 75 - 8, this->_controlSlide.getStaticY() + 12 - 8);
        
        this->_units.at(0)->addToMovementOrders(Vector2(this->_controlSlide.getStaticX() + 75 - 8, this->_controlSlide.getStaticY() + 12 - 8));
        this->_units.at(0)->addToAngleOrders(this->_controlSlide.getAngle());
        //this->_units.at(0)->moveToPosition(this->_controlSlide.getStaticX() + 75 - 8, this->_controlSlide.getStaticY() + 12 - 8);
        
    }
    
    
    else if(this->_numUnits == 2) {
        
        double radius = 60;
        double angle_seperation = 90;   //degrees
        
        
        //find positions to move to
        Vector2 slideLocation(this->_controlSlide.getStaticX() + 75, this->_controlSlide.getStaticY() + 12);

        
        //this changes with slide angle change
        Vector2 centerOfCircle(slideLocation.x, slideLocation.y + radius);
        

        //for 0 angle
        Vector2 pos1(centerOfCircle.x + radius * std::cos((90 - angle_seperation / 2.0) * 3.14159 / 180.0) , centerOfCircle.y - radius * std::sin((90 - angle_seperation / 2.0) * 3.14159 / 180.0) );
        Vector2 pos2(centerOfCircle.x - radius * std::cos((90 - angle_seperation / 2.0) * 3.14159 / 180.0) , centerOfCircle.y - radius * std::sin((90 - angle_seperation / 2.0) * 3.14159 / 180.0) );
        
        
        //Vector2 pos1(slideLocation.x - 20,slideLocation.y + 20);
        //Vector2 pos2(slideLocation.x + 20,slideLocation.y + 20);

        pos1.x -= 8;
        pos1.y -= 8;
        
        pos2.x -= 8;
        pos2.y -= 8;
        
        
        //find which unit to move where to get there in the shortest amount of time
        
        this->_units.at(0)->addToMovementOrders(pos1);
        this->_units.at(0)->addToAngleOrders(this->_controlSlide.getAngle());
        //this->_units.at(0)->moveToPosition(pos1.x, pos1.y);
        
        
        this->_units.at(1)->addToMovementOrders(pos2);
        this->_units.at(1)->addToAngleOrders(this->_controlSlide.getAngle());
        //this->_units.at(1)->moveToPosition(pos2.x, pos2.y);
        
        graphics.storeDebugCircle(centerOfCircle.x, centerOfCircle.y, radius);
        
        
        
    }
    
    else if(this->_numUnits == 3) {
        
        double radius = 60;
        double angle_seperation = 60;   //degrees
        
        //find positions to move to
        Vector2 slideLocation(this->_controlSlide.getStaticX() + 75, this->_controlSlide.getStaticY() + 12);
        
        
        //this changes with slide angle change
        Vector2 centerOfCircle(slideLocation.x, slideLocation.y + radius);
        
        
        //for 0 angle
        Vector2 pos1(centerOfCircle.x + radius * std::cos((90 - angle_seperation) * 3.14159 / 180.0) , centerOfCircle.y - radius * std::sin((90 - angle_seperation) * 3.14159 / 180.0) );
        Vector2 pos2(centerOfCircle.x, centerOfCircle.y - radius);
        Vector2 pos3(centerOfCircle.x - radius * std::cos((90 - angle_seperation) * 3.14159 / 180.0) , centerOfCircle.y - radius * std::sin((90 - angle_seperation) * 3.14159 / 180.0) );
        
        pos1.x -= 8;
        pos1.y -= 8;
        
        pos2.x -= 8;
        pos2.y -= 8;
        
        pos3.x -= 8;
        pos3.y -= 8;
        
        
        //find which unit to move where to get there in the shortest amount of time
        this->_units.at(0)->addToMovementOrders(pos1);
        this->_units.at(0)->addToAngleOrders(this->_controlSlide.getAngle());
        this->_units.at(1)->addToMovementOrders(pos2);
        this->_units.at(1)->addToAngleOrders(this->_controlSlide.getAngle());
        this->_units.at(2)->addToMovementOrders(pos3);
        this->_units.at(2)->addToAngleOrders(this->_controlSlide.getAngle());
        
        graphics.storeDebugCircle(centerOfCircle.x, centerOfCircle.y, radius);

    }
    
    
}

void Fireteam::moveToSlidePosition(Graph &graph, Graphics &graphics, std::vector<Vector2> positions) {
    
    int count = 0;
    for(Vector2 it : positions) {
        this->_units.at(count)->addToMovementOrders(it);
        count++;
    }

    
}



bool Fireteam::isCenterSelected() {
    return this->_controlSlide.isCenterSelected();
}

Vector2 Fireteam::getSlideCenter() {
    
    return this->_controlSlide.getCenter();
    
}


Vector2 Fireteam::checkUnitCollision(double x1, double y1, double x2, double y2) {                  //FIX THIS
    //iterate over every unit in the fireteam
    
    // get length of the line
    double distX = x1 - x2;
    double distY = y1 - y2;
    double len = std::sqrt( (distX*distX) + (distY*distY) );
    
    double dot = 0.0;
    double closestX = 0.0;
    double closestY = 0.0;
    
    bool onSegment = 0;
    
    double dist = 100.0;
    
    bool oneCollision = 0;
    
    double closestClosestX = 0.0;
    double closestClosestY = 0.0;

    
    
    
    std::vector<int> unitsToErase;
    
    int index = 0;
    for (std::shared_ptr<Unit> &iter : this->_units) {
        //iter->handleMovement();
        //iter->update(elapsedTime, graphics);
        
        double px = iter->getStaticX() + 8;
        double py = iter->getStaticY() + 12;
        
        
        dot = ( ((px-x1)*(x2-x1)) + ((py-y1)*(y2-y1)) ) / std::pow(len,2);
        closestX = x1 + (dot * (x2-x1));
        closestY = y1 + (dot * (y2-y1));
        
        onSegment = linePoint(x1, y1, x2, y2, closestX, closestY);
        
        dist = std::sqrt(std::pow(px - closestX,2) + std::pow(py - closestY,2));
        
        
        if(onSegment && (dist < 6.0)) {
            //erase the unit
            
            printf("collision with unit %f\n", dist);
            
            unitsToErase.push_back(index);
            
            
            oneCollision = 1;
        }
        
        index++;
        
    }
    //sort unitsToErase vector by largest first
    std::sort(unitsToErase.rbegin(), unitsToErase.rend());
    
    
    
    for(int i : unitsToErase) {
        
        printf("%d\n", i);
        
        
    }
    
    for(int i : unitsToErase) {
        this->_units.erase(this->_units.begin() + i);
        this->_numUnits--;
    }

    /*
    //this is bad
    for(int i : unitsToErase) {

        printf("hello\n");
        index = 0;
        std::vector< std::shared_ptr<Unit> >::iterator it = this->_units.begin();
        while(it != this->_units.end()) {
            
            
            if(index == i) {
                
                
                
                if(it != this->_units.end()) {
                    it = this->_units.erase(it);
                }
                else {
                    it++;
                }
                
                
                
                this->_numUnits--;
                
                
            }
            
            printf("%d\n", index);
            
            index++;
            
        }
            
            
            
            
            
    }
    */
        /*
        for(std::vector< std::shared_ptr<Unit> >::iterator it = this->_units.begin(); it != this->_units.end(); ++it) {
            
            if(index == i) {
                
                
                
                if(it != this->_units.end()) {
                    it = this->_units.erase(it);
                }
                else {
                    this->_units.erase(it);
                }
                
                
                
                this->_numUnits--;
                

            }
            
            printf("%d\n", index);
            
            index++;
            
        }

        
    }
    */
    
    

    if(oneCollision) {
        return Vector2(closestClosestX, closestClosestY);
    }

    return Vector2(-1, -1);
}

// LINE/POINT
bool Fireteam::linePoint(double x1, double y1, double x2, double y2, double px, double py) {
    
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

double Fireteam::dist(double x1, double y1, double x2, double y2) {
    
    double distX = x1 - x2;
    double distY = y1 - y2;
    
    return std::sqrt( (distX*distX) + (distY*distY) );
    
}

int Fireteam::getSize() {
    return this->_numUnits;
}
