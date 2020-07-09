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
#include <SDL2_gfx/SDL2_gfxPrimitives.h>
#include "globals.hpp"
#include <vector>

struct SDL_Window;
struct SDL_Renderer;


class Graphics {
    
public:
    Graphics();
    Graphics(int real);
    ~Graphics();
    SDL_Surface* loadImage(const std::string &filePath);
    
    void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);
    
    void flip();
    
    void clear();
    
    SDL_Renderer* getRenderer() const;
    
    void moveMouse();
    
    void drawRect(int x, int y, int width, int height);
    void drawFilledRect(int x, int y, int width, int height);
    
    
    void drawLine(int x1, int y1, int x2, int y2);
    void drawColorLine(int x1, int y1, int x2, int y2, int color);
    
    void drawPoint(int x, int y);
    
    void renderText();
    
    SDL_Surface * loadText(const std::string &text);
    SDL_Surface * loadText(const std::string &text, Uint8 r, Uint8 g, Uint8 b);
    
    
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
    void setAngle(double angle);
    void setCameraX(double x);
    void setCameraY(double y);
    
    void drawDebug();
    void storeLineDebug(int x1, int y1, int x2, int y2, int color);
    void storeDebugCircle(double centerX, double centerY, double radius);
    void eraseDebugLines();
    
    void updateFrameTimeIndicator(int elapsedTime);
    
    void storeMapLineDebug(int x1, int y1, int x2, int y2, int color);
    
    void drawGunshotLine(int x1, int y1, int x2, int y2, int opacity);
    
    void drawShape();
    
    void drawCircle(int x, int y);
    void drawCircle(int x, int y, int radius);
    
    
    void drawPolygon(std::vector<PolygonCorner> polygonCorners);
    
    void updateCommandCameraOffset(int old_xm, int old_ym, int xm, int ym);
    
    void storeCameraCoordinates();
    void revertToRegularCameraCoordinates();
    
    double getPlayerX();
    double getPlayerY();
    
    double getCommandCameraOffsetX();
    double getCommandCameraOffsetY();
    
    bool isLineLineCollision(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
    Vector2 whereLineLineCollision(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
    double distToLine(double playerX, double playerY, double x1, double y1, double x2, double y2);
    
    void drawText(SDL_Texture* texture, SDL_Rect* destinationRectangle);
    void drawColoredText(SDL_Texture* texture, SDL_Rect* destinationRectangle, int r, int g, int b);
    
    void storeRotatedDebugCircle(double centerX, double centerY, double radius);
    
    void storeLineOnBackground(double x1, double y1, double x2, double y2, double color);
    void drawBackgroundLines();
    
    Vector2 rotatePoint(Vector2 point, Vector2 rotateAxis);
    void clearBackgroundLines();
    
    void setCameraDx(double dx);
    void setCameraDy(double dy);
    
    void eraseDebugLines2();
    
    
    
    
    
    
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
    
    double _playerX;
    double _playerY;
    
    std::vector< std::vector<int> > _debugLines;
    std::vector< std::vector<double> > _debugCircles;
    
    
    std::vector< std::vector<int> > _mapDebugLines;
    
    std::vector<int> _frameTimes;       //size is 100
    int _frameTimeIndex;
    
    Vector2 _cameraPositionStore;
    
    double _cameraCommandOffsetX;
    double _cameraCommandOffsetY;
    
    
    std::vector< std::vector<double> > _debugBackgroundLines;
    
};


#endif /* graphics_hpp */
