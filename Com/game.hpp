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
#include "gamestate.hpp"
#include "organizationChart.hpp"
#include "debugWindow.hpp"

class Game {
public:
    Game();
    ~Game();
    void printDirection(Direction inPower);
    void printActionState(ActionState direction);
    
    
    void giveUnitPointerToPlayer(std::shared_ptr<Unit> unit);
    
    
    
private:
    void gameLoop();
    void draw();
    //void update(float elapsedTime, Direction &inPower, int mx, int my, int old_xm, int old_ym, Graphics &graphics);
    void update(float elapsedTime, Direction &inPower, int mx, int my, int old_xm, int old_ym);
    void handleEvents();
    void handleMouseEvent();
    //void handleMovement(Direction &inPower, Input &input, Graphics &graphics);
    void handleMovement(Direction &inPower, Input &input);
    
    
    
    Player _player;
    
    Level _level;
    
    
    
    SDL_Texture* _textSheet;
    
    HUD _hud;
    
    Cursor _cursor;
    
    ActionState _actionState;
    
    OrganizationChart _organizationChart;
    
    DebugWindow _debugWindow;
    
    bool _isCameraFloating;
    bool _rightMouseDown;
    bool _isCameraRotating;
    
    int _mouseX;
    int _mouseY;
    
    Graphics _graphics;
    
    
    
};

#endif /* game_hpp */
