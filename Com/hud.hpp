//
//  hud.hpp
//  Com
//
//  Created by Jonah Glick on 8/3/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef hud_hpp
#define hud_hpp

#include "sprite.hpp"
#include "player.hpp"
#include <vector>

class Graphics;

class HUD {
public:
    HUD();
    HUD(Graphics &graphics, Player &player);
    void update(int elapsedTime, Player &player);
    void draw(Graphics &graphics);
    
private:
    Player _player;

    Sprite _fov;
    
    
};

#endif /* hud_hpp */
