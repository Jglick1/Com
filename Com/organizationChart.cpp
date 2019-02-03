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
_mouseOffsetY(0),
_cameraX(0.0),
_cameraY(0.0)
{
    
    this->_background = Sprite(graphics, "/Users/jonahglick/Documents/Com/organizationChart.png", 0, 0, 1280, 800, 0, 0);
    
    this->_hoverShadow = Sprite(graphics, "/Users/jonahglick/Documents/Com/unitCardSelect.png", 0, 0, 32, 32, 0, 0);
    
    //this->_cards.push_back(UnitCard(SSGT, RIFLEMAN, graphics));
    
    //this->_formationCard = FormationCard(SQUAD, graphics); //formation chart begins at y = 100
    
    //this->_plusSign = PlusSign(graphics);
    
    //std::map< std::vector<int>, int> _mapToNumChildren; // map from position of formationcard to number of children the formation card has
    
    //this would be things you read in
    
    //read in children numbers
    std::vector<int> tmp = {0};
    this->_mapToNumChildren.insert(std::make_pair(tmp, 3)); //squad {0} has three fireteams
    
    tmp = {0, 0};
    this->_mapToNumChildren.insert(std::make_pair(tmp, 0));
    tmp = {0, 1};
    this->_mapToNumChildren.insert(std::make_pair(tmp, 3));
    tmp = {0, 2};
    this->_mapToNumChildren.insert(std::make_pair(tmp, 0));
    
    //produce cards
    tmp = {0};
    FormationCard testCard(SQUAD, tmp, this->_mapToNumChildren, graphics);
    this->_formationCards.push_back(testCard);
    
    tmp = {0, 0};
    FormationCard fireTeam1(FIRETEAM, tmp, this->_mapToNumChildren, graphics);
    this->_formationCards.push_back(fireTeam1);
    
    tmp = {0, 1};
    FormationCard fireTeam2(FIRETEAM, tmp, this->_mapToNumChildren, graphics);
    this->_formationCards.push_back(fireTeam2);
    
    tmp = {0, 2};
    FormationCard fireTeam3(FIRETEAM, tmp, this->_mapToNumChildren, graphics);
    this->_formationCards.push_back(fireTeam3);
    
    tmp = {0, 1, 0};
    UnitCard unit1(PFC, RIFLEMAN, tmp, this->_mapToNumChildren, graphics);
    this->_Unitcards.push_back(unit1);
    
    tmp = {0, 1, 1};
    UnitCard unit2(CPL, RIFLEMAN, tmp, this->_mapToNumChildren, graphics);
    this->_Unitcards.push_back(unit2);
    
    tmp = {0, 1, 2};
    UnitCard unit3(SGT, RIFLEMAN, tmp, this->_mapToNumChildren, graphics);
    this->_Unitcards.push_back(unit3);
    
    
    /*
    
    //adding formations
    FormationCard tmp(FIRETEAM, graphics);
    this->_formationCard.addSubFormation(tmp);
    
    FormationCard tmp1(FIRETEAM, graphics);
    this->_formationCard.addSubFormation(tmp1);
    
    FormationCard tmp2(FIRETEAM, graphics);
    this->_formationCard.addSubFormation(tmp2);
    
    
    //adding units                          ADD THESE GUYS TO this->_cards
    UnitCard unitTmp(PFC, RIFLEMAN, graphics);
    std::vector<int> position = {1}; //platoon, then first squad
    this->_formationCard.addSubUnit(unitTmp, position);
    
    UnitCard unitTmp1(CPL, RIFLEMAN, graphics);
    std::vector<int> position1 = {0}; //platoon, then first squad
    this->_formationCard.addSubUnit(unitTmp1, position1);
    
    
    this->_Unitcards.push_back(unitTmp);
    this->_unitToPost.insert(std::make_pair(0, std::vector<int> {1}));
    
    int subSquad = this->_formationCard.getSubFormationNumber();
    
    FormationCard formTemp = this->_formationCard.getSubFormation(1);
    int subFireTeam = formTemp.getSubUnitNumber();
    
    int xpos = 620+32; // center of squad

    if (subSquad > 1) {
        xpos -= (16+32+8)*(subSquad-1); // far right, just below the leftmost fireteam
        xpos += 1 * 112;    //find appropriate fireteam
        xpos -= 16;         //center the unitCard
    }
    
    this->_Unitcards.at(0).changeX(xpos);
    
    this->_Unitcards.at(0).changeY(100 + 64 + 20 + 32 + 20);

    
    
    this->_Unitcards.push_back(unitTmp1);
    this->_unitToPost.insert(std::make_pair(1, std::vector<int> {0}));
    
    */
    
    
    
    
    /*
    UnitCard unitTmp2(FSGT, RIFLEMAN, graphics);
    std::vector<int> position2 = {2}; //platoon, then first squad
    this->_formationCard.addSubUnit(unitTmp2, position2);
    */
    
    
    
    //TODO: handleClickUpCollision
    //find the collision of the formation icon, lookup in map the appropriate path, make a copy, and put the unit there
        //erase unit from previous position
    //v handleClickDownCollision, selecting the unit
    
}

void OrganizationChart::handleMouseHover(int mx, int my, Graphics &graphics) {
    //printf("organization\n");
    Rectangle tmp;
    for (UnitCard card : this->_Unitcards) {
        tmp = card.getRectangle();
        if(mx > tmp.getX() && mx < tmp.getX() + tmp.getWidth()) {
            if(my > tmp.getY() && my < tmp.getY() + tmp.getHeight()) {
                
                //this->_hoverShadow.draw(graphics, tmp.getX(), tmp.getY());
                
                this->_drawHover = 1;
                this->_hoverShadowPosition = Vector2(tmp.getX(), tmp.getY());
                return;
            }
        }
        
    }
    
    this->_drawHover = 0;
    
    
}

void OrganizationChart::draw(Graphics &graphics) {
    
    //draw the formation cards and the unit cards in position according to each one's vector<int> position
    
    
    
    this->_background.draw(graphics, 0, 0);
    
    //this->_formationCard.draw(graphics);
    
    
    
    //draw lines
    
    //squad to fireteam lines
    int numFireteams = this->_mapToNumChildren[std::vector<int> {0}];
    if(numFireteams == 1) {
        graphics.drawLine(620 + 32 + this->_cameraX, 100 + 64 + this->_cameraY, 620 + 32 + this->_cameraX, 100 + 64 + 20 + this->_cameraY);
    }
    else if (numFireteams > 1) {
        graphics.drawLine(620 + 32 + this->_cameraX, 100 + 64 + this->_cameraY, 620 + 32 + this->_cameraX, 100 + 64 + 10  + this->_cameraY); // verticle
        graphics.drawLine(620 + 32 - (56 * (numFireteams-1)) + this->_cameraX , 100 + 64 + 10 + this->_cameraY, 620 + 32 + (56 * (numFireteams-1)) + this->_cameraX, 100 + 64 + 10 + this->_cameraY); //horizontal
        for(int i = 0 ; i < numFireteams; i++) {
            graphics.drawLine(620 + 32 - (56 * (numFireteams-1)) + 112 * i + this->_cameraX, 100 + 64 + 10 + this->_cameraY, 620 + 32 - (56 * (numFireteams-1)) + 112 * i + this->_cameraX, 100 + 64 + 10 + 10 + this->_cameraY);
        }
    }
    
    //fireteam to unit lines
    for(int i = 0; i < numFireteams; i++) {
        
        int startX = 620 + 32 - (56 * (numFireteams-1)) + 112 * i + this->_cameraX;
        int startY = 100 + 64 + 10 + 10 + this->_cameraY;
        
        startY += 32;
        
        
        int numUnits = this->_mapToNumChildren[std::vector<int> {0, i}];
        
        if(numUnits == 1) {
            graphics.drawLine(startX, startY, startX, startY + 20);
        }
        else if (numUnits > 1) {
            graphics.drawLine(startX, startY, startX, startY + 10); // verticle
            graphics.drawLine(startX - (16 * (numUnits-1)), startY + 10, startX + (16 * (numUnits-1)), startY + 10); // 16 = 32/2
            for(int j = 0 ; j < numUnits; j++) {
                graphics.drawLine(startX - (16 * (numUnits-1)) + 32*j, startY + 10, startX - (16 * (numUnits-1))  + 32*j, startY + 10 + 10);
            }
        }
        
    }
    
    
    
    
    
    
    
    //draw formation cards
    for(FormationCard iter : this->_formationCards) {
        iter.draw(graphics);
    }
    
    
    
    //draw unit cards
    if(this->_isSelected) {     //fix this!
        for(int i = 0; i < this->_selectedUnitCardIndex; i++) {
            this->_Unitcards.at(i).draw(graphics);
        }
        
        this->_Unitcards.at(this->_selectedUnitCardIndex).drawTmp(graphics);
        
        for(int i = this->_selectedUnitCardIndex+1; i < this->_Unitcards.size(); i++) {
            this->_Unitcards.at(i).draw(graphics);
        }
    }
    else {
        for(UnitCard card : this->_Unitcards) {
            card.draw(graphics);
        }
    }
    
    
    
    /*
    for(UnitCard *card : this->_collisionCards) {   //this breaks the game
        card->draw(graphics);
    }
    */
    
    //this->_plusSign.draw(graphics);
    
    
    
    if(this->_drawHover) {
        this->_hoverShadow.draw(graphics, this->_hoverShadowPosition.x, this->_hoverShadowPosition.y);
    }
    

}

void OrganizationChart::setToNotSelected() {
    this->_isSelected = 0;
}

bool OrganizationChart::isSelected() {
    return this->_isSelected;
}

void OrganizationChart::handleMouseCollision(Graphics &graphics, int mx, int my) {
    
    Rectangle tmp;
    
    
    //check cards
    for(int i = 0; i < this->_Unitcards.size(); i++) {

        tmp = this->_Unitcards.at(i).getRectangle();
        
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
    /*
    tmp = this->_plusSign.getRectangle();

    if(mx > tmp.getX() && mx < tmp.getX() + tmp.getWidth()) {
        if(my > tmp.getY() && my < tmp.getY() + tmp.getHeight()) {

            FormationCard tmp(FIRETEAM, graphics);
            this->_formationCard.addSubFormation(tmp);
            
            return;
            
        }
    }
    */

}



void OrganizationChart::handleMouseLiftCollision(Graphics &graphics, int mx, int my) {
    //collision rects that are the formation counters
    printf("lift up\n");

    
    
    
    Rectangle tmp;
    
    
    //check formation cards
    for(int i = 0; i < this->_formationCards.size(); i++) {
        
        tmp = this->_formationCards.at(i).getRectangle();
        
        if(mx > tmp.getX() && mx < tmp.getX() + tmp.getWidth()) {
            if(my > tmp.getY() && my < tmp.getY() + tmp.getHeight()) {
                //collision!
                //printf("collision!\n");
                
                //make changes
                
                
                std::vector<int> formationPath = this->_formationCards.at(i).getPost();
                std::vector<int> unitPath = this->_Unitcards.at(this->_selectedUnitCardIndex).getPost(); //get the selected unit card's post
                
                //printf("collision with formation card\n");
                //for(int i : formationPath) {
                //    printf("%d ", i);
                //}
                //printf("\n");
                std::vector<int> temp3 = unitPath;
                temp3.pop_back();
                if(formationPath == temp3) {
                    //do nothing
                    //printf("do nothing\n");
                }
                else {
                    

                    
                    
                    std::vector<int> temp = unitPath;
                    temp.pop_back();
                    this->_mapToNumChildren[temp] -= 1; //reduce the number of children of former fireteam by 1
                    
                    //move post of all other units in fireteam above this one, down by 1
                    for(UnitCard & iter : this->_Unitcards) {
                        std::vector<int> temp1 = iter.getPost();
                        temp1.pop_back();
                        if(temp1 == temp) { //if it's in the old fireteam
                            //printf("update old fireteam\n");
                            std::vector<int> temp4 = this->_Unitcards.at(this->_selectedUnitCardIndex).getPost();
                            //printf("last index: %d\n", temp4.at(2));
                            iter.updatePostOldFireteam(temp4.at(2), this->_mapToNumChildren);
                        }
                    }
                    
                    //update units in old fireteam. Update units in new fireteam.
                    
                    
                    //update selected unit's post
                    int numberOfUnitsAlreadyPresent = this->_mapToNumChildren[formationPath];   //change unit's post
                    temp = formationPath;
                    temp.push_back(numberOfUnitsAlreadyPresent);
                    this->_Unitcards.at(this->_selectedUnitCardIndex).changePost(temp, this->_mapToNumChildren);
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    this->_mapToNumChildren[formationPath] += 1; //increase number of children of new fireteam by 1

                    //update all units in the new fireteam
                    
                    temp = formationPath;
                    
                    for(UnitCard & iter : this->_Unitcards) {
                        std::vector<int> temp1 = iter.getPost();
                        temp1.pop_back();
                        if(temp1 == temp) { //if it's in the old fireteam
                            iter.updatePostNewFireteam(this->_mapToNumChildren);
                        }
                    }
                    
                    
                    
                    
                    
                }
                
                /*
                printf("cycling through units...\n");
                std::vector<int> help;
                for(int i = 0 ; i < this->_Unitcards.size(); i++) {
                    help = this->_Unitcards.at(i).getPost();
                    printf("%d, ", help.at(2));
                }
                printf("\n");
                */
                
                
                return;
                
            }
        }
    }
    

    
    
}


void OrganizationChart::handleCameraMove(int xm, int ym) {
    
    if(this->_cameraDrag) {
        this->_cameraX = xm;
        this->_cameraY = ym;
    }

}


void OrganizationChart::update(int mx, int my) {
    
    
    if(this->_isSelected) {
        this->_Unitcards.at(this->_selectedUnitCardIndex).update(mx, my, this->_mouseOffsetX, this->_mouseOffsetY);
        //this->_collisionCards.at(this->_selectedUnitCardIndex)->update(mx, my, this->_mouseOffsetX, this->_mouseOffsetY);
    }
    
    
}



int OrganizationChart::getNumberOfChildren(std::vector<int> path) {
    
    FormationCard tmp = this->_formationCard.getSubFormation(path.at(0));
    return tmp.getSubUnitNumber();
    
    
}



void OrganizationChart::cameraDragging() {
    this->_cameraDrag = 1;
}

void OrganizationChart::cameraStopDragging() {
    this->_cameraDrag = 0;
}



