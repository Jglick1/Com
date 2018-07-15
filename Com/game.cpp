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
    const int FPS = 30;
    const int MAX_FRAME_TIME = 1000 / FPS;
}


Game::Game() {
    //printf("SDL_Init\n");
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return;
    }
    
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return;
    }
    
    this->gameLoop();
    
}

Game::~Game() {
    
}

void Game::gameLoop() {
    //printf("beginning game loop\n");
    
    Graphics graphics;
    SDL_Event event;
    Input input;
    
    graphics.loadSound();
    
    //graphics.resolutionTest();
    
    /*
    textSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadText("Hello World"));
    if(textSheet == NULL) {
        printf("Error: Unable to load text\n");
    }
     */


    
    //this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
    
    this->_player = Player(graphics, Vector2(3,3));
    //this->_unit = Unit(graphics, Vector2(0,0));
    
    //this->_unit.moveToPosition(1280, 0);
    
    this->_level = Level("/Users/jonahglick/Documents/Com/com_test2", graphics);
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
            this->_level.changeAngle(-0.5*(xm - old_xm));
        }

    
        if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
            return;
        }
        
        if(input.wasKeyPressed(SDL_SCANCODE_X)) {
            this->_level.moveUnitToPosition(500, 500);
        }
        
        if(input.wasKeyPressed(SDL_SCANCODE_G)) {
            graphics.playShot();
        }

        
        //handle input
        //handle movement
        //render
        
        //this->_unit.setPlayerAngle(this->_level.getAngle());
        this->_level.setUnitAngle();
        
        
        //if(wasThereAnEvent) {
            this->handleMovement(inPower, input);
        //}
        
        //this->_unit.moveForward();
        
        
        const int CURRENT_TIME_MS = SDL_GetTicks();
        int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
        
        //this->_graphics = graphics;
        
        this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), inPower);
        

        LAST_UPDATE_TIME = CURRENT_TIME_MS;
        
        if (ELAPSED_TIME_MS < MAX_FRAME_TIME) {
            SDL_Delay(MAX_FRAME_TIME - ELAPSED_TIME_MS);
        }
        
        this->draw(graphics);
        //this->_unit.stopMoving();
        
        //handle movement
        //update
        //draw
        
    }
    
}

void Game::handleMovement(Direction &inPower, Input &input) {
    
    //this->_unit.handleMovement();
    
    //this->_level.handleUnitMovement();
    
    //this->printDirection(inPower);
    
    
    switch (inPower) {
        case UP:
            if (input.wasKeyReleased(SDL_SCANCODE_W)) {
                inPower = NONE;
                this->handleMovement(inPower, input);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_D)) {
                inPower = UPRIGHT;
                this->handleMovement(inPower, input);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_A)) {
                inPower = UPLEFT;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.moveBackward();
            }
            
            break;
            
        case DOWN:
            if (input.wasKeyReleased(SDL_SCANCODE_S)) {
                inPower = NONE;
                this->handleMovement(inPower, input);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_D)) {
                inPower = DOWNRIGHT;
                this->handleMovement(inPower, input);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_A)) {
                inPower = DOWNLEFT;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.moveForward();
            }
            
            break;
            
        case RIGHT:
            if (input.wasKeyReleased(SDL_SCANCODE_D)) {
                inPower = NONE;
                this->handleMovement(inPower, input);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_W)) {
                inPower = UPRIGHT;
                this->handleMovement(inPower, input);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_S)) {
                inPower = DOWNRIGHT;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.moveLeft();
            }
            break;
            
        case LEFT:
            if (input.wasKeyReleased(SDL_SCANCODE_A)) {
                inPower = NONE;
                this->handleMovement(inPower, input);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_W)) {
                inPower = UPLEFT;
                this->handleMovement(inPower, input);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_S)) {
                inPower = DOWNLEFT;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.moveRight();
            }
            
            break;
        case UPRIGHT:
            if(input.wasKeyReleased(SDL_SCANCODE_W)) {
                if(input.wasKeyReleased(SDL_SCANCODE_D)) {
                    inPower = NONE;
                    this->handleMovement(inPower, input);
                }
                else {
                    inPower = RIGHT;
                    this->handleMovement(inPower, input);
                }
            }
            else if(input.wasKeyReleased(SDL_SCANCODE_D)) {
                inPower = UP;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.moveDownLeft();
            }
            break;
        case UPLEFT:
            if(input.wasKeyReleased(SDL_SCANCODE_W)) {
                if(input.wasKeyReleased(SDL_SCANCODE_A)) {
                    inPower = NONE;
                    this->handleMovement(inPower, input);
                }
                else {
                    inPower = LEFT;
                    this->handleMovement(inPower, input);
                }
            }
            else if(input.wasKeyReleased(SDL_SCANCODE_A)) {
                inPower = UP;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.moveDownRight();
            }
            break;
        case DOWNRIGHT:
            if(input.wasKeyReleased(SDL_SCANCODE_S)) {
                if(input.wasKeyReleased(SDL_SCANCODE_D)) {
                    inPower = NONE;
                    this->handleMovement(inPower, input);
                }
                else {
                    inPower = RIGHT;
                    this->handleMovement(inPower, input);
                }
            }
            else if(input.wasKeyReleased(SDL_SCANCODE_D)) {
                inPower = DOWN;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.moveUpLeft();
            }
            break;
        case DOWNLEFT:
            if(input.wasKeyReleased(SDL_SCANCODE_S)) {
                if(input.wasKeyReleased(SDL_SCANCODE_A)) {
                    inPower = NONE;
                    this->handleMovement(inPower, input);
                }
                else {
                    inPower = LEFT;
                    this->handleMovement(inPower, input);
                }
            }
            else if(input.wasKeyReleased(SDL_SCANCODE_A)) {
                inPower = DOWN;
                this->handleMovement(inPower, input);
            }
            else {
                this->_level.moveUpRight();
            }
            break;
        case NONE:
            if(input.isKeyHeld(SDL_SCANCODE_W)) {
                if(input.isKeyHeld(SDL_SCANCODE_D)) {
                    inPower = UPRIGHT;
                    this->handleMovement(inPower, input);
                }
                else if(input.isKeyHeld(SDL_SCANCODE_A)) {
                    inPower = UPLEFT;
                    this->handleMovement(inPower, input);
                }
                else {
                    inPower = UP;
                    this->handleMovement(inPower, input);
                }
            }
            else if(input.isKeyHeld(SDL_SCANCODE_S)) {
                if(input.isKeyHeld(SDL_SCANCODE_D)) {
                    inPower = DOWNRIGHT;
                    this->handleMovement(inPower, input);
                }
                else if(input.isKeyHeld(SDL_SCANCODE_A)) {
                    inPower = DOWNLEFT;
                    this->handleMovement(inPower, input);
                }
                else {
                    inPower = DOWN;
                    this->handleMovement(inPower, input);
                }
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
            }
            break;
            
    }
    
    this->_level.handleUnitMovement();
}


void Game::draw(Graphics &graphics) {
    graphics.clear();

    
    this->_level.draw(graphics);
    this->_player.draw(graphics);
    
    //SDL_Rect destinationRectangle = { 0, 0, 200, 200};
    //graphics.blitSurface(this->textSheet, NULL, &destinationRectangle);
    
    //graphics.renderText();
    
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


    //this->_unit.moveForward();
    
    
    int mx;
    int my;
    
    SDL_GetMouseState(&mx, &my);
    this->_level.update(elapsedTime);
    
    this->_player.update(elapsedTime, mx, my);

    //this->_unit.moveToPosition(1280, 0);
    
    //this->_unit.setDXDY(this->_level.getDX(), this->_level.getDY());
    //this->_unit.update(elapsedTime, this->_level.getAngle());

    
    
    
                                                                //handle collisions shoudl come after update for some reason
    std::vector<Rectangle> others;
    if((others = this->_level.checkTileCollisions(this->_player.getPlayerBoundingBox())).size() > 0) {
        //this->_player.handleTileCollisions(others);
        //printf("tile collision!a");
        this->_level.handleTileCollisions(others, elapsedTime);
        
        //this->_unit.handleTileCollisions(others, elapsedTime);
    }

   
}

void Game::printDirection(Direction inPower) {
    switch(inPower) {
        case UP:
            printf("UP\n");
            break;
        case DOWN:
            printf("DOWN\n");
            break;
        case RIGHT:
            printf("RIGHT\n");
            break;
        case LEFT:
            printf("LEFT\n");
            break;
        case UPRIGHT:
            printf("UPRIGHT\n");
            break;
        case UPLEFT:
            printf("UPLEFT\n");
            break;
        case DOWNRIGHT:
            printf("DOWNRIGHT\n");
            break;
        case DOWNLEFT:
            printf("DOWNLEFT\n");
            break;
        case NONE:
            printf("NONE\n");
            break;
    }
}
