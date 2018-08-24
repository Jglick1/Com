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
#include "hud.hpp"
#include "cursor.hpp"
//#include "inventory.hpp"

//class Level;

class Game {
public:
    Game();
    ~Game();
    void printDirection(Direction inPower);
    void printGameState(GameState state);
    
    
private:
    void gameLoop();
    void draw(Graphics &graphics);
    void update(float elapsedTime, Direction &inPower, int mx, int my, int old_xm, int old_ym);
    void handleMovement(Direction &inPower, Input &input);
    
    Player _player;
    
    Level _level;
    
    //HUD _hud;
    
    //Inventory _inventory;
    
    //Graphics _graphics;
    
    float _deltaX;
    float _deltaY;
    
    //Unit _unit;
    
                    //erase eventually
    SDL_Texture* textSheet;
    
    HUD _hud;
    
    Cursor _cursor;
    
    GameState _gameState;
    
    
};

#endif /* game_hpp */
