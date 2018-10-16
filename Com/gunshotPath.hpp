//
//  gunshotPath.hpp
//  Com
//
//  Created by Jonah Glick on 10/15/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef gunshotPath_hpp
#define gunshotPath_hpp

#include "graphics.hpp"

class GunshotPath {
  
public:
    GunshotPath();
    GunshotPath(Graphics &graphics, int beginX, int beginY, int endX, int endY, int lifeTime);
    bool update(int elapsedTime);
    void draw(Graphics &graphics);
    

private:
    int _beginX, _beginY;
    int _endX, _endY;
    int _lifeTime;
    int _age;
    
    
    
};

#endif /* gunshotPath_hpp */
