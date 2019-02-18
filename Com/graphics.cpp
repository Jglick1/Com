//
//  graphics.cpp
//  Com
//
//  Created by Jonah Glick on 6/20/18.
//
//

#include "graphics.hpp"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>
#include "globals.hpp"
#include <math.h>

namespace player_constants {
    const float WALK_SPEED = 0.2f;
}


Graphics::Graphics() :
    _cameraX(0.01),
    _cameraY(0.01),
    _cameraAngle(0.01),
    _cameraDx(0.0),
    _cameraDy(0.0),
    _playerCenterX(640.0),
    _playerCenterY(400.0),
    _frameTimeIndex(0),
    _cameraPositionStore(Vector2(0.0,0.0)),
    _playerX(640.0),
    _playerY(400.0),
    _cameraCommandOffsetX(0.0),
    _cameraCommandOffsetY(0.0)
    {
    SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);
        
        
    //SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI, &this->_window, &this->_renderer);
        
        
        
        
        
    SDL_SetWindowTitle(this->_window, "Com");
    
    this->_font = NULL;
    
    //printf("loading font\n");
    //this->_font =  TTF_OpenFont("/Users/jonahglick/Documents/Com/Moms_typewriter.ttf", 12); //12 sets the size of the font
    this->_font =  TTF_OpenFont("/Users/jonahglick/Documents/Com/OpenSans_Semibold.ttf", 12);
    
        
        
    if( this->_font == NULL ) {
        printf( "Failed to load font\n");
    }
    else{
        //printf( "Sucessfully loaded Font\n");
    }
    
    //SDL_WarpMouseInWindow(this->_window, 10, 10);
    //SDL_SetWindowFullscreen(this->_window, SDL_WINDOW_FULLSCREEN);
        
        for(int i = 0; i < 100; i++) {
            this->_frameTimes.push_back(0);
        }
        
        
        
        
        
        
        SDL_SetRenderDrawBlendMode(this->_renderer, SDL_BLENDMODE_BLEND);   //for the transparent gunshots
}

Graphics::~Graphics() {
    
    //TTF_CloseFont(this->_font);
    
    //printf("graphics destructor\n");
    
    SDL_DestroyWindow(this->_window);
    SDL_DestroyRenderer(this->_renderer);
    
    Mix_FreeChunk(this->_shot);
    this->_shot = NULL;

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
    if(this->_spriteSheets.count(filePath) == 0) {
        this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
    }
    return this->_spriteSheets[filePath];
}

SDL_Surface * Graphics::loadText(const std::string &text) {
    SDL_Color textColor = { 0, 0, 0 };
    if(this->_textSheets.count(text) == 0) {
        this->_textSheets[text] = TTF_RenderText_Solid(this->_font, text.c_str(), textColor);
    }
    return this->_textSheets[text];
}

void Graphics::loadSound() {
    this->_shot = Mix_LoadWAV("/Users/jonahglick/Documents/Com/kar98_fp.wav");
    if(this->_shot == NULL) {
        printf( "Failed to load shot sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
    }
}

void Graphics::playShot() {
    Mix_PlayChannel( -1, this->_shot, 0 );
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
    
    SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
    
    
}

/*
void Graphics::renderText() {
    std::string aMessage = "Hello World1";
    SDL_Color textColor = { 0, 0, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Solid( this->_font, aMessage.c_str(), textColor );
    SDL_CreateTextureFromSurface( this->_renderer, textSurface );
    SDL_FreeSurface( textSurface );
    
}
*/

void Graphics::drawText(SDL_Texture* texture, SDL_Rect* destinationRectangle) {
    
    SDL_RenderCopy(this->_renderer, texture, NULL, destinationRectangle);
    
}

void Graphics::flip() {
    SDL_RenderPresent(this->_renderer);
}

void Graphics::clear() {
    SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
    return this->_renderer;
}

void Graphics::moveMouse() {
    SDL_WarpMouseInWindow(this->_window, 640, 100);
}

void Graphics::drawRect(int x, int y, int width, int height) {
    SDL_Rect destRect = {x, y, width, height};
    SDL_RenderDrawRect(this->_renderer, &destRect);
}

void Graphics::drawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(this->_renderer, x1, y1, x2, y2);
}

void Graphics::drawPoint(int x, int y) {
    SDL_RenderDrawPoint(this->_renderer, x, y);
}

void Graphics::resolutionTest() {
    SDL_RenderSetLogicalSize(this->_renderer, 1280, 800);
}

double Graphics::getCameraX() {
    return this->_cameraX;
}

double Graphics::getCameraY() {
    return this->_cameraY;
}

double Graphics::getCameraDx() {
    return this->_cameraDx;
}

double Graphics::getCameraDy() {
    return this->_cameraDy;
}

int Graphics::getPlayerCenterX() {
    return this->_playerCenterX;
}

int Graphics::getPlayerCenterY() {
    return this->_playerCenterY;
}

double Graphics::getCameraAngle() {
    return this->_cameraAngle;
}

void Graphics::update(int elapsedTime) {
    this->_cameraX += this->_cameraDx * elapsedTime;
    this->_cameraY += this->_cameraDy * elapsedTime;
    
    this->_playerX = this->_cameraX + this->_playerCenterX + this->_cameraCommandOffsetX;
    this->_playerY = this->_cameraY + this->_playerCenterY + this->_cameraCommandOffsetY;
    
    
    //printf("%f, %f\n", this->_playerX, this->_playerY);
}

void Graphics::moveCameraForward() {
    this->_cameraDx = player_constants::WALK_SPEED * std::sin(this->_cameraAngle * 3.14159 / 180);
    this->_cameraDy = player_constants::WALK_SPEED * std::cos(this->_cameraAngle * 3.14159 / 180);
}

void Graphics::moveCameraBackward() {
    this->_cameraDx = -player_constants::WALK_SPEED * std::sin(this->_cameraAngle * 3.14159 / 180);
    this->_cameraDy = -player_constants::WALK_SPEED * std::cos(this->_cameraAngle * 3.14159 / 180);
}

void Graphics::moveCameraRight() {
    this->_cameraDx = -player_constants::WALK_SPEED * std::cos(this->_cameraAngle * 3.14159 / 180);
    this->_cameraDy = player_constants::WALK_SPEED * std::sin(this->_cameraAngle * 3.14159 / 180);
}

void Graphics::moveCameraLeft() {
    this->_cameraDx = player_constants::WALK_SPEED * std::cos(this->_cameraAngle * 3.14159 / 180);
    this->_cameraDy = -player_constants::WALK_SPEED * std::sin(this->_cameraAngle * 3.14159 / 180);
}

void Graphics::stopCameraMoving() {
    this->_cameraDx = 0.0f;
    this->_cameraDy = 0.0f;
}

void Graphics::moveCameraUpRight() {
    this->_cameraDx = player_constants::WALK_SPEED * std::sin(this->_cameraAngle * 3.14159 / 180)/1.41421 + -player_constants::WALK_SPEED * std::cos(this->_cameraAngle * 3.14159 / 180)/1.41421;
    this->_cameraDy = player_constants::WALK_SPEED * std::cos(this->_cameraAngle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::sin(this->_cameraAngle * 3.14159 / 180)/1.41421;
}

void Graphics::moveCameraUpLeft() {
    this->_cameraDx = player_constants::WALK_SPEED * std::sin(this->_cameraAngle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::cos(this->_cameraAngle * 3.14159 / 180)/1.41421;
    this->_cameraDy = player_constants::WALK_SPEED * std::cos(this->_cameraAngle * 3.14159 / 180)/1.41421 + -player_constants::WALK_SPEED * std::sin(this->_cameraAngle * 3.14159 / 180)/1.41421;
}

void Graphics::moveCameraDownRight() {
    this->_cameraDx = -player_constants::WALK_SPEED * std::sin(this->_cameraAngle * 3.14159 / 180)/1.41421 + -player_constants::WALK_SPEED * std::cos(this->_cameraAngle * 3.14159 / 180)/1.41421;
    this->_cameraDy = -player_constants::WALK_SPEED * std::cos(this->_cameraAngle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::sin(this->_cameraAngle * 3.14159 / 180)/1.41421;
}

void Graphics::moveCameraDownLeft() {
    this->_cameraDx = -player_constants::WALK_SPEED * std::sin(this->_cameraAngle * 3.14159 / 180)/1.41421 + player_constants::WALK_SPEED * std::cos(this->_cameraAngle * 3.14159 / 180)/1.41421;
    this->_cameraDy = -player_constants::WALK_SPEED * std::cos(this->_cameraAngle * 3.14159 / 180)/1.41421 - player_constants::WALK_SPEED * std::sin(this->_cameraAngle * 3.14159 / 180)/1.41421;
}

void Graphics::changeAngle(double angle) {
    this->_cameraAngle += angle;
    if(this->_cameraAngle > 180) {
        this->_cameraAngle = this->_cameraAngle - 360;
    }
    else if(this->_cameraAngle < -180) {
        this->_cameraAngle = this->_cameraAngle + 360;
    }
}

void Graphics::setAngle(double angle) {
    this->_cameraAngle = angle;
}

void Graphics::setCameraX(double x) {
    this->_cameraX = x;
}

void Graphics::setCameraY(double y) {
    this->_cameraY = y;
}

void Graphics::drawDebug() {
    
    
    drawShape();
    
    
    //draw debug lines
    for(std::vector<int> line : this->_debugLines) {
        if(!(line[4] == 0)) {
            SDL_SetRenderDrawColor(this->_renderer, 255, 0, 0, 255);
        }
           
        SDL_RenderDrawLine(this->_renderer, line[0], line[1], line[2], line[3]); // x1, y1, x2, y2
        
        SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255); //back to black
        
    }
    
    
    //draw map debug lines
    for(std::vector<int> line : this->_mapDebugLines) {
        if(!(line[4] == 0)) {
            SDL_SetRenderDrawColor(this->_renderer, 255, 0, 0, 255);
        }
        
        SDL_RenderDrawLine(this->_renderer, line[0] + this->_cameraX, line[1] + this->_cameraY, line[2] + this->_cameraX, line[3] + this->_cameraY); // x1, y1, x2, y2
        
        SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255); //back to black
        
    }
    
    for(std::vector<double> circle : this->_debugCircles) {
        drawCircle(std::round(circle[0]), std::round(circle[1]), std::round(circle[2]));
        
    }
    
    
    
    //drawFrameTimes
    
    
    SDL_SetRenderDrawColor(this->_renderer, 255, 0, 0, 255);
    for(int i = 0; i < 100; i++) {      //max frame time at 30 FPS. 1000 / 30
        drawLine(i, 100, i, this->_frameTimes.at(i)/33.33*100);
    }
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
    
    
    
    //drawShape();
    
}


void Graphics::storeLineDebug(int x1, int y1, int x2, int y2, int color) {
    std::vector<int> line = {x1, y1, x2, y2, color};
    
    this->_debugLines.push_back(line);
}

void Graphics::eraseDebugLines() {
    
    this->_debugLines.clear();
    this->_mapDebugLines.clear();
    this->_debugCircles.clear();
    
}

void Graphics::updateFrameTimeIndicator(int elapsedTime) {
    
    this->_frameTimes[this->_frameTimeIndex] = elapsedTime;
    
    
    this->_frameTimeIndex = (this->_frameTimeIndex+1) % 100;
    //printf("%d\n",this->_frameTimeIndex);
}

void Graphics::storeMapLineDebug(int x1, int y1, int x2, int y2, int color) {
    std::vector<int> line = {x1, y1, x2, y2, color};
    
    this->_mapDebugLines.push_back(line);
}

void Graphics::drawGunshotLine(int x1, int y1, int x2, int y2, int opacity) {
    //SDL_SetRenderDrawBlendMode(this->_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, opacity);
    //printf("opacity: %d\n", opacity);
    
    SDL_RenderDrawLine(this->_renderer, x1, y1, x2, y2);
    
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
    
}

void Graphics::drawShape() {
    
    
    /*
    const Sint16 vx[3] = {100, 200, 300};
    const Sint16 vy[3] = {100, 200, 100};
    

    
    //Uint32 color = 0xff00ff00;    //green               //0xAABBGGRR
    //Uint32 color = 0xffffff;
    
    Uint32 color = 0xfe00ff00; //for some reason an ff opacity messes up the opacity of the gunshots
    
    filledPolygonColor(this->_renderer, vx, vy, 3, color);
    */
    
    
    
    
    
    //filledPolygonColor(this->_renderer, pVx, pVy, 3, 0xff0000ff);
    
    
    /*
    const Sint16 vx1[4] = {600, 800, 800, 600};
    const Sint16 vy1[4] = {600, 600, 800, 800};
    
    color = 0xfe00ff00;
    
    filledPolygonColor(this->_renderer, vx1, vy1, 4, color);
    */
    
    
}

void Graphics::drawCircle(int x, int y) {
    circleColor(this->_renderer, x, y, 10, 0xfe0000ff); //red
    //circleColor(this->_renderer, x, y, 10, 0xfeFF0000);
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
}


void Graphics::drawCircle(int x, int y, int radius) {
    //circleColor(this->_renderer, x, y, radius, 0xfe00ff00);
    circleColor(this->_renderer, x, y, radius, 0xfe0000ff);
    //circleColor(this->_renderer, x, y, 10, 0xfeFF0000);
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
}



void Graphics::drawPolygon(std::vector<PolygonCorner> polygonCorners) {
    int numCorners = polygonCorners.size();
    
    Sint16 vx[numCorners];// = {600, 800, 800, 600};
    Sint16 vy[numCorners];// = {600, 600, 800, 800};
    Uint32 color = 0xfe00ff00;
    
    for(int i = 0; i < numCorners; i++) {
        vx[i] = polygonCorners.at(i).x + this->_cameraX;
        vy[i] = polygonCorners.at(i).y + this->_cameraY;
        //vx[i] = polygonCorners.at(i).x;
        //vy[i] = polygonCorners.at(i).y;
    }
    
    filledPolygonColor(this->_renderer, vx, vy, numCorners, color);
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
}


void Graphics::updateCommandCameraOffset(int old_xm, int old_ym, int xm, int ym) {
    
    //R(a - b) + b
    // Cos x    - Sin x
    // Sin x    Cos x
    
    /*
    double ax = (old_xm - xm);
    double ay = (old_ym - ym);
    
    
    
    
    
    double bx = 640;
    double by = 400;
    
    double x = std::cos(-this->_cameraAngle  * 3.14159 / 180) * (ax - bx) - std::sin(-this->_cameraAngle * 3.14159 / 180) * (ay - by) + bx;
    double y = std::sin(-this->_cameraAngle  * 3.14159 / 180) * (ax - bx) + std::cos(-this->_cameraAngle * 3.14159 / 180) * (ay - by) + by;
    
    
    this->_cameraCommandOffsetX += x;
    this->_cameraCommandOffsetY += y;
    
    this->_cameraX += x;
    this->_cameraY += y;
    */
    
    
    
    
    double x = std::cos(-this->_cameraAngle  * 3.14159 / 180) * (old_xm - xm) - std::sin(-this->_cameraAngle * 3.14159 / 180) * (old_ym - ym);
    double y = std::sin(-this->_cameraAngle  * 3.14159 / 180) * (old_xm - xm) + std::cos(-this->_cameraAngle * 3.14159 / 180) * (old_ym - ym);
    
    
    
    
    this->_cameraCommandOffsetX += x;
    this->_cameraCommandOffsetY += y;
    
    this->_cameraX += x;
    this->_cameraY += y;
    
    
    
    

    /*
    this->_cameraCommandOffsetX += (old_xm - xm);
    this->_cameraCommandOffsetY += (old_ym - ym);
    
    this->_cameraX += (old_xm - xm);
    this->_cameraY += (old_ym - ym);
    */
    

}

void Graphics::storeCameraCoordinates() {
    this->_cameraPositionStore.x = this->_cameraX;
    this->_cameraPositionStore.y = this->_cameraY;
}

void Graphics::revertToRegularCameraCoordinates() {
    this->_cameraCommandOffsetX = 0.0;
    this->_cameraCommandOffsetY = 0.0;
    
    this->_cameraX = 0.0;
    this->_cameraY = 0.0;
    
}

double Graphics::getPlayerX() {
    return this->_playerX;
}

double Graphics::getPlayerY() {
    return this->_playerY;
}

double Graphics::getCommandCameraOffsetX() {
    return this->_cameraCommandOffsetX;
}

double Graphics::getCommandCameraOffsetY() {
    return this->_cameraCommandOffsetY;
}

void Graphics::storeDebugCircle(double centerX, double centerY, double radius) {
    std::vector<double> tmp = {centerX, centerY, radius};
    
    this->_debugCircles.push_back(tmp);
    
    
}

void Graphics::storeRotatedDebugCircle(double centerX, double centerY, double radius) {
    
    /*
    float posx = graphics.getCameraX() + this->_staticx;
    float posy = graphics.getCameraY() + this->_staticy;
    
    
    int playerx = graphics.getPlayerCenterX();
    int playery = graphics.getPlayerCenterY();
    
    double playerAngle = graphics.getCameraAngle();
    
    double renderX = playerx-8 + ( (posx)- playerx+8)* std::cos(playerAngle*3.14159/180) - ((posy)-playery+12) * std::sin(playerAngle*3.14159/180);
    double renderY = playery-12 + ( ((posy)-playery+12) * std::cos(playerAngle*3.14159/180) + ((posx)-playerx+8) * std::sin(playerAngle*3.14159/180));
    */
    
    
    
    
    double posX = centerX + this->_cameraX;
    double posY = centerY + this->_cameraY;
    
    int playerx = this->_playerCenterX;
    int playery = this->_playerCenterY;
    
    //printf("%d, %d\n", this->_playerCenterX, this->_playerCenterY);
    
    //this->_camerax = 640-75 + ( (posx)-640+75)* std::cos(playerAngle*3.14159/180) - ((posy)-400+12) * std::sin(playerAngle*3.14159/180);
    //this->_cameray = 400-12 + ( ((posy)-400+12) * std::cos(playerAngle*3.14159/180) + ((posx)-640+75) * std::sin(playerAngle*3.14159/180));
    
    double newCenterX = playerx + ( (posX)- playerx)* std::cos(this->_cameraAngle*3.14159/180) - ((posY)-playery) * std::sin(this->_cameraAngle*3.14159/180);
    double newCenterY = playery + ( ((posY)-playery) * std::cos(this->_cameraAngle*3.14159/180) + ((posX)-playerx) * std::sin(this->_cameraAngle*3.14159/180));
    
    
    
    storeDebugCircle(newCenterX, newCenterY, radius);
    
    
    
}


//http://www.jeffreythompson.org/collision-detection/poly-line.php
bool Graphics::isLineLineCollision(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    
    double uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    double uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    
    return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
    
    
}

Vector2 Graphics::whereLineLineCollision(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    
    
    double m1 = (y2 - y1) / (x2 - x1);
    double b1 = - m1 * x1 + y1;
    double m2 = (y4 - y3) / (x4 - x3);
    double b2 = - m2 * x3 + y3;
    
    double x = (-b1 + b2) / (m1 - m2);
    double y = m1 * x + b1;
    
    return Vector2(x, y);
    
}

double Graphics::distToLine(double playerX, double playerY, double x1, double y1, double x2, double y2) {
    double m = (y2 - y1) / (x2 - x1);
    
    
    //y = (1/m) x + b
    double bPerp = playerY - (1/m) * playerX;
    

    
    
    return 0.0;
    
}
