//
//  unit.cpp
//  Com
//
//  Created by Jonah Glick on 6/23/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#include "unit.hpp"
#include "graphics.hpp"
#include <iostream>
#include <math.h>


namespace player_constants {
    const float WALK_SPEED = 0.2f;
}

Unit::Unit() {}

Unit::Unit(Graphics &graphics, Vector2 spawnPoint) :
Sprite(graphics, "/Users/jonahglick/Documents/Com/rifelman_final_96x96.png", 0, 0, 96, 96, 600, 600),
_dx(0),
_dy(0),
_mx(0),
_my(0)
{
    //graphics.loadImage("/Users/jonahglick/Documents/Com/rifelman_final_96x96.png");
    
    
    this->_fov = Sprite(graphics, "/Users/jonahglick/Documents/Com/view_direction.png", 0, 0, 96, 128, 600, 600);

}
