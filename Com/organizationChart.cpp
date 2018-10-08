//
//  organizationChart.cpp
//  Com
//
//  Created by Jonah Glick on 10/6/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "organizationChart.hpp"
//#include "globals.hpp"

OrganizationChart::OrganizationChart() {}
OrganizationChart::~OrganizationChart() {}

OrganizationChart::OrganizationChart(Graphics &graphics) :
_isSelected(0),
_mouseOffsetX(0),
_mouseOffsetY(0)
{
    
    this->_background = Sprite(graphics, "/Users/jonahglick/Documents/Com/organizationChart.png", 0, 0, 1280, 800, 0, 0);
    
    this->_cards.push_back(UnitCard(FSGT, RIFLEMAN, graphics));
    
    this->_formationCard = FormationCard(SQUAD, graphics);
    
    this->_plusSign = PlusSign(graphics);
    
    //UnitCounter tmp(FIRETEAM, graphics);
    //this->_unitCounter.addSubCounter(tmp);
    
    //UnitCounter tmp1(FIRETEAM, graphics);
    //this->_unitCounter.addSubCounter(tmp1);
    
}

void OrganizationChart::draw(Graphics &graphics) {
    
    this->_background.draw(graphics, 0, 0);
    
    this->_formationCard.draw(graphics);
    
    for(UnitCard card : this->_cards) {
        card.draw(graphics);
    }
    
    this->_plusSign.draw(graphics);

}

void OrganizationChart::isNotSelected() {
    this->_isSelected = 0;
}

void OrganizationChart::handleMouseCollision(Graphics &graphics, int mx, int my) {
    
    Rectangle tmp;
    //check cards
    for(int i = 0; i < this->_cards.size(); i++) {

        tmp = this->_cards.at(i).getRectangle();
        
        if(mx > tmp.getX() && mx < tmp.getX() + tmp.getWidth()) {
            if(my > tmp.getY() && my < tmp.getY() + tmp.getHeight()) {
                //collision!
                printf("collision!\n");
                
                //store the offset
                this->_isSelected = 1;
                this->_selectedUnitCardIndex = i;
                
                
                this->_mouseOffsetX = -mx + tmp.getX();
                this->_mouseOffsetY = -my + tmp.getY();
                
                
                
                return;
                
            }
        }

    }
    
    //check plus sign
    
    tmp = this->_plusSign.getRectangle();

    if(mx > tmp.getX() && mx < tmp.getX() + tmp.getWidth()) {
        if(my > tmp.getY() && my < tmp.getY() + tmp.getHeight()) {

            FormationCard tmp(FIRETEAM, graphics);
            this->_formationCard.addSubFormation(tmp);
            
            return;
            
        }
    }
    

}

void OrganizationChart::update(int mx, int my) {
    
    
    if(this->_isSelected) {
        this->_cards.at(this->_selectedUnitCardIndex).update(mx, my, this->_mouseOffsetX, this->_mouseOffsetY);
    }
    
    
}
