//
//  unitCounter.cpp
//  Com
//
//  Created by Jonah Glick on 10/7/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "formationCard.hpp"

FormationCard::FormationCard() {}

FormationCard::~FormationCard() {}

FormationCard::FormationCard(Formation formation, Graphics &graphics) :
_width(64),
_height(64),
_x(620.0),
_y(100.0),
_numberOfSubUnits(0),
_numberOfSubFormations(0)
{
    
    int formationPNGLocation = formationToPNGLocation(formation);
    //printf("rank: %d\n", rankPNGLocation);
    
    //this->_rank = Sprite(graphics, "/Users/jonahglick/Documents/Com/aranks.png", rankPNGLocation, 0, 32, 64, 0, 0);
    
    //this->_role = Sprite(graphics, "/Users/jonahglick/Documents/Com/rifleman.png", 0, 0, 16, 20, 0, 0);
    
    this->_template = Sprite(graphics, "/Users/jonahglick/Documents/Com/template_germany.png", 0, 0, 64, 64, 0, 0);
    
    this->_rectangle = Rectangle(this->_x, this->_y, 64, 64);
    
}

void FormationCard::addSubUnit(UnitCard &subUnit) {
    
    this->_subUnits.push_back(subUnit);
    

    this->_numberOfSubUnits++;
}

void FormationCard::addSubFormation(FormationCard &subFormationCard) {
    
    
    this->_subFormations.push_back(subFormationCard);
    
    
    this->_numberOfSubFormations++;
}

void FormationCard::draw(Graphics &graphics) {
    
    this->_template.draw(graphics, this->_x, this->_y);
    //this->_formation.draw(graphics, this->_x + 16, this->_y);
    
    graphics.drawRect(std::round(this->_rectangle.getX()), std::round(this->_rectangle.getY()), this->_rectangle.getWidth(), this->_rectangle.getHeight());
    
    
    if(this->_numberOfSubFormations > 1) {
        //then draw lines
        int startx = this->_x + this->_width/2;
        int starty = this->_y + this->_height;
        graphics.drawLine(startx, starty, startx, starty+20);
        
        int length = (this->_numberOfSubFormations-1) * 64;
        
        //int length = (5-1) * 64;
        
        
        graphics.drawLine(startx - length/2, starty+20, startx + length/2, starty+20);
        
        for(int i = 0; i < this->_numberOfSubFormations; i++) {
        //for(int i = 0; i < 5; i++) {
            graphics.drawLine(startx - length/2 + i*(64), starty+20, startx - length/2 + i*(64), starty+40);
        }
        
        
        //then draw subcounters
        for(int i = 0; i < this->_subFormations.size(); i++) {
            this->_subFormations.at(i).draw(graphics, startx - (this->_width*(this->_numberOfSubFormations/2.0)) + i*(64), starty+40);
        }
        
        /*
        for(UnitCounter unitCounter : this->_subCounter) {
            unitCounter.draw(graphics, 500, 500);
        }
        */
        
    }
    else if(this->_numberOfSubFormations == 1) {
        int startx = this->_x + this->_width/2;
        int starty = this->_y + this->_height;
        graphics.drawLine(startx, starty, startx, starty+40);
        
        this->_subFormations.at(0).draw(graphics, startx - this->_width/2, starty+40);
        
    }
    
    
}

void FormationCard::draw(Graphics &graphics, int x, int y) {
    
    this->_template.draw(graphics, x, y);
    //this->_formation.draw(graphics, this->_x + 16, this->_y);
    
    graphics.drawRect(x, y, this->_rectangle.getWidth(), this->_rectangle.getHeight());
    
    
}


Rectangle FormationCard::getRectangle() {
    return this->_rectangle;
}

void FormationCard::update(int mx, int my, int offsetX, int offsetY) {
    
    //this->_x = mx + offsetX;
    //this->_y = my + offsetY;
    
    //this->_rectangle.changeX(mx + offsetX);
    //this->_rectangle.changeY(my + offsetY);
    
}


int FormationCard::formationToPNGLocation(Formation formation) {
    
    int result = -1;
    
    switch(formation) {
        case FIRETEAM:
            result = 0;
            break;
        case SQUAD:
            result = 16*2;
            break;
        case PLATOON:
            result = 32*2;
            break;
    }
    
    return result;
}
