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


Game::Game() :
_graphics(Graphics(2)),
_debugWindow(DebugWindow())
{

    //this->_graphics = Graphics(1);
    /*
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
    
    */
    
    this->_actionState = NORMAL;
    
    //this might be the wrong spot for it. But it seems to work here.
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    
    this->_isCameraFloating = 1;
    this->_rightMouseDown = 0;
    this->_isCameraRotating = 0;
    
    
    
    //we might actually not need this
    //this->_graphics = Graphics(2);
    
    
    //this->_debugWindow = DebugWindow(2);
    
    this->_player = Player(this->_graphics, Vector2(3,3));
    printf("player called\n");
    
    this->_level = Level("/Users/jonahglick/Documents/Com/com_test6", this->_graphics);
    //this->_level = Level("/Users/jonahglick/Documents/Com/dehast.png", this->_graphics);
    this->_organizationChart = OrganizationChart(this->_graphics);
    this->_hud = HUD(this->_graphics, this->_player);
    
    this->_organizationChart.readUnitInformation(this->_level.returnFireteam(0), this->_level.returnFireteam(1), this->_level.returnFireteam(2), this->_graphics);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    this->gameLoop();
    
}

Game::~Game() {
    
}


//handle input
//handle movement
//render


void Game::gameLoop() {
    
    //Graphics graphics;
    
    
    
    //graphics.storeLineOnBackground(0, 0, 100, 100);
    //graphics.storeLineOnBackground(200, 200, 500, 500);
    
    
    SDL_Event event;
    
    
    
    
    Input input;
    
    this->_graphics.loadSound();
    
    
    
    this->_textSheet = SDL_CreateTextureFromSurface(this->_graphics.getRenderer(), this->_graphics.loadText("Hello World"));
    if(this->_textSheet == NULL) {
        printf("Error: Unable to load text\n");
    }
     

    /*
    this->_player = Player(graphics, Vector2(3,3));
    this->_level = Level("/Users/jonahglick/Documents/Com/com_test6", graphics);
    //this->_level = Level("/Users/jonahglick/Documents/Com/dehast.png", graphics);
    this->_organizationChart = OrganizationChart(graphics);
    this->_hud = HUD(graphics, this->_player);
    
    this->_organizationChart.readUnitInformation(this->_level.returnFireteam(0), this->_level.returnFireteam(1), this->_level.returnFireteam(2), graphics);
    */
     
     
    printf("test\n");
    //this->_debugWindow = DebugWindow(graphics);
    
    
    
    std::shared_ptr<Unit> testUnit = this->_level.getPointerToAUnit();
    giveUnitPointerToPlayer(testUnit);
    
    
    
    
    
    
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
        
        
        this->handleEvents();
        
        
        
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
                    this->_rightMouseDown = 1;
                    rightMouseDown = 1;
                    if(this->_actionState == NORMAL && this->_isCameraFloating == 0) {
                        this->_actionState = COMMAND;
                        this->_graphics.storeCameraCoordinates();
                    }
                    else if(this->_actionState == ORGANIZATION && this->_isCameraFloating == 0) {
                        this->_organizationChart.cameraDragging();
                        this->_organizationChart.handleCameraMove(xm - old_xm, ym - old_ym);
                    }
                    else if((this->_actionState == COMMAND) && this->_isCameraFloating == 0) {
                        if(this->_level.checkSlideCollision(xm, ym)) {
                            this->_actionState = SLIDE_MOVE;
                            //printf("collision!\n");
                        }
                    }
                }
                if(event.button.button == SDL_BUTTON_LEFT) {
                    if(this->_actionState == ORGANIZATION) {
                        this->_organizationChart.handleMouseCollision(this->_graphics, xm, ym);
                    }
                    else if (this->_isCameraFloating == 0) {
                        if(this->_actionState == COMMAND) {
                            this->_actionState = NORMAL;
                            //graphics.revertToRegularCameraCoordinates();                      nwe command state
                        }
                        else {
                            this->_level.playerFireShot(this->_graphics);
                            //printf("left mouse down");
                        }
                    }
                    else if (this->_isCameraFloating == 1) {
                        if(this->_level.checkSlideCollision(xm, ym)) {
                            this->_actionState = SLIDE_MOVE;
                            //printf("collision!\n");
                        }
                    }

                    
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    rightMouseDown = 0;
                    //printf("mouse up\n");
                    this->_organizationChart.cameraStopDragging();
                    this->_rightMouseDown = 0;
                    this->_graphics.setCameraDx(0.0);
                    this->_graphics.setCameraDy(0.0);
                    if(this->_actionState == SLIDE_MOVE && this->_isCameraFloating == 0) {
                        this->_actionState = COMMAND;
                        this->_level.handleSlideRelease(xm, ym, this->_graphics);
                    }
                }
                if (event.button.button == SDL_BUTTON_LEFT) {   //left mouse up
                    if(this->_actionState == ORGANIZATION) {
                        if(this->_organizationChart.isSelected()) {
                            this->_organizationChart.handleMouseLiftCollision(this->_graphics, xm, ym);
                        }
                        this->_organizationChart.setToNotSelected();
                    }
                    if(this->_actionState == SLIDE_MOVE && this->_isCameraFloating == 1) {
                        this->_actionState = NORMAL;
                        this->_level.handleSlideRelease(xm, ym, this->_graphics);
                    }
                }
            }
            
        }
        
        
        
        
        if (input.isKeyHeld(SDL_SCANCODE_T)) {
            this->_level.playerTriggerPull(this->_graphics);
            this->_hud.cycleFireMode();
        }
        
        if(input.isKeyHeld(SDL_SCANCODE_LSHIFT) && this->_isCameraFloating == 1) {
            //rotate
            this->_isCameraRotating = 1;
        }
        else {
            this->_isCameraRotating = 0;
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
        
        //printf("%d, %d\n", rightMouseClick, rightMouseDown);
        

        
        
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
        if(input.wasKeyPressed(SDL_SCANCODE_F)) {
            this->_level.moveUnitAssignment();
            this->_organizationChart.readUnitInformation(this->_level.returnFireteam(0), this->_level.returnFireteam(1), this->_level.returnFireteam(2), this->_graphics);
        }
        
        if(input.wasKeyPressed(SDL_SCANCODE_H)){                   //this breaks it
            this->_level.moveUnitToPosition(0, 0, this->_graphics);
            //graphics.drawShape();
        }
        
        if(input.wasKeyPressed(SDL_SCANCODE_G)) {
            //graphics.playShot();
            this->_graphics.eraseDebugLines();
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
            //this->_graphics.playShot();
            this->_level.moveUnitToNearestCover(this->_graphics);
        }
        
        //change to floating camera mode
        if(input.wasKeyPressed(SDL_SCANCODE_K)) {
            this->_isCameraFloating = !this->_isCameraFloating;
        }
        
        
        
        if(this->_actionState == SLIDE_MOVE) {
            this->_level.handleSlideMovement(xm, ym, this->_graphics, 0);
        }
        else if(this->_actionState == ORGANIZATION) {
            //printf("%d, %d\n", xm - old_xm, ym - old_ym);
            this->_organizationChart.handleMouseHover(xm, ym, this->_graphics);
            this->_organizationChart.handleCameraMove(xm - old_xm, ym - old_ym);
        }
        else if (this->_actionState == COMMAND) {
            //this->_graphics.updateCommandCameraOffset(old_xm, old_ym, xm, ym);                           new command state
        }
        

        if(this->_isCameraFloating == 0) {
            this->handleMovement(inPower, input);
        }
        

        
        
        const int CURRENT_TIME_MS = SDL_GetTicks();                         //getting total elapsedTime
        int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
        
        
        LAST_UPDATE_TIME = CURRENT_TIME_MS;
        
        this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), inPower, xm, ym, old_xm, old_ym);
        
        
        
        
        
        
        

        //RENDER
        
        
        
        
        
        
        
        this->draw();
        
        
        
        FRAME_CALCULATE_TIME_END = SDL_GetTicks();
        

        
        int TOTAL_CALC_TIME = FRAME_CALCULATE_TIME_END - FRAME_CALCULATE_TIME_BEGIN;            //getting time to spare in frame
        
        if ((TOTAL_CALC_TIME) < MAX_FRAME_TIME) {
            SDL_Delay(MAX_FRAME_TIME - (TOTAL_CALC_TIME));
        }
        
        FRAME_CALCULATE_TIME_BEGIN = SDL_GetTicks();

        
        
        
        this->_graphics.updateFrameTimeIndicator(TOTAL_CALC_TIME);
        
    }
    
}

void Game::handleEvents() {
    SDL_Event event;
    
    
    
    
    
    
    
    this->handleMouseEvent();


}

void Game::handleMouseEvent() {
 
    
    
}


void Game::handleMovement(Direction &inPower, Input &input) {
    

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
                this->_graphics.moveCameraForward();
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
                this->_graphics.moveCameraBackward();
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
                this->_graphics.moveCameraRight();
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
                this->_graphics.moveCameraLeft();
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
                this->_graphics.moveCameraUpRight();
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
                this->_graphics.moveCameraUpLeft();
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
                this->_graphics.moveCameraDownRight();
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
                this->_graphics.moveCameraDownLeft();
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
                this->_graphics.stopCameraMoving();
            }
            break;
            
    }
    
    this->_level.handleUnitMovement();
}


void Game::draw() {
    this->_graphics.clear();

    
    if(this->_actionState != ORGANIZATION) {
        this->_level.draw(this->_graphics);
        this->_player.draw(this->_graphics);
        
        //this->_graphics.renderText();
        
        this->_hud.draw(this->_graphics);
        
        this->_cursor.draw(this->_graphics);
        
        
        this->_graphics.drawDebug();       //draw all debug lines
        this->_graphics.eraseDebugLines2();
        
        this->_graphics.drawCircle(640, 400, 8);
        
        this->_graphics.drawBackgroundLines();
        
        
        
        this->_debugWindow.draw(this->_graphics);
        
        
        
        
    }
    else {
        
        this->_organizationChart.draw(this->_graphics);
        
        //drawing some text
        SDL_Rect desintationRectangle;
        desintationRectangle.x = 0;
        desintationRectangle.y = 0;
        
        SDL_Surface* tmp = this->_graphics.loadText("Hello World");
        
        desintationRectangle.w = tmp->w;
        desintationRectangle.h = tmp->h;
        
        
        //desintationRectangle.w = 200;
        //desintationRectangle.h = 50;
        
        
        this->_graphics.drawText(this->_textSheet, &desintationRectangle);
        
        
    }

    
    this->_graphics.flip();
    
    
}

void Game::update(float elapsedTime, Direction &inPower, int xm, int ym, int old_xm, int old_ym) {

    
    if(this->_isCameraFloating == 0) {
        if ((this->_actionState == NORMAL) && std::abs((xm - old_xm)) < 100) {
            this->_graphics.changeAngle(-0.5*(xm - old_xm));
        }
        this->_level.setUnitAngle();
    }
    else {
        if(this->_actionState == NORMAL && this->_rightMouseDown) {
            //printf("test1\n");
            //update the camera position in graphics
            //graphics.setCameraX(xm);
            //graphics.setCameraY(ym);
            
            if(this->_isCameraRotating == 1) {
                this->_graphics.changeAngle(-0.5*(xm - old_xm));
            }
            else {
                //this will depend on the resolution of the screen
                this->_graphics.setCameraDx((xm - old_xm) / 16.0);
                this->_graphics.setCameraDy((ym - old_ym) / 16.0);
            }

        }
        //else {
        //    graphics.setCameraDx(0.0);
        //    graphics.setCameraDy(0.0);
        //}
    }
    //printf("test2\n");
    
    
    
    
    
    
    this->_graphics.update(elapsedTime);
    
    this->_level.update(elapsedTime, this->_graphics);
    
    this->_player.update(elapsedTime, xm, ym);

    this->_cursor.update(elapsedTime, ym, old_ym, xm, old_xm, this->_graphics.getCameraAngle(), this->_graphics);
    
    this->_organizationChart.update(xm, ym);
    
    
    if(this->_isCameraFloating == 0) {
        this->_level.handlePlayerCollisions(elapsedTime, this->_graphics);
    }
    
    
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

void Game::giveUnitPointerToPlayer(std::shared_ptr<Unit> unit) {
    this->_player.setPlayerFocus(unit);
}
