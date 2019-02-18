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
#include "fireteam.hpp"

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
    void handleMouseLiftCollision(Graphics &graphics, int mx, int my);
    
    bool isSelected();
    void setToNotSelected();
    
    int getNumberOfChildren(std::vector<int> path);
    
    void handleMouseHover(int xm, int ym, Graphics &graphics);
    
    void cameraDragging();
    void cameraStopDragging();
    
    
    bool _drawHover = 1;
    Vector2 _hoverShadowPosition;
    
    void handleCameraMove(int xm, int ym);

    void readUnitInformation(Fireteam f1, Fireteam f2, Fireteam f3, Graphics &graphics);
    
    
private:
    
    std::vector<UnitCard> _Unitcards;
    std::vector<FormationCard> _formationCards;
    
    //std::vector<UnitCard *> _collisionCards;
    
    Sprite _background;
    
    PlusSign _plusSign;
    
    Sprite _hoverShadow;
    
    Sprite _selectShadow;
    
    int _selectedUnitCardIndex;
    bool _isSelected;
    
    FormationCard _formationCard;
    
    int _mouseOffsetX, _mouseOffsetY;
    
    std::map<int, std::vector<int> > _unitToPost;
    
    //map from collision rec (of formation card) to vector<int> of how to get there {0, 2, 3} like
    
    std::map< std::vector<int>, int> _mapToNumChildren;
    
    int _cameraX;
    int _cameraY;
    
    bool _cameraDrag;
    
    int _mx, _my;
    
};


#endif /* organizationChart_hpp */
