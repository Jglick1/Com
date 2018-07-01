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
    
    Direction inPower = NONE;
    
    this->_deltaX = 0.0;
    this->_deltaY = 0.0;

    int old_xm = 0;
    
    int xm = 0;
    int ym = 0;
    
    bool wasThereAnEvent = 0;
    
    while(true) {

        
        input.beginNewFrame();
        
        
        wasThereAnEvent = 0;
        while (SDL_PollEvent(&event)) {
            wasThereAnEvent = 1;
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
            //else if (event.type == SDL_MOUSEMOTION) {
            //    this->_deltaX = event.motion.xrel;
            //    this->_deltaY += event.motion.y;
            //}

            
            //else if (event.type == SDL_MOUSEBUTTONDOWN) {
            //    if(event.button.button == SDL_BUTTON_LEFT) {
            //        //std:: cout << "x: " << this->_player.getMX() << "y: " << this->_player.getMY() << std::endl;
            //       return;
            //    }
            //}
            

        }

        old_xm = xm;
        SDL_GetMouseState(&xm, &ym);

        if (std::abs((xm - old_xm)) < 100) {
            this->_level.changeAngle(-0.2*(xm - old_xm));
        }

    
        if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
            return;
        }

        
        //handle input
        //handle movement
        //render
        
        this->_unit.setPlayerAngle(this->_level.getAngle());
        
        
        //if(wasThereAnEvent) {
            this->handleMovement(inPower, input);
        //}
        
 
        
        
        const int CURRENT_TIME_MS = SDL_GetTicks();
        int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
        
        this->_graphics = graphics;
        
        this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), inPower);
        

        LAST_UPDATE_TIME = CURRENT_TIME_MS;
        
        if (ELAPSED_TIME_MS < MAX_FRAME_TIME) {
            SDL_Delay(MAX_FRAME_TIME - ELAPSED_TIME_MS);
        }
        
        this->draw(graphics);
        this->_unit.stopMoving();
        
    }
    
}

void Game::handleMovement(Direction &inPower, Input &input) {
    switch (inPower) {
        case UP:
            if (input.wasKeyReleased(SDL_SCANCODE_W)) {
                inPower = NONE;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.moveBackward();
                this->_unit.moveBackwardParallax();
            }
            
            break;
            
        case DOWN:
            if (input.wasKeyReleased(SDL_SCANCODE_S)) {
                inPower = NONE;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.moveForward();
                this->_unit.moveForwardParallax();
            }
            
            break;
            
        case RIGHT:
            if (input.wasKeyReleased(SDL_SCANCODE_D)) {
                inPower = NONE;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.moveLeft();
                this->_unit.moveLeftParallax();
            }
            break;
            
        case LEFT:
            if (!input.isKeyHeld(SDL_SCANCODE_A)) {
                inPower = NONE;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.moveRight();
                this->_unit.moveRightParallax();
            }
            
            break;
        case NONE:
            if(input.isKeyHeld(SDL_SCANCODE_W)) {
                inPower = UP;
                this->handleMovement(inPower, input);
            }
            else if(input.isKeyHeld(SDL_SCANCODE_S)) {
                inPower = DOWN;
                this->handleMovement(inPower, input);
            }
            else if(input.isKeyHeld(SDL_SCANCODE_A)) {
                inPower = LEFT;
                this->handleMovement(inPower, input);
            }
            else if(input.isKeyHeld(SDL_SCANCODE_D)) {
                inPower = RIGHT;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.stopMoving();
                this->_unit.stopMoving();
            }
            
    }
}


void Game::draw(Graphics &graphics) {
    graphics.clear();
    
    
    this->_level.draw(graphics);
    this->_player.draw(graphics);
    
    this->_unit.draw(graphics);

    
    graphics.flip();
    
    
}

void Game::update(float elapsedTime, Direction &inPower) {
    
    /*
    std::vector<Rectangle> others;
    if((others = this->_level.checkTileCollisions(this->_player.getPlayerBoundingBox())).size() > 0) {
        //this->_player.handleTileCollisions(others);
        this->_level.handleTileCollisions(others);
        this->_unit.handleTileCollisions();
    }
     */
    
    
    
    
    /*
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
    this->_unit.update(elapsedTime, this->_level.getAngle());
    //this->_hud.update(elapsedTime, this->_player);
        
    
    std::vector<Rectangle> others;
    if((others = this->_level.checkTileCollisions(this->_player.getPlayerBoundingBox())).size() > 0) {
        //this->_player.handleTileCollisions(others);
        this->_level.handleTileCollisions(others);
        this->_unit.handleTileCollisions();
    }
    
    
   
}
