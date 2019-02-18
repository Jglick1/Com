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

FormationCard::FormationCard(Formation formation, std::vector<int> post, std::map< std::vector<int>, int> mapToNumChildren, Graphics &graphics) :
_x(620.0),
_y(100.0),
_numberOfSubUnits(0),
_numberOfSubFormations(0)
{
    
    this->_post = post;
    /*
    for (int i: this->_post) {
        printf("%d, ",i);
    }
    printf("\n");
    */
    
    //calculate x and y position with post vector
    if(this->_post.size() == 1) { //then it is squad
        //keep x and y the same
    }
    else { //we need to know how many children exist            //this is wrong
        //lets assume we know
        std::vector<int> tmp = this->_post;
        //this->_y += 20;
        
        int numFireteams = mapToNumChildren[std::vector<int> {0}];
        
        if(tmp.size() == 2) {
            this->_y += 64 + 20;
            this->_x += 112 * tmp.at(1); //32+32+16+32
            this->_x -= (56 * (numFireteams-1));
            this->_x += 16; //for firteams
        }
        
        
        
        /*
        for(int i = 1; i < tmp.size(); i++) { //skip the first one
            //printf("%d\n", tmp.at(i));
            this->_x += 100 * tmp.at(i);
        }
        */
    }
    
    
    int formationPNGLocation = formationToPNGLocation(formation);
    //printf("rank: %d\n", rankPNGLocation);
    
    //this->_rank = Sprite(graphics, "/Users/jonahglick/Documents/Com/aranks.png", rankPNGLocation, 0, 32, 64, 0, 0);
    
    //this->_role = Sprite(graphics, "/Users/jonahglick/Documents/Com/rifleman.png", 0, 0, 16, 20, 0, 0);
    
    if(formation != FIRETEAM) {
        this->_template = Sprite(graphics, "/Users/jonahglick/Documents/Com/template_germany.png", 0, 0, 64, 64, 0, 0);
        this->_rectangle = Rectangle(this->_x, this->_y, 64, 64);
        this->_width = 64;
        this->_height = 64;
    }
    else {
        this->_template = Sprite(graphics, "/Users/jonahglick/Documents/Com/template_fireteam_germany.png", 0, 0, 32, 32, 0, 0);
        this->_rectangle = Rectangle(this->_x, this->_y, 32, 32);
        this->_width = 32;
        this->_height = 32;
    }
    
    
}

void FormationCard::addSubUnit(UnitCard &subUnit) {
    
    this->_subUnits.push_back(subUnit);
    

    this->_numberOfSubUnits++;
}



void FormationCard::addSubUnit(UnitCard &subUnit, std::vector<int> &position) {
    
    
    if(position.size() > 0) {
        position.pop_back();
        this->_subFormations.at(position[0]).addSubUnit(subUnit, position);
    }
    else {
        this->_subUnits.push_back(subUnit);
    }
    
    
    this->_numberOfSubUnits++;
    
}



void FormationCard::addSubFormation(FormationCard &subFormationCard) {
    
    
    this->_subFormations.push_back(subFormationCard);
    
    
    this->_numberOfSubFormations++;
}

void FormationCard::draw(Graphics &graphics, int x, int y) {
    this->_template.draw(graphics, this->_x + x, this->_y + y);
    graphics.drawRect(std::round(this->_rectangle.getX() + x), std::round(this->_rectangle.getY() + y), this->_rectangle.getWidth(), this->_rectangle.getHeight());
}

void FormationCard::draw(Graphics &graphics) {
    
    this->_template.draw(graphics, this->_x, this->_y);
    //this->_formation.draw(graphics, this->_x + 16, this->_y);
    
    graphics.drawRect(std::round(this->_rectangle.getX()), std::round(this->_rectangle.getY()), this->_rectangle.getWidth(), this->_rectangle.getHeight());
    
    
    
    /*
    if(this->_numberOfSubFormations > 1) {
        //draw first line
        int startx = this->_x + this->_width/2;
        int starty = this->_y + this->_height;
        graphics.drawLine(startx, starty, startx, starty+10);
        
        //for sub fireteams
        int length = (this->_numberOfSubFormations-1) * (64+48);
        

        //draw horizontal line
        graphics.drawLine(startx - length/2, starty+10, startx + length/2, starty+10);
        
        //draw smaller verticle lines
        for(int i = 0; i < this->_numberOfSubFormations; i++) {
            graphics.drawLine(startx - length/2 + i*(64+48), starty+10, startx - length/2 + i*(64+48), starty+20);
        }

        //then draw subcounters recursivly
        for(int i = 0; i < this->_subFormations.size(); i++) {
            this->_subFormations.at(i).draw(graphics, startx - (this->_width*(this->_numberOfSubFormations)) + i*(64+48) + (this->_numberOfSubFormations-1)*8 + 48 , starty+20);
        }
        
    }
    else if(this->_numberOfSubFormations == 1) {
        int startx = this->_x + this->_width/2;
        int starty = this->_y + this->_height;
        graphics.drawLine(startx, starty, startx, starty+20);
        
        this->_subFormations.at(0).draw(graphics, startx - this->_width/2, starty+20);
        
    }
    */
    
    
    
    
    
    
    /*
    else if(this->_numberOfSubUnits > 1) {
        //then draw lines
        int startx = this->_x + this->_width/2;
        int starty = this->_y + this->_height;
        graphics.drawLine(startx, starty, startx, starty+20);
        
        int length = (this->_numberOfSubFormations-1) * 64 * 2;
        
        graphics.drawLine(startx - length/2, starty+20, startx + length/2, starty+20);
        
        for(int i = 0; i < this->_numberOfSubFormations; i++) {
            graphics.drawLine(startx - length/2 + i*(64*2), starty+20, startx - length/2 + i*(64*2), starty+40);
        }
        
        
        //then draw subcounters
        for(int i = 0; i < this->_subFormations.size(); i++) {
            this->_subUnits.at(i).draw(graphics, startx - (this->_width*(this->_numberOfSubUnits)) + i*(64*2)+32 , starty+40);
        }
    }
    else if(this->_numberOfSubUnits == 1) {
        int startx = this->_x + this->_width/2;
        int starty = this->_y + this->_height;
        graphics.drawLine(startx, starty, startx, starty+40);
        
        this->_subUnits.at(0).draw(graphics, startx - this->_width/2, starty+40);
        
    }
    */
}


/*
void FormationCard::draw(Graphics &graphics, int x, int y) {
    
    this->_template.draw(graphics, x, y);
    //this->_formation.draw(graphics, this->_x + 16, this->_y);
    
    graphics.drawRect(x, y, this->_rectangle.getWidth(), this->_rectangle.getHeight());
    
    
    if(this->_numberOfSubFormations > 1) {
        //then draw lines
        int startx = x + this->_width/2;
        int starty = y + this->_height;
        graphics.drawLine(startx, starty, startx, starty+20);
        
        int length = (this->_numberOfSubFormations-1) * 64 * 2;

        graphics.drawLine(startx - length/2, starty+20, startx + length/2, starty+20);
        
        for(int i = 0; i < this->_numberOfSubFormations; i++) {
            //for(int i = 0; i < 5; i++) {
            graphics.drawLine(startx - length/2 + i*(64*2), starty+20, startx - length/2 + i*(64*2), starty+40);
        }
        
        
        //then draw subcounters
        for(int i = 0; i < this->_subFormations.size(); i++) {
            this->_subFormations.at(i).draw(graphics, startx - (this->_width*(this->_numberOfSubFormations)) + i*(64*2)+32 , starty+40);
        }
        
    }
    else if(this->_numberOfSubFormations == 1) {
        int startx = x + this->_width/2;
        int starty = y + this->_height;
        graphics.drawLine(startx, starty, startx, starty+40);
        
        this->_subFormations.at(0).draw(graphics, startx - this->_width/2, starty+40);
        
    }
    
    else if(this->_numberOfSubUnits > 1) {
        //then draw lines
        int startx = x + this->_width/2;
        int starty = y + this->_height;
        graphics.drawLine(startx, starty, startx, starty+10);
        
        int length = (this->_numberOfSubUnits-1) * 32;
        
        graphics.drawLine(startx - length/2, starty+10, startx + length/2, starty+10);
        
        for(int i = 0; i < this->_numberOfSubUnits; i++) {
            graphics.drawLine(startx - length/2 + i*(32), starty+10, startx - length/2 + i*(32), starty+20);
        }
        

    }
    else if(this->_numberOfSubUnits == 1) {
        int startx = x + this->_width/2;
        int starty = y + this->_height;
        graphics.drawLine(startx, starty, startx, starty+20);
        
        //this->_subUnits.at(0).draw(graphics, startx, starty+40);
        
    }
    
    
    
}
*/

Rectangle FormationCard::getRectangle() {
    return this->_rectangle;
}

void FormationCard::update(int mx, int my, int offsetX, int offsetY) {
    
    //this->_x = mx + offsetX;
    //this->_y = my + offsetY;
    
    //this->_rectangle.changeX(mx + offsetX);
    //this->_rectangle.changeY(my + offsetY);
    
    //change the location of the sub units
    
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

FormationCard FormationCard::getSubFormation(int pos) {
    
    return this->_subFormations.at(pos);
    
}

int FormationCard::getSubUnitNumber() {
    return this->_numberOfSubUnits;
}

int FormationCard::getSubFormationNumber() {
    return this->_numberOfSubFormations;
}

std::vector<int> FormationCard::getPost() {
    return this->_post;
}
