//
//  unitCard.hpp
//  Com
//
//  Created by Jonah Glick on 10/6/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef unitCard_hpp
#define unitCard_hpp

#include "sprite.hpp"
#include "globals.hpp"
#include "rectangle.hpp"
#include "graphics.hpp"

class Graphics;


class UnitCard {
public:
    UnitCard();
    UnitCard(Rank rank, Role role, Graphics &graphics);
    ~UnitCard();
    void update(int mx, int my, int offsetX, int offsetY);
    void draw(Graphics &graphics);

    void changeY(int newY, int newCollisionY);
    void changeX(int newX, int newCollisionX);
    
    int rankToPNGLocation(Rank rank);
    
    Rectangle getRectangle();
    
private:
    
    double _x, _y;
    int _width, _height;
    
    Sprite _rank;
    Sprite _role;
    
    Rectangle _rectangle;
    
};


#endif /* unitCard_hpp */
