//
//  debugWindow.hpp
//  Com
//
//  Created by Jonah Glick on 7/8/20.
//  Copyright © 2020 Jonah Glick. All rights reserved.
//

#ifndef debugWindow_hpp
#define debugWindow_hpp

#include <stdio.h>
#include "rectangle.hpp"
#include "graphics.hpp"
#include "button.hpp"



class Graphics;


class DebugWindow {
public:
    DebugWindow();
    DebugWindow(Graphics &graphics);
    DebugWindow(int test);
    ~DebugWindow();
    
    void update(int mx, int my);
    void draw(Graphics &graphics);
    
    void handleMouseEvent(int mx, int my);
    
     
private:
    
    double _x, _y; //the top left corner of the window
    int _width, _height; //the window's width and height
    
    double _mouseDragOffsetX, _mouseDragOffsetY;
    

    //SDL_Texture* _textSheet; //for pringting text
    
    SDL_Texture* _textSheet_MAP_MESH;
    SDL_Texture* _textSheet_FREE_CAMERA;
    SDL_Texture* _textSheet_LINE_OF_SIGHT;
    
    SDL_Surface* _surface_MAP_MESH;
    SDL_Surface* _surface_FREE_CAMERA;
    SDL_Surface* _surface_LINE_OF_SIGHT;
    
    bool _isBeingDragged;
    
    std::vector<Button> _buttons;
};


#endif /* debugWindow_hpp */
