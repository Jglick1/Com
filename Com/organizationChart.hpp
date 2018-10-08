//
//  organizationChart.hpp
//  Com
//
//  Created by Jonah Glick on 10/6/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef organizationChart_hpp
#define organizationChart_hpp

#include <vector>
#include "rectangle.hpp"
#include "sprite.hpp"
#include "unitCard.hpp"
#include "formationCard.hpp"
#include "graphics.hpp"
#include "plusSign.hpp"

class Graphics;


class OrganizationChart {
public:
    OrganizationChart();
    OrganizationChart(Graphics &graphics);
    ~OrganizationChart();
    
    void update(int mx, int my);
    void draw(Graphics &graphics);
    
    //std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
    
    
    //void changeY(int newY, int newCollisionY);
    //void changeX(int newX, int newCollisionX);
    
    
    //the tree structure should be here
    
    void handleMouseCollision(Graphics &graphics, int mx, int my);
    void isNotSelected();
    
private:
    
    std::vector<UnitCard> _cards;
    
    Sprite _background;
    
    PlusSign _plusSign;
    
    int _selectedUnitCardIndex;
    bool _isSelected;
    
    FormationCard _formationCard;
    
    int _mouseOffsetX, _mouseOffsetY;
    
    
};


#endif /* organizationChart_hpp */
