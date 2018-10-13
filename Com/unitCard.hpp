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
    UnitCard(Rank rank, Role role, std::vector<int> post, std::map<std::vector<int>, int> mapToNumChildren, Graphics &graphics);
    ~UnitCard();
    void update(int mx, int my, int offsetX, int offsetY);
    
    void draw(Graphics &graphics);
    void draw(Graphics &graphics, int x, int y);
    void drawTmp(Graphics &graphics);

    void changeY(int newY);
    void changeX(int newX);
    
    int rankToPNGLocation(Rank rank);
    
    Rectangle getRectangle();
    
    void drawTmp();
    
    std::vector<int> getPost();
    
    void changePost(std::vector<int> newPost, std::map<std::vector<int>, int> mapToNumChildren);
    
    void updatePostOldFireteam(int removedUnitCardFireteamPosition, std::map<std::vector<int>, int> mapToNumChildren);
    
    void updatePostNewFireteam(std::map<std::vector<int>, int> mapToNumChildren);
    
    void updateRectangle();
    
    int getX();
    int getY();
    
private:
    
    double _x, _y;
    double _tmpX, _tmpY;
    int _width, _height;
    
    Sprite _rank;
    Sprite _role;
    
    Rectangle _rectangle;
    
    std::vector<int> _post;
    
};


#endif /* unitCard_hpp */
