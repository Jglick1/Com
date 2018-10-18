//
//  fogOfWar.hpp
//  Com
//
//  Created by Jonah Glick on 10/17/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef fogOfWar_hpp
#define fogOfWar_hpp
#include "graphics.hpp"
#include "globals.hpp"

class FogOfWar {
    
public:
    FogOfWar();
    FogOfWar(Graphics &graphics);
    void update();
    void draw(Graphics &graphics);
    
    
private:
    //std::vector< std::vector<Vector2> > _triangles;
    std::vector<Vector2> _visibleCorners;
    
    
    
};

#endif /* fogOfWar_hpp */
