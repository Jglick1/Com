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



//Game::Game() : _graphics(Graphics()), _level("/Users/jonahglick/Documents/Com/com_test2", this->_graphics){
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
    
    this->_actionState = NORMAL;
    
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
    
    //printf("test1\n");
    
    this->_level = Level("/Users/jonahglick/Documents/Com/com_test5", graphics);
    
    this->_organizationChart = OrganizationChart(graphics);
    
    
    //this->_hud = HUD(graphics, this->_player);
    //this->_inventory = Inventory(graphics, this->_player);
    
    //printf("test2\n");
    
    this->_hud = HUD(graphics, this->_player);
    
    this->_cursor = Cursor(graphics);
    
    int LAST_UPDATE_TIME = SDL_GetTicks();
    
    Direction inPower = NONE;
    
    this->_deltaX = 0.0;
    this->_deltaY = 0.0;

    int old_xm = 0;
    int old_ym = 0;
    
    int xm = 0;
    int ym = 0;
    
    bool wasThereAnEvent = 0;
    
    //GameState game_state = NORMAL;
    
    bool rightMouseDown = 0;
    //bool rightMouseHeld = 0;
    bool centerHold = 0;
    
    bool double_time = 0;
    
    bool rightMouseClick = 0;
    
    int FRAME_CALCULATE_TIME_BEGIN = 0;
    int FRAME_CALCULATE_TIME_END = 0;
    
    while(true) {

        
        input.beginNewFrame();
        
        old_xm = xm;
        old_ym = ym;
        SDL_GetMouseState(&xm, &ym);        //update mouse positions
        
        
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
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                printf("mouse button down\n");
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    rightMouseDown = 1;
                    if(this->_actionState == NORMAL) {
                        this->_actionState = COMMAND;
                    }
                    //printf("mouse down\n");
                    
                    
                }
                if(event.button.button == SDL_BUTTON_LEFT) {
                    if(this->_actionState == ORGANIZATION) {
                        this->_organizationChart.handleMouseCollision(graphics, xm, ym);
                    }
                    else {
                        this->_actionState = NORMAL;
                        this->_level.playerFireShot(graphics);
                        printf("left mouse down");
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
            
            
            
            //else if (event.type == SDL_MOUSEBUTTONDOWN) {
            //    if(event.button.button == SDL_BUTTON_LEFT) {
            //        //std:: cout << "x: " << this->_player.getMX() << "y: " << this->_player.getMY() << std::endl;
            //       return;
            //    }
            //}
            
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
        
        
        //printf("%d\n", rightMouseClick);
        
/*
        old_xm = xm;
        old_ym = ym;
        SDL_GetMouseState(&xm, &ym);
        */

/*
        if ((game_state == NORMAL) && std::abs((xm - old_xm)) < 100) {
            this->_level.changeAngle(-0.5*(xm - old_xm));
            //this->_level.changeAngle(-0.3*(((xm - old_xm) > 0) - ((xm - old_xm) < 0)) *std::sqrt(std::pow((xm - old_xm),2) + std::pow((ym - old_ym),2)));
        }

        */
    
        if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
            return;
        }
        
        if(input.wasKeyPressed(SDL_SCANCODE_X)) {
            //this->_level.moveUnitToPosition(480, 336);
            //this->_level.moveUnitToPosition(610, 290);
            this->_level.moveUnitToPosition(790, 400, graphics);
        }
        
        if(input.wasKeyPressed(SDL_SCANCODE_H)){                   //this breaks it
            this->_level.moveUnitToPosition(0, 0, graphics);
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
        
        if(this->_actionState == SLIDE_MOVE) {
            if(rightMouseDown == 0) {   //sliders is released
                this->_actionState = COMMAND;
                this->_level.centerSlideToZero();
                this->_level.moveUnitToSlidePosition(graphics);
                this->_level.moveUnitAngleToSlideAngle(graphics);
            }
            else {
                this->_level.handleSlideMovement(xm, ym, graphics);
            }
        }
        else if(this->_actionState == ORGANIZATION) {
            this->_organizationChart.handleMouseHover(xm, ym, graphics);
        }

        
        
        
        
        
        
        
        //printGameState(this->_gameState);
        
        
        
        
        
        /*
        else {
            this->_level.centerSlideToZero();
            //this->handleMovement(inPower, input);
        }
        */
        
        //this->_level.handleSlideMovement(xm, ym);
        
        
        
        //handle input
        //handle movement
        //render
        
        //this->_unit.setPlayerAngle(this->_level.getAngle());
        
        //this->_level.setUnitAngle();
        
        
        //if(wasThereAnEvent) {
        
        
        
        this->handleMovement(inPower, input, graphics);
        
        
        
        //}
        
        //this->_unit.moveForward();
        
        
        const int CURRENT_TIME_MS = SDL_GetTicks();                         //getting total elapsedTime
        int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
        
        //this->_graphics = graphics;
        
        LAST_UPDATE_TIME = CURRENT_TIME_MS;
        
        this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), inPower, xm, ym, old_xm, old_ym, graphics);
        

        this->draw(graphics);
        
        //LAST_UPDATE_TIME = CURRENT_TIME_MS;
        
        FRAME_CALCULATE_TIME_END = SDL_GetTicks();
        
        //if (ELAPSED_TIME_MS < MAX_FRAME_TIME) {
        //    SDL_Delay(MAX_FRAME_TIME - ELAPSED_TIME_MS);
        //}
        
        int TOTAL_CALC_TIME = FRAME_CALCULATE_TIME_END - FRAME_CALCULATE_TIME_BEGIN;            //getting time to spare in frame
        
        if ((TOTAL_CALC_TIME) < MAX_FRAME_TIME) {
            SDL_Delay(MAX_FRAME_TIME - (TOTAL_CALC_TIME));
        }
        
        FRAME_CALCULATE_TIME_BEGIN = SDL_GetTicks();
        
        
        //LAST_UPDATE_TIME = CURRENT_TIME_MS;
        
        
        //this->draw(graphics);

        
        //handle movement
        //update
        //draw
        

        
        
        
        graphics.updateFrameTimeIndicator(TOTAL_CALC_TIME);
        //printf("%d\n",TOTAL_CALC_TIME);
        
    }
    
}

void Game::handleMovement(Direction &inPower, Input &input, Graphics &graphics) {
    
    //this->_level.handleUnitMovement();
    
    //this->printDirection(inPower);
    
    
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
                //this->_level.moveBackward();
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
                //this->_level.moveForward();
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
                //this->_level.moveLeft();
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
                //this->_level.moveRight();
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
                //this->_level.moveDownLeft();
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
                //this->_level.moveDownRight();
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
                //this->_level.moveUpLeft();
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
                //this->_level.moveUpRight();
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
                //this->_level.stopMoving();
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
        
        //SDL_Rect destinationRectangle = { 0, 0, 200, 200};
        //graphics.blitSurface(this->textSheet, NULL, &destinationRectangle);
        
        //graphics.renderText();
        
        this->_hud.draw(graphics);
        
        this->_cursor.draw(graphics);
        
        graphics.drawDebug();       //draw all debug lines
        //graphics.eraseDebugLines();      //erase all stored debug lines
    }
    else {
        
        this->_organizationChart.draw(graphics);
        
        
    }

    
    
    graphics.flip();
    
    
}

void Game::update(float elapsedTime, Direction &inPower, int xm, int ym, int old_xm, int old_ym, Graphics &graphics) {
    
    /*
    std::vector<Rectangle> others;
    if((others = this->_level.checkTileCollisions(this->_player.getPlayerBoundingBox())).size() > 0) {
        //this->_player.handleTileCollisions(others);
        this->_level.handleTileCollisions(others);
        this->_unit.handleTileCollisions();
    }
     */


    //this->_unit.moveForward();
    
    
    //int mx;
    //int my;
    
    //SDL_GetMouseState(&mx, &my);
    
    //printf("%d\n",this->_gameState == NORMAL);
    
    
    if ((this->_actionState == NORMAL) && std::abs((xm - old_xm)) < 100) {


        
        graphics.changeAngle(-0.5*(xm - old_xm));
        
        
        /*
        double xdiff = (xm) - graphics.getPlayerCenterX();
        double ydiff = (ym) - graphics.getPlayerCenterY();
        
        double newAngle = 0.0;
        
        if(ydiff < 0) {
            newAngle = (-std::atan(xdiff/ydiff)*180/3.14159);
        }
        else {
            newAngle = (-std::atan(xdiff/ydiff) - 3.14159)*180/3.14159;
        }
        
        newAngle = -newAngle;
        
        if(newAngle > 180) {
            newAngle -= 360;
        }
        else if(newAngle < -180) {
            newAngle += 360;
        }
        
        graphics.setAngle(newAngle);
        */
        
        
        
        
        
    }
    else if(this->_actionState == COMMAND) {
        
        //graphics.changeAngle(-0.5*(xm - old_xm));
        
        
        //graphics.changeAngle(-180/3.14159*std::atan(((xm - old_xm) / 100.0)));
        //graphics.setAngle();
        
        
        /*
        double dy = (400 - this->_cursor.getMapY());
        double dx = (640 - this->_cursor.getMapX());
        
        printf("dx: %f,\t dy: %f\n", dx, dy);
        
        //printf("mapX: %f \t mapY: %f\n", this->_cursor.getMapX(), this->_cursor.getMapY());
        
        if(dy >= 0) {
            graphics.setAngle(180/3.14159*std::atan(dx / dy));
        }
        else {
            graphics.setAngle(180/3.14159*std::atan(dx / dy) + 180);
        }
        */
        
        
        
        
    }
    
     
    
    this->_level.setUnitAngle();
    
    graphics.update(elapsedTime);
    
    this->_level.update(elapsedTime, graphics);
    
    this->_player.update(elapsedTime, xm, ym);

    this->_cursor.update(elapsedTime, ym, old_ym, xm, old_xm, graphics.getCameraAngle(), graphics);
    
    this->_organizationChart.update(xm, ym);
    
    
                                                                //handle collisions shoudl come after update for some reason
    std::vector<Rectangle> others;
    if((others = this->_level.checkTileCollisions(this->_player.getPlayerBoundingBox())).size() > 0) {
        //this->_player.handleTileCollisions(others);
        //printf("tile collision!a");
        this->_level.handleTileCollisions(others, elapsedTime, graphics);
        
        //this->_unit.handleTileCollisions(others, elapsedTime);
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
