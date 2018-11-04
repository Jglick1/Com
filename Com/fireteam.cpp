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

Fireteam::Fireteam(Graphics &graphics, bool allied) {
    this->_controlSlide = ControlSlide(graphics);
    
    
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
    }
    
    
}



void Fireteam::update(int elapsedTime, Graphics &graphics) {
    //update all units
    for (std::shared_ptr<Unit>& iter : this->_units) {
        iter->update(elapsedTime, graphics);
    }
    
    
    //update controlSlide
    
    
    
}

/*
void Fireteam::addUnit(Unit *unit) {
    this->_units.push_back(unit);
    
    this->_numUnits++;
    
    
    
}
*/


void Fireteam::addUnit(std::shared_ptr<Unit> unit) {
    this->_units.push_back(std::move(unit));
    //this->_units.push_back(unit);
    
    this->_numUnits++;
    
    
}
