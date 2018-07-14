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
    
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    
    TTF_Font* _font;
    
    Mix_Chunk* _shot = NULL;
    Mix_Music *gMusic = NULL;
    
    std::map<std::string, SDL_Surface*> _spriteSheets;
    
    std::map<std::string, SDL_Surface*> _textSheets;
    
    
};




#endif /* graphics_hpp */
