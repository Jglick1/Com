//
//  graphics.hpp
//  Com
//
//  Created by Jonah Glick on 6/20/18.
//
//

#ifndef graphics_hpp
#define graphics_hpp

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>
//#include "globals.hpp"

struct SDL_Window;
struct SDL_Renderer;


class Graphics {
    
public:
    Graphics();
    ~Graphics();
    SDL_Surface* loadImage(const std::string &filePath);
    
    void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);
    
    void flip();
    
    void clear();
    
    SDL_Renderer* getRenderer() const;
    
    void moveMouse();
    
    void drawRect(int x, int y, int width, int height);
    
    void drawLine(int x1, int y1, int x2, int y2);
    
    void drawPoint(int x, int y);
    
    void renderText();
    
    SDL_Surface * loadText(const std::string &text);
    
    void loadSound();
    
    void playShot();
    
    void resolutionTest();
    
    double getCameraX();
    double getCameraY();
    double getCameraAngle();
    
    int getPlayerCenterX();
    int getPlayerCenterY();
    
    double getCameraDx();
    double getCameraDy();
    
    void moveCameraForward();
    void moveCameraBackward();
    void moveCameraRight();
    void moveCameraLeft();
    void stopCameraMoving();
    
    void moveCameraUpRight();
    void moveCameraUpLeft();
    void moveCameraDownRight();
    void moveCameraDownLeft();
    
    void update(int elapsedTime);
    
    void changeAngle(double angle);
    void setCameraX(double x);
    void setCameraY(double y);
    
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    
    TTF_Font* _font;
    
    Mix_Chunk* _shot = NULL;
    Mix_Music *gMusic = NULL;
    
    std::map<std::string, SDL_Surface*> _spriteSheets;
    
    std::map<std::string, SDL_Surface*> _textSheets;
    
    double _cameraX;
    double _cameraY;
    double _cameraAngle;
    double _cameraDx;
    double _cameraDy;
    
    int _playerCenterX;
    int _playerCenterY;
    
    
};


#endif /* graphics_hpp */
