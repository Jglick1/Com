//
//  unitCard.cpp
//  Com
//
//  Created by Jonah Glick on 10/6/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "unitCard.hpp"

UnitCard::UnitCard() {}

UnitCard::~UnitCard() {}

UnitCard::UnitCard(Rank rank, Role role, Graphics &graphics) :
_width(32),
_height(32),
_x(0.0),
_y(0.0)

{
    
    int rankPNGLocation = rankToPNGLocation(rank);
    printf("rank: %d\n", rankPNGLocation);
    
    this->_rank = Sprite(graphics, "/Users/jonahglick/Documents/Com/aranks.png", rankPNGLocation, 0, 32, 64, 0, 0);

    this->_role = Sprite(graphics, "/Users/jonahglick/Documents/Com/rifleman.png", 0, 0, 16, 20, 0, 0);
    
    this->_rectangle = Rectangle(0, 0, 64, 64);
    
}



void UnitCard::draw(Graphics &graphics) {
    
    this->_rank.draw(graphics, this->_x, this->_y);
    this->_role.draw(graphics, this->_x + 32, this->_y + 24);
    
    graphics.drawRect(std::round(this->_rectangle.getX()), std::round(this->_rectangle.getY()), this->_rectangle.getWidth(), this->_rectangle.getHeight());

    
}

Rectangle UnitCard::getRectangle() {
    return this->_rectangle;
}

void UnitCard::update(int mx, int my, int offsetX, int offsetY) {
    
    this->_x = mx + offsetX;
    this->_y = my + offsetY;
    
    this->_rectangle.changeX(mx + offsetX);
    this->_rectangle.changeY(my + offsetY);
    
}


int UnitCard::rankToPNGLocation(Rank rank) {
    
    int result = -1;

    switch(rank) {
        case FSGT:
            result = 0;
            break;
        case MSGT:
            result = 16*2;
            break;
        case TSGT:
            result = 32*2;
            break;
        case SSGT:
            result = 48*2;
            break;
        case SGT:
            result = 80*2;
            break;
        case CPL:
            result = 132*2;
            break;
        case PFC:
            result = 144*2;
            break;
    }
    
    return result;
}
