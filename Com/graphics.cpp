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


Graphics::Graphics() {
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
