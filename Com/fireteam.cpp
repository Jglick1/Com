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

Fireteam::Fireteam(Graphics &graphics, bool allied) :
_numUnits(0)
{
    this->_controlSlide = ControlSlide(graphics, Vector2(100, 100));
    
    
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
    
    //for (Unit * iter : this->_units) {
        //iter->draw(graphics);
    //}
    
    //draw control slide
    
    for (std::shared_ptr<Unit>& iter : this->_units) {
        iter->draw(graphics);
        //printf("x: %f, y: %f\n", iter->getStaticX(), iter->getStaticY());
        
    }
    
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

bool Fireteam::isCenterSelected() {
    return this->_controlSlide.isCenterSelected();
}
