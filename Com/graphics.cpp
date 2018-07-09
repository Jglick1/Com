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
#include "globals.hpp"

Graphics::Graphics() {
    SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);
    SDL_SetWindowTitle(this->_window, "Com");
    
    //SDL_WarpMouseInWindow(this->_window, 10, 10);
    //SDL_SetWindowFullscreen(this->_window, SDL_WINDOW_FULLSCREEN);
    
}

Graphics::~Graphics() {
    SDL_DestroyWindow(this->_window);
    SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
    if(this->_spriteSheets.count(filePath) == 0) {
        this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
    }
    return this->_spriteSheets[filePath];
}


void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
    
    SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
    
    
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
