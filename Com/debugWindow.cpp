//
//  debugWindow.cpp
//  Com
//
//  Created by Jonah Glick on 7/8/20.
//  Copyright © 2020 Jonah Glick. All rights reserved.
//

#include "debugWindow.hpp"


//we'll just have square boxes with text next to them.
//and we cross out the boxes that we want
//this way we can do debugging in the actual game and we won't need to go deep into the code to comment certain sections off


//TODO : be able to drag the window
//          be able to click the buttons on the window
//          be able to pull up the window from in the game (by pressing v)
//          have the window actually influence what debugging tools are in use
//




DebugWindow::DebugWindow() {
printf("debug window constructor 2\n");
}

DebugWindow::~DebugWindow() {
    //cleaning up
    //we deallocate our textures using SDL_DestroyTexture
    //h ttps://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php
    
    SDL_DestroyTexture(this->_textSheet_MAP_MESH);
    SDL_DestroyTexture(this->_textSheet_FREE_CAMERA);
    SDL_DestroyTexture(this->_textSheet_LINE_OF_SIGHT);
    this->_textSheet_MAP_MESH = NULL;
    this->_textSheet_FREE_CAMERA = NULL;
    this->_textSheet_LINE_OF_SIGHT = NULL;
    
    //huh. This gets called twice for some reason
    printf("Debug Window destructor called\n");
    
    
    //for some reason this is called twice,
    
    
    if(this->_surface_MAP_MESH != NULL) {
        SDL_FreeSurface(this->_surface_MAP_MESH);
    }
    if(this->_surface_FREE_CAMERA != NULL) {
        SDL_FreeSurface(this->_surface_FREE_CAMERA);
    }
    if(this->_surface_LINE_OF_SIGHT != NULL) {
        SDL_FreeSurface(this->_surface_LINE_OF_SIGHT);
    }
    
    
}

DebugWindow::DebugWindow(int test) {
    printf("debug window constructor test called\n");
}


DebugWindow::DebugWindow(Graphics &graphics) :
_width(128),
_height(256),
_x(620.0),
_y(100.0)
{
    printf("debug window constructor 1\n");
    //the button has a location         (x, y)
    //it has a type -> "map mesh", "free camera", "line of sight", ...  (string)
    //we need to know if it is enabled      (bool)
    
    
    this->_buttons.push_back(Button(12, 12, 16, 16, MAP_MESH, 1));
    this->_buttons.push_back(Button(12, 12 + 32, 16, 16, FREE_CAMERA, 0));
    this->_buttons.push_back(Button(12, 12 + 64, 16, 16, LINE_OF_SIGHT, 0));
    
    
    //ok, probably this text printing stuff should be handled by graphics
    
    
    
    this->_surface_MAP_MESH = graphics.loadText("map mesh");
    this->_surface_FREE_CAMERA = graphics.loadText("free camera");
    this->_surface_LINE_OF_SIGHT = graphics.loadText("line of sight");
    
    
    
    
    
    this->_textSheet_MAP_MESH = SDL_CreateTextureFromSurface(graphics.getRenderer(), this->_surface_MAP_MESH);
    this->_textSheet_FREE_CAMERA = SDL_CreateTextureFromSurface(graphics.getRenderer(), this->_surface_FREE_CAMERA);
    this->_textSheet_LINE_OF_SIGHT = SDL_CreateTextureFromSurface(graphics.getRenderer(), this->_surface_LINE_OF_SIGHT);
    
    if(this->_textSheet_MAP_MESH == NULL) {
        printf("Error: Unable to load text\n");
    }
    if(this->_textSheet_FREE_CAMERA == NULL) {
        printf("Error: Unable to load text\n");
    }
    if(this->_textSheet_LINE_OF_SIGHT == NULL) {
        printf("Error: Unable to load text\n");
    }
    
    
    //this->_surface_MAP_MESH = graphics.loadText("map mesh");
    //this->_surface_FREE_CAMERA = graphics.loadText("free camera");
    //this->_surface_LINE_OF_SIGHT = graphics.loadText("line of sight");
    
    
    
    this->_isBeingDragged = 0;
    
    this->_mouseDragOffsetX = 0;
    this->_mouseDragOffsetY = 0;
    
}




void DebugWindow::update(int mx, int my) {
    if(this->_isBeingDragged == 1) {
        this->_x = mx + this->_mouseDragOffsetX;
        this->_y = my + this->_mouseDragOffsetY;
    }
}



void DebugWindow::handleMouseEvent(int mx, int my){
    //if the  mouse starts dragging the window
    
    
    
    
    
    
    //if the mouse clicks on a debugging option
    //here we would need to change something in the game class
    
    
    
    
    
}


void DebugWindow::draw(Graphics &graphics) {


    
    //so first we draw a white rectangle
    graphics.drawFilledRect(this->_x, this->_y, this->_width, this->_height);
    
    
    
    //then we draw the outline
    graphics.drawRect(this->_x, this->_y, this->_width, this->_height);
    
    
    
    //then we draw everything inside the window
    
    
    for(Button &iter : this->_buttons) {
        graphics.drawRect(this->_x + iter.getX(), this->_y + iter.getY(), 16, 16);
        if(iter.isEnabled()) {
            graphics.drawLine(this->_x + iter.getX(), this->_y + iter.getY(), this->_x + iter.getX() + iter.getWidth(), this->_y + iter.getY() + iter.getHeight());
            graphics.drawLine(this->_x + iter.getX() + iter.getWidth(), this->_y + iter.getY(), this->_x + iter.getX(), this->_y + iter.getY() + iter.getHeight());
        }
        
        //drawing some text
        //TODO: handle this in the graphics class
        //TODO: also I worry about memory leaks since we define a tmp at every frame, then never clear it
        //      maybe this should be a smart pointer. But also we probably don't need to define a desitination rectangle at every frame. We'll need to find a way for graphics to handle this.
        SDL_Rect desintationRectangle;
        switch(iter.getType()) {
            case MAP_MESH:
                desintationRectangle.x = this->_x + iter.getX() + 32;
                desintationRectangle.y = this->_y + iter.getY();
                desintationRectangle.w = (this->_surface_MAP_MESH)->w;
                desintationRectangle.h = (this->_surface_MAP_MESH)->h;
                graphics.drawColoredText(this->_textSheet_MAP_MESH, &desintationRectangle, 0, 0, 0);
                break;
                
            case FREE_CAMERA:
                desintationRectangle.x = this->_x + iter.getX() + 32;
                desintationRectangle.y = this->_y + iter.getY();
                desintationRectangle.w = (this->_surface_FREE_CAMERA)->w;
                desintationRectangle.h = (this->_surface_FREE_CAMERA)->h;
                graphics.drawColoredText(this->_textSheet_FREE_CAMERA, &desintationRectangle, 0, 0, 0);
                break;
                
            case LINE_OF_SIGHT:
                desintationRectangle.x = this->_x + iter.getX() + 32;
                desintationRectangle.y = this->_y + iter.getY();
                desintationRectangle.w = (this->_surface_LINE_OF_SIGHT)->w;
                desintationRectangle.h = (this->_surface_LINE_OF_SIGHT)->h;
                graphics.drawColoredText(this->_textSheet_LINE_OF_SIGHT, &desintationRectangle, 0, 0, 0);
                break;
        }
    }
    
    
    
}
