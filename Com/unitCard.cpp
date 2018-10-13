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

UnitCard::UnitCard(Rank rank, Role role, std::vector<int> post, std::map<std::vector<int>, int> mapToNumChildren, Graphics &graphics) :
_width(32),
_height(32),
_x(620.0),
_y(100.0),
_tmpX(0.0),
_tmpY(0.0)
{
    
    int rankPNGLocation = rankToPNGLocation(rank);
    printf("rank: %d\n", rankPNGLocation);
    
    this->_rank = Sprite(graphics, "/Users/jonahglick/Documents/Com/aranks.png", rankPNGLocation, 0, 16, 32, 0, 0);

    this->_role = Sprite(graphics, "/Users/jonahglick/Documents/Com/rifleman.png", 0, 0, 16, 20, 0, 0);
    
    
    
    this->_post = post;
    
    //find the position of the unit card
    std::vector<int> tmp = this->_post;
    
    int numFireteams = mapToNumChildren[std::vector<int> {0}];
    
    int numUnitsInFireteam = mapToNumChildren[std::vector<int> {0, this->_post[1]}];
    
    if(tmp.size() == 3) {
        this->_y += 64 + 20 + 32 + 20;
        this->_x += 112 * tmp.at(1);
        this->_x += 32 * tmp.at(2);
        
        this->_x -= (56 * (numFireteams-1));
        this->_x += 16; //for firteams
        
        printf("numUnits: %d\n", numUnitsInFireteam);
        
        if(numUnitsInFireteam > 1) {
            this->_x -= 16 * (numUnitsInFireteam-1);
            //this->_x += 0*this->_post[2];
            //printf("post: %d\n",this->_post[2]);
        }
        
    }
    

    
    
    //this->_y += 20;
    /*
    for(int i = 1; i < tmp.size(); i++) { //skip the first one
        printf("%d\n", tmp.at(i));
        this->_x += 100 * tmp.at(i);
        this->_y += 20 + 64;
    }
    */
    
    
    
    
    this->_rectangle = Rectangle(this->_x, this->_y, 32, 32);
    
    
}



void UnitCard::draw(Graphics &graphics) {
    
    this->_rank.draw(graphics, this->_x, this->_y);
    this->_role.draw(graphics, this->_x + 16, this->_y - 4 + 8);
    
    graphics.drawRect(std::round(this->_rectangle.getX()), std::round(this->_rectangle.getY()), this->_rectangle.getWidth(), this->_rectangle.getHeight());

    
}

void UnitCard::draw(Graphics &graphics, int x, int y) {
    this->_rank.draw(graphics, x, y);
    this->_role.draw(graphics, x + 16, y - 4 + 8);
    
    graphics.drawRect(x, y, this->_rectangle.getWidth(), this->_rectangle.getHeight());
    
    
}



void UnitCard::drawTmp(Graphics &graphics) {
    this->_rank.draw(graphics, this->_tmpX, this->_tmpY);
    this->_role.draw(graphics, this->_tmpX + 16, this->_tmpY - 4 + 8);
    
    graphics.drawRect(this->_tmpX, this->_tmpY, this->_rectangle.getWidth(), this->_rectangle.getHeight());
    
}


void UnitCard::changeY(int newY) {
    this->_y = newY;
    this->_rectangle.changeY(newY);
}


void UnitCard::changeX(int newX) {
    this->_x = newX;
    this->_rectangle.changeX(newX);
}


Rectangle UnitCard::getRectangle() {
    return this->_rectangle;
}

void UnitCard::update(int mx, int my, int offsetX, int offsetY) {
    
    //this->_x = mx + offsetX;
    //this->_y = my + offsetY;
    
    this->_tmpX = mx + offsetX;
    this->_tmpY = my + offsetY;
    
    //this->_rectangle.changeX(mx + offsetX);
    //this->_rectangle.changeY(my + offsetY);
    
}


int UnitCard::rankToPNGLocation(Rank rank) {
    
    int result = -1;

    switch(rank) {
        case FSGT:
            result = 0;
            break;
        case MSGT:
            result = 16;
            break;
        case TSGT:
            result = 32;
            break;
        case SSGT:
            result = 48;
            break;
        case SGT:
            result = 80;
            break;
        case CPL:
            result = 112;
            break;
        case PFC:
            result = 144;
            break;
    }
    
    return result;
}

std::vector<int> UnitCard::getPost() {
    return this->_post;
}

void UnitCard::changePost(std::vector<int> newPost, std::map<std::vector<int>, int> mapToNumChildren){
    this->_post = newPost;
    
    
    int numFireteams = mapToNumChildren[std::vector<int> {0}];
    
    int numUnitsInFireteam = mapToNumChildren[std::vector<int> {0, this->_post[1]}];
    
    this->_x = 620;
    this->_y = 100;
    
    //change static position
    if(newPost.size() == 3) {
        this->_y += 64 + 20 + 32 + 20;
        this->_x += 112 * newPost.at(1);
        this->_x += 32 * newPost.at(2);
        
        this->_x -= (56 * (numFireteams-1));
        this->_x += 16; //for firteams
        
        //printf("numUnits: %d\n", numUnitsInFireteam);
        
        if(numUnitsInFireteam > 1) {
            this->_x -= 16 * (numUnitsInFireteam-1);
            //this->_x += 0*this->_post[2];
            //printf("post: %d\n",this->_post[2]);
        }
        
    }
    
    updateRectangle();
    
    
    
}

void UnitCard::updatePostOldFireteam(int removedUnitCardFireteamPosition, std::map<std::vector<int>, int> mapToNumChildren) {   //assuming this->_post is size 3
    //move position in fireteam down by 1
    if(this->_post[2] > removedUnitCardFireteamPosition) {
        this->_post[2] -= 1;
    }
    
    int numFireteams = mapToNumChildren[std::vector<int> {0}];
    
    int numUnitsInFireteam = mapToNumChildren[std::vector<int> {0, this->_post[1]}];
    
    //printf("updated NumUnits: %d\n", numUnitsInFireteam);
    
    this->_x = 620;
    this->_y = 100;
    
    //update static position
    if(this->_post.size() == 3) {
        this->_y += 64 + 20 + 32 + 20;
        this->_x += 112 * this->_post.at(1);
        this->_x += 32 * this->_post.at(2);
        
        this->_x -= (56 * (numFireteams-1));
        this->_x += 16; //for firteams
        
        //printf("numUnits: %d\n", numUnitsInFireteam);
        
        if(numUnitsInFireteam > 1) {
            this->_x -= 16 * (numUnitsInFireteam-1);
            //this->_x += 0*this->_post[2];
            //printf("post: %d\n",this->_post[2]);
        }
        
    }
    
    updateRectangle();
    
}

void UnitCard::updatePostNewFireteam(std::map<std::vector<int>, int> mapToNumChildren) {
    
    int numFireteams = mapToNumChildren[std::vector<int> {0}];
    
    int numUnitsInFireteam = mapToNumChildren[std::vector<int> {0, this->_post[1]}];
    
    //printf("updated NumUnits: %d\n", numUnitsInFireteam);
    
    this->_x = 620;
    this->_y = 100;
    
    //update static position
    if(this->_post.size() == 3) {
        this->_y += 64 + 20 + 32 + 20;
        this->_x += 112 * this->_post.at(1);
        this->_x += 32 * this->_post.at(2);
        
        this->_x -= (56 * (numFireteams-1));
        this->_x += 16; //for firteams
        
        //printf("numUnits: %d\n", numUnitsInFireteam);
        
        if(numUnitsInFireteam > 1) {
            this->_x -= 16 * (numUnitsInFireteam-1);
            //this->_x += 0*this->_post[2];
            //printf("post: %d\n",this->_post[2]);
        }
        
    }
    
    updateRectangle();
}

void UnitCard::updateRectangle() {
    this->_rectangle.changeX(this->_x);
    this->_rectangle.changeY(this->_y);
}


int UnitCard::getX() {
    return this->_x;
}


int UnitCard::getY() {
    return this->_y;
}
