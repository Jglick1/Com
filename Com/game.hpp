//
//  game.hpp
//  Com
//
//  Created by Jonah Glick on 6/20/18.
//
//

#ifndef game_hpp
#define game_hpp

#include "player.hpp"
#include "level.hpp"
#include "graphics.hpp"
#include "unit.hpp"
#include "input.hpp"
//#include "hud.hpp"
//#include "inventory.hpp"


class Game {
public:
    Game();
    ~Game();
    
private:
    void gameLoop();
    void draw(Graphics &graphics);
    void update(float elapsedTime, Direction &inPower);
    void handleMovement(Direction &inPower, Input &input);
    
    Player _player;
    
    Level _level;
    
    //HUD _hud;
    
    //Inventory _inventory;
    
    Graphics _graphics;
    
    float _deltaX;
    float _deltaY;
    
    Unit _unit;
    
    
};

#endif /* game_hpp */
