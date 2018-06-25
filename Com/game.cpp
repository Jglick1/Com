//
//  game.cpp
//  Com
//
//  Created by Jonah Glick on 6/20/18.
//
//

#include "game.hpp"
#include <SDL2/SDL.h>
#include "graphics.hpp"
#include "input.hpp"
#include <iostream>
#include <algorithm>
#include "globals.hpp"


namespace {
    const int FPS = 60;
    const int MAX_FRAME_TIME = 1000 / FPS;
}


Game::Game() {
    SDL_Init(SDL_INIT_EVERYTHING);
    this->gameLoop();
    
}

Game::~Game() {
    
}

void Game::gameLoop() {
    Graphics graphics;
    SDL_Event event;
    Input input;

    
    //this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
    
    this->_player = Player(graphics, Vector2(3,3));
    this->_unit = Unit(graphics, Vector2(0,0));
    
    this->_level = Level("/Users/jonahglick/Documents/Com/com_test1", graphics);
    //this->_hud = HUD(graphics, this->_player);
    //this->_inventory = Inventory(graphics, this->_player);
    
    int LAST_UPDATE_TIME = SDL_GetTicks();
    
    Direction inPower;
    
    bool keyInPower = 0;
    
    this->_deltaX = 0.0;
    this->_deltaY = 0.0;

    int old_xm = 0;
    
    int xm = 0;
    int ym = 0;
    
    while(true) {
        
        input.beginNewFrame();
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.repeat == 0) {
                    input.keyDownEvent(event);
                }
            }
            else if (event.type == SDL_KEYUP) {
                input.keyUpEvent(event);
            }
            else if (event.type == SDL_QUIT) {
                return;
            }
            else if (event.type == SDL_MOUSEMOTION) {
                this->_deltaX = event.motion.xrel;
            //    this->_deltaY += event.motion.y;
            }
            /*
            else if (input.isKeyHeld(SDL_SCANCODE_0)) {
                this->_level.changeAngle(10.0);
            }
            else if (input.isKeyHeld(SDL_SCANCODE_9)) {
                this->_level.changeAngle(-10.0);
            }
            */
            
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if(event.button.button == SDL_BUTTON_LEFT) {
                    //std:: cout << "x: " << this->_player.getMX() << "y: " << this->_player.getMY() << std::endl;
                    return;
                }
            }
            
            //else if (input.isKeyHeld(SDL_SCANCODE_I)) {
            //    this->_inventory_menu_active = !this->_inventory_menu_active;
            //}
        }
        
        
        /*
        if (!(this->_deltaX > 100) && (this->_deltaX > 2 || this->_deltaX < -2)) {
            this->_level.changeAngle(0.2*(this->_deltaX));
        }
         */
        
        //std::cout << this->_deltaX << std::endl;
        
        old_xm = xm;
        SDL_GetMouseState(&xm, &ym);
        if (std::abs((xm - old_xm)) < 100 && std::abs(xm - old_xm) > 1) {
            this->_level.changeAngle(-0.2*(xm - old_xm));
        }

        
        
        //std::cout << "x: " << xpos << " y: " << ypos << std::endl;
        
        if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
            return;
        }

        if(keyInPower) {
            switch (inPower) {
                case UP:
                    if (!input.isKeyHeld(SDL_SCANCODE_UP)) {
                        keyInPower = 0;
                    }
                    this->_level.moveBackward();
                    /*
                    else if(input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
                        inPower = UPRIGHT;
                        keyInPower = 1;
                        this->_player.moveUpAndRight();
                    }
                    else if(input.isKeyHeld(SDL_SCANCODE_LEFT)) {
                        inPower = UPLEFT;
                        keyInPower = 1;
                        this->_player.moveUpAndLeft();
                    }
                     */
                    break;
                     
                case DOWN:
                    if (!input.isKeyHeld(SDL_SCANCODE_DOWN)) {
                        keyInPower = 0;
                    }
                    this->_level.moveForward();
                    /*
                    else if(input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
                        inPower = DOWNRIGHT;
                        keyInPower = 1;
                        this->_player.moveDownAndRight();
                    }
                    else if(input.isKeyHeld(SDL_SCANCODE_LEFT)) {
                        inPower = DOWNLEFT;
                        keyInPower = 1;
                        this->_player.moveDownAndLeft();
                    }
                     */
                    break;
                     
                case RIGHT:
                    if (!input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
                        keyInPower = 0;
                    }
                    this->_level.moveLeft();
                    /*
                    else if(input.isKeyHeld(SDL_SCANCODE_UP)) {
                        inPower = UPRIGHT;
                        keyInPower = 1;
                        this->_player.moveUpAndRight();
                    }
                    else if(input.isKeyHeld(SDL_SCANCODE_DOWN)) {
                        inPower = DOWNRIGHT;
                        keyInPower = 1;
                        this->_player.moveDownAndRight();
                    }
                    */
                    break;
                case LEFT:
                    if (!input.isKeyHeld(SDL_SCANCODE_LEFT)) {
                        keyInPower = 0;
                    }
                    this->_level.moveRight();
                    /*
                    else if(input.isKeyHeld(SDL_SCANCODE_UP)) {
                        inPower = UPLEFT;
                        keyInPower = 1;
                        this->_player.moveUpAndLeft();
                    }
                    else if(input.isKeyHeld(SDL_SCANCODE_DOWN)) {
                        inPower = DOWNLEFT;
                        keyInPower = 1;
                        this->_player.moveDownAndLeft();
                    }
                     */
                    break;
                    /*
                case UPRIGHT:
                    if(!input.isKeyHeld(SDL_SCANCODE_UP) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)){
                        keyInPower = 0;
                    }
                    else if(!input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
                        keyInPower = 1;
                        inPower = UP;
                        //this->_player.moveUp();
                        this->_level.moveBackward();
                    }
                    else if(!input.isKeyHeld(SDL_SCANCODE_UP)) {
                        keyInPower = 1;
                        inPower = RIGHT;
                        //this->_player.moveRight();
                        this->_level.moveLeft();
                    }
                    break;
                case UPLEFT:
                    if(!input.isKeyHeld(SDL_SCANCODE_UP) && !input.isKeyHeld(SDL_SCANCODE_LEFT)){
                        keyInPower = 0;
                    }
                    else if(!input.isKeyHeld(SDL_SCANCODE_LEFT)) {
                        keyInPower = 1;
                        inPower = UP;
                        //this->_player.moveUp();
                        this->_level.moveBackward();
                    }
                    else if(!input.isKeyHeld(SDL_SCANCODE_UP)) {
                        keyInPower = 1;
                        inPower = LEFT;
                        //this->_player.moveLeft();
                        this->_level.moveRight();
                    }
                    break;
                case DOWNRIGHT:
                    if(!input.isKeyHeld(SDL_SCANCODE_DOWN) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)){
                        keyInPower = 0;
                    }
                    else if(!input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
                        keyInPower = 1;
                        inPower = DOWN;
                        this->_player.moveDown();
                    }
                    else if(!input.isKeyHeld(SDL_SCANCODE_DOWN)) {
                        keyInPower = 1;
                        inPower = RIGHT;
                        this->_player.moveRight();
                    }
                    break;
                case DOWNLEFT:
                    if(!input.isKeyHeld(SDL_SCANCODE_DOWN) && !input.isKeyHeld(SDL_SCANCODE_LEFT)){
                        keyInPower = 0;
                    }
                    else if(!input.isKeyHeld(SDL_SCANCODE_LEFT)) {
                        keyInPower = 1;
                        inPower = DOWN;
                        this->_player.moveDown();
                    }
                    else if(!input.isKeyHeld(SDL_SCANCODE_DOWN)) {
                        keyInPower = 1;
                        inPower = LEFT;
                        this->_player.moveLeft();
                    }
                    break;
                     */
            }
        }
        
        else {
            /*
            if (input.isKeyHeld(SDL_SCANCODE_UP) == true && input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
                inPower = UPRIGHT;
                keyInPower = 1;
                this->_player.moveUpAndRight();
            }
            else if(input.isKeyHeld(SDL_SCANCODE_UP) == true && input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
                inPower = UPLEFT;
                keyInPower = 1;
                this->_player.moveUpAndLeft();
            }
            else if(input.isKeyHeld(SDL_SCANCODE_DOWN) == true && input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
                inPower = DOWNRIGHT;
                keyInPower = 1;
                this->_player.moveDownAndRight();
            }
            else if(input.isKeyHeld(SDL_SCANCODE_DOWN) == true && input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
                inPower = DOWNLEFT;
                keyInPower = 1;
                this->_player.moveDownAndLeft();
            }
             */
            if (input.isKeyHeld(SDL_SCANCODE_UP) == true) {
                inPower = UP;
                keyInPower = 1;
                //this->_player.moveUp();
                this->_level.moveBackward();
            }
            else if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true) {
                inPower = DOWN;
                keyInPower = 1;
                //this->_player.moveDown();
                this->_level.moveForward();
            }
            else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
                inPower = RIGHT;
                keyInPower = 1;
                //this->_player.moveRight();
                this->_level.moveLeft();
            }
            else if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
                inPower = LEFT;
                keyInPower = 1;
                //this->_player.moveLeft();
                this->_level.moveRight();
            }
            
            
            
            
            
            else {
                //this->_player.stopMoving();
                this->_level.stopMoving();
            }
        }
        
        //this->_unit.moveLeft();
        
        
        const int CURRENT_TIME_MS = SDL_GetTicks();
        int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
        
        
        this->_graphics = graphics;
        
        
        //if(!this->_inventory_menu_active) {					//if it is not paused
        //    this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
        //}
        this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
        
        LAST_UPDATE_TIME = CURRENT_TIME_MS;
        
        if (ELAPSED_TIME_MS < MAX_FRAME_TIME) {
            //std::cout << "delay: " << MAX_FRAME_TIME - ELAPSED_TIME_MS << "\n";
            SDL_Delay(MAX_FRAME_TIME - ELAPSED_TIME_MS);
        }
        
        this->draw(graphics);
        
    }
    
}




void Game::draw(Graphics &graphics) {
    graphics.clear();
    
    
    this->_level.draw(graphics);
    this->_player.draw(graphics);
    
    this->_unit.draw(graphics);
    
    //this->_hud.draw(graphics);
    
    //if(this->_inventory_menu_active) {
    //    this->_inventory.draw(graphics);
    //}
    //std::cout << this->_inventory_menu_active << std::endl;
    
    graphics.flip();
    
    
}

void Game::update(float elapsedTime) {
    
    
    /*
    std::vector<Rectangle> others;
    if((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
        this->_player.handleTileCollisions(others);
        this->_level.handleTileCollisions();
    }
    
    std::vector<Door> otherDoors;
    if((otherDoors = this->_level.checkDoorCollisions(this->_player.getBoundingBox())).size() > 0) {
        this->_player.handleDoorCollision(otherDoors, this->_level, this->_graphics);
        this->_isOutside = 0;
    }
    */
    
    int mx;
    int my;
    
    SDL_GetMouseState(&mx, &my);
    
    this->_player.update(elapsedTime, mx, my);
    this->_level.update(elapsedTime);
    this->_unit.update(elapsedTime);
    //this->_hud.update(elapsedTime, this->_player);
    
    
}
