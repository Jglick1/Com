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
    _cameraX(0.0),
    _cameraY(0.0),
    _cameraAngle(0.0),
    _cameraDx(0.0),
    _cameraDy(0.0),
    _playerCenterX(640.0),
    _playerCenterY(400.0)
    {
    SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);
    SDL_SetWindowTitle(this->_window, "Com");
    
    this->_font = NULL;
    
    //printf("loading font\n");
    this->_font =  TTF_OpenFont("/Users/jonahglick/Documents/Com/OpenSans_Semibold.ttf", 28 );
    
    if( this->_font == NULL ) {
        printf( "Failed to load font\n");
    }
    else{
        //printf( "Sucessfully loaded Font\n");
    }
    
    //SDL_WarpMouseInWindow(this->_window, 10, 10);
    //SDL_SetWindowFullscreen(this->_window, SDL_WINDOW_FULLSCREEN);
    
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
    std::string aMessage = "Hello World";
    SDL_Color textColor = { 0, 0, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Solid( this->_font, aMessage.c_str(), textColor );
    SDL_CreateTextureFromSurface( this->_renderer, textSurface );
    SDL_FreeSurface( textSurface );
    
}
 */

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
    
    //printf("%f, %f\n", this->_cameraDx, this->_cameraDy);
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
    for(std::vector<int> line : this->_debugLines) {
        if(!(line[4] == 0)) {
            SDL_SetRenderDrawColor(this->_renderer, 255, 0, 0, 255);
        }
           
        SDL_RenderDrawLine(this->_renderer, line[0], line[1], line[2], line[3]); // x1, y1, x2, y2
        
        SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255); //back to black
        
    }

}


void Graphics::storeLineDebug(int x1, int y1, int x2, int y2, int color) {
    std::vector<int> line = {x1, y1, x2, y2, color};
    
    this->_debugLines.push_back(line);
}

void Graphics::eraseDebugLines() {
    
    this->_debugLines.clear();
    
}
