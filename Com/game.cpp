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
    
    this->_actionState = NORMAL;
    
    this->gameLoop();
    
}

Game::~Game() {
    
}


//handle input
//handle movement
//render


void Game::gameLoop() {
    
    Graphics graphics;
    
    SDL_Event event;
    Input input;
    
    graphics.loadSound();
    
    
    /*
    textSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadText("Hello World"));
    if(textSheet == NULL) {
        printf("Error: Unable to load text\n");
    }
     */

    
    this->_player = Player(graphics, Vector2(3,3));
    this->_level = Level("/Users/jonahglick/Documents/Com/com_test6", graphics);
    this->_organizationChart = OrganizationChart(graphics);
    this->_hud = HUD(graphics, this->_player);
    
    int LAST_UPDATE_TIME = SDL_GetTicks();
    
    Direction inPower = NONE;

    int old_xm = 0;
    int old_ym = 0;
    int xm = 0;
    int ym = 0;
    
    
    
    bool rightMouseDown = 0;
    bool double_time = 0;
    bool rightMouseClick = 0;
    
    
    
    int FRAME_CALCULATE_TIME_BEGIN = 0;
    int FRAME_CALCULATE_TIME_END = 0;
    
    while(true) {

        
        input.beginNewFrame();
        
        //HANDLE INPUT
        
        old_xm = xm;
        old_ym = ym;
        SDL_GetMouseState(&xm, &ym); //update mouse position
        
        
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
            if (event.type == SDL_MOUSEBUTTONDOWN) {

                if (event.button.button == SDL_BUTTON_RIGHT) {
                    
                    rightMouseDown = 1;
                    if(this->_actionState == NORMAL) {
                        this->_actionState = COMMAND;
                        graphics.storeCameraCoordinates();
                    }

                    
                    
                }
                if(event.button.button == SDL_BUTTON_LEFT) {
                    if(this->_actionState == ORGANIZATION) {
                        this->_organizationChart.handleMouseCollision(graphics, xm, ym);
                    }
                    else if(this->_actionState == COMMAND) {
                        this->_actionState = NORMAL;
                        //graphics.revertToRegularCameraCoordinates();                      nwe command state
                    }
                    else {
                        this->_level.playerFireShot(graphics);
                        //printf("left mouse down");
                    }
                    
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    rightMouseDown = 0;
                    //printf("mouse up\n");
                }
                if (event.button.button == SDL_BUTTON_LEFT) {   //left mouse up
                    if(this->_actionState == ORGANIZATION) {
                        if(this->_organizationChart.isSelected()) {
                            this->_organizationChart.handleMouseLiftCollision(graphics, xm, ym);
                        }
                        this->_organizationChart.setToNotSelected();
                    }
                }
            }
            
        }
        
        
        //check for click
        if(rightMouseDown == 1) {
            if(double_time == 0) {
                rightMouseClick = 1;
                //this->_gameState = SLIDE_MOVE;
            }
            else {
                rightMouseClick = 0;
            }
            double_time = 1;
        }
        else {
            rightMouseClick = 0;
            double_time = 0;
        }
        
        
        

        
        
        // UPDATE
        
        
        
        
    
        
        
        if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
            return;
        }
        
        if(input.wasKeyPressed(SDL_SCANCODE_X)) {
            //this->_level.moveUnitToPosition(480, 336);
            //this->_level.moveUnitToPosition(610, 290);
            //this->_level.moveUnitToPosition(790, 400, graphics);
            this->_level.changeDrawFoVNode();
        }
        
        if(input.wasKeyPressed(SDL_SCANCODE_H)){                   //this breaks it
            this->_level.moveUnitToPosition(0, 0, graphics);
            //graphics.drawShape();
        }
        
        if(input.wasKeyPressed(SDL_SCANCODE_G)) {
            //graphics.playShot();
            graphics.eraseDebugLines();
            this->_level.clearGunshotPaths();
        }
        
        if(input.wasKeyPressed(SDL_SCANCODE_O)) {
            if(this->_actionState == ORGANIZATION) {
                this->_actionState = NORMAL;
            }
            else {
                this->_actionState = ORGANIZATION;
            }
        }
        
        if(input.wasKeyPressed(SDL_SCANCODE_C)) {
            //graphics.playShot();
            this->_level.moveUnitToNearestCover(graphics);
        }

        if((this->_actionState == COMMAND) && rightMouseClick) {
            if(this->_level.checkSlideCollision(xm, ym)) {
                this->_actionState = SLIDE_MOVE;
                //printf("collision!\n");
            }
        }
        
        if(this->_actionState == SLIDE_MOVE) {                      //slider is released
            if(rightMouseDown == 0) {   //sliders is released
                this->_actionState = COMMAND;
                
                this->_level.handleSlideRelease(graphics);
                
            }
            else {
                this->_level.handleSlideMovement(xm, ym, graphics);
            }
        }
        else if(this->_actionState == ORGANIZATION) {
            this->_organizationChart.handleMouseHover(xm, ym, graphics);
        }
        else if (this->_actionState == COMMAND) {
            //graphics.updateCommandCameraOffset(old_xm, old_ym, xm, ym);                           new command state
        }
        

        this->handleMovement(inPower, input, graphics);
        

        
        
        const int CURRENT_TIME_MS = SDL_GetTicks();                         //getting total elapsedTime
        int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
        
        
        LAST_UPDATE_TIME = CURRENT_TIME_MS;
        
        this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), inPower, xm, ym, old_xm, old_ym, graphics);
        
        
        
        
        
        
        

        //RENDER
        
        
        
        
        
        
        
        this->draw(graphics);
        
        
        
        FRAME_CALCULATE_TIME_END = SDL_GetTicks();
        

        
        int TOTAL_CALC_TIME = FRAME_CALCULATE_TIME_END - FRAME_CALCULATE_TIME_BEGIN;            //getting time to spare in frame
        
        if ((TOTAL_CALC_TIME) < MAX_FRAME_TIME) {
            SDL_Delay(MAX_FRAME_TIME - (TOTAL_CALC_TIME));
        }
        
        FRAME_CALCULATE_TIME_BEGIN = SDL_GetTicks();

        
        
        
        graphics.updateFrameTimeIndicator(TOTAL_CALC_TIME);
        
    }
    
}

void Game::handleMovement(Direction &inPower, Input &input, Graphics &graphics) {
    

    switch (inPower) {
        case UP:
            if (input.wasKeyReleased(SDL_SCANCODE_W)) {
                inPower = NONE;
                this->handleMovement(inPower, input, graphics);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_D)) {
                inPower = UPRIGHT;
                this->handleMovement(inPower, input, graphics);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_A)) {
                inPower = UPLEFT;
                this->handleMovement(inPower, input, graphics);
            }
            else {
                graphics.moveCameraForward();
            }
            
            break;
            
        case DOWN:
            if (input.wasKeyReleased(SDL_SCANCODE_S)) {
                inPower = NONE;
                this->handleMovement(inPower, input, graphics);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_D)) {
                inPower = DOWNRIGHT;
                this->handleMovement(inPower, input, graphics);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_A)) {
                inPower = DOWNLEFT;
                this->handleMovement(inPower, input, graphics);
            }
            else {
                graphics.moveCameraBackward();
            }
            
            break;
            
        case RIGHT:
            if (input.wasKeyReleased(SDL_SCANCODE_D)) {
                inPower = NONE;
                this->handleMovement(inPower, input, graphics);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_W)) {
                inPower = UPRIGHT;
                this->handleMovement(inPower, input, graphics);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_S)) {
                inPower = DOWNRIGHT;
                this->handleMovement(inPower, input, graphics);
            }
            else {
                graphics.moveCameraRight();
            }
            break;
            
        case LEFT:
            if (input.wasKeyReleased(SDL_SCANCODE_A)) {
                inPower = NONE;
                this->handleMovement(inPower, input, graphics);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_W)) {
                inPower = UPLEFT;
                this->handleMovement(inPower, input, graphics);
            }
            else if(input.wasKeyPressed(SDL_SCANCODE_S)) {
                inPower = DOWNLEFT;
                this->handleMovement(inPower, input, graphics);
            }
            else {
                graphics.moveCameraLeft();
            }
            
            break;
        case UPRIGHT:
            if(input.wasKeyReleased(SDL_SCANCODE_W)) {
                if(input.wasKeyReleased(SDL_SCANCODE_D)) {
                    inPower = NONE;
                    this->handleMovement(inPower, input, graphics);
                }
                else {
                    inPower = RIGHT;
                    this->handleMovement(inPower, input, graphics);
                }
            }
            else if(input.wasKeyReleased(SDL_SCANCODE_D)) {
                inPower = UP;
                this->handleMovement(inPower, input, graphics);
            }
            else {
                graphics.moveCameraUpRight();
            }
            break;
        case UPLEFT:
            if(input.wasKeyReleased(SDL_SCANCODE_W)) {
                if(input.wasKeyReleased(SDL_SCANCODE_A)) {
                    inPower = NONE;
                    this->handleMovement(inPower, input, graphics);
                }
                else {
                    inPower = LEFT;
                    this->handleMovement(inPower, input, graphics);
                }
            }
            else if(input.wasKeyReleased(SDL_SCANCODE_A)) {
                inPower = UP;
                this->handleMovement(inPower, input, graphics);
            }
            else {
                graphics.moveCameraUpLeft();
            }
            break;
        case DOWNRIGHT:
            if(input.wasKeyReleased(SDL_SCANCODE_S)) {
                if(input.wasKeyReleased(SDL_SCANCODE_D)) {
                    inPower = NONE;
                    this->handleMovement(inPower, input, graphics);
                }
                else {
                    inPower = RIGHT;
                    this->handleMovement(inPower, input, graphics);
                }
            }
            else if(input.wasKeyReleased(SDL_SCANCODE_D)) {
                inPower = DOWN;
                this->handleMovement(inPower, input, graphics);
            }
            else {
                graphics.moveCameraDownRight();
            }
            break;
        case DOWNLEFT:
            if(input.wasKeyReleased(SDL_SCANCODE_S)) {
                if(input.wasKeyReleased(SDL_SCANCODE_A)) {
                    inPower = NONE;
                    this->handleMovement(inPower, input, graphics);
                }
                else {
                    inPower = LEFT;
                    this->handleMovement(inPower, input, graphics);
                }
            }
            else if(input.wasKeyReleased(SDL_SCANCODE_A)) {
                inPower = DOWN;
                this->handleMovement(inPower, input, graphics);
            }
            else {
                graphics.moveCameraDownLeft();
            }
            break;
        case NONE:
            if(input.isKeyHeld(SDL_SCANCODE_W)) {
                if(input.isKeyHeld(SDL_SCANCODE_D)) {
                    inPower = UPRIGHT;
                    this->handleMovement(inPower, input, graphics);
                }
                else if(input.isKeyHeld(SDL_SCANCODE_A)) {
                    inPower = UPLEFT;
                    this->handleMovement(inPower, input, graphics);
                }
                else {
                    inPower = UP;
                    this->handleMovement(inPower, input, graphics);
                }
            }
            else if(input.isKeyHeld(SDL_SCANCODE_S)) {
                if(input.isKeyHeld(SDL_SCANCODE_D)) {
                    inPower = DOWNRIGHT;
                    this->handleMovement(inPower, input, graphics);
                }
                else if(input.isKeyHeld(SDL_SCANCODE_A)) {
                    inPower = DOWNLEFT;
                    this->handleMovement(inPower, input, graphics);
                }
                else {
                    inPower = DOWN;
                    this->handleMovement(inPower, input, graphics);
                }
            }
            else if(input.isKeyHeld(SDL_SCANCODE_A)) {
                inPower = LEFT;
                this->handleMovement(inPower, input, graphics);
            }
            else if(input.isKeyHeld(SDL_SCANCODE_D)) {
                inPower = RIGHT;
                this->handleMovement(inPower, input, graphics);
            }
            else {
                graphics.stopCameraMoving();
            }
            break;
            
    }
    
    this->_level.handleUnitMovement();
}


void Game::draw(Graphics &graphics) {
    graphics.clear();

    
    if(this->_actionState != ORGANIZATION) {
        this->_level.draw(graphics);
        this->_player.draw(graphics);
        
        //graphics.renderText();
        
        this->_hud.draw(graphics);
        
        this->_cursor.draw(graphics);
        
        graphics.drawDebug();       //draw all debug lines
        //graphics.eraseDebugLines();      //erase all stored debug lines
        
        graphics.drawCircle(640, 400, 8);
        
    }
    else {
        
        this->_organizationChart.draw(graphics);
        
    }

    
    
    graphics.flip();
    
    
}

void Game::update(float elapsedTime, Direction &inPower, int xm, int ym, int old_xm, int old_ym, Graphics &graphics) {

    
    if ((this->_actionState == NORMAL) && std::abs((xm - old_xm)) < 100) {
        
        graphics.changeAngle(-0.5*(xm - old_xm));
        
    }

    
    this->_level.setUnitAngle();
    
    graphics.update(elapsedTime);
    
    this->_level.update(elapsedTime, graphics);
    
    this->_player.update(elapsedTime, xm, ym);

    this->_cursor.update(elapsedTime, ym, old_ym, xm, old_xm, graphics.getCameraAngle(), graphics);
    
    this->_organizationChart.update(xm, ym);
    
    
    
    this->_level.handlePlayerCollisions(elapsedTime, graphics);
    
    
    this->_hud.update(elapsedTime, this->_player);
   
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

void Game::printActionState(ActionState actionState) {
    switch(actionState) {
        case NORMAL:
            printf("NORMAL\n");
            break;
        case SLIDE_MOVE:
            printf("SLIDE_MOVE\n");
            break;
        case COMMAND:
            printf("COMMAND\n");
            break;
        case NONE:
            printf("NONE\n");
            break;
    }
}
