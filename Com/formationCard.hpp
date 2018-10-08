//
//  unitCounter.hpp
//  Com
//
//  Created by Jonah Glick on 10/7/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef unitCounter_hpp
#define unitCounter_hpp

#include "sprite.hpp"
#include "globals.hpp"
#include "rectangle.hpp"
#include "graphics.hpp"
#include "unitCard.hpp"

class Graphics;


class FormationCard {
public:
    FormationCard();
    FormationCard(Formation formation, Graphics &graphics);
    ~FormationCard();
    void update(int mx, int my, int offsetX, int offsetY);
    void draw(Graphics &graphics);
    void draw(Graphics &graphics, int x, int y);
    
    void changeY(int newY, int newCollisionY);
    void changeX(int newX, int newCollisionX);
    
    int formationToPNGLocation(Formation formation);
    
    Rectangle getRectangle();
    
    void addSubUnit(UnitCard &subUnit);
    void addSubFormation(FormationCard &subCounter);
    
private:
    
    double _x, _y;
    int _width, _height;
    
    Sprite _template;
    Sprite _formation;
    
    Rectangle _rectangle;
    
    //children
    std::vector<FormationCard> _subFormations;
    std::vector<UnitCard> _subUnits;
    
    int _numberOfSubUnits, _numberOfSubFormations;
    
    
};

#endif /* unitCounter_hpp */
