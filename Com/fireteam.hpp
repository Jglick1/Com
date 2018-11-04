//
//  fireteam.hpp
//  Com
//
//  Created by Jonah Glick on 7/15/18.
//  Copyright © 2018 Jonah Glick. All rights reserved.
//

#ifndef fireteam_hpp
#define fireteam_hpp

//https://stackoverflow.com/questions/1361139/how-to-avoid-memory-leaks-when-using-a-vector-of-pointers-to-dynamically-allocat

#include <vector>
#include "unit.hpp"
#include "controlSlide.hpp"
//#include "squad.hpp"
#include "graphics.hpp"

class Fireteam {
public:
    Fireteam();
    Fireteam(Graphics &graphics, bool allied);
    ~Fireteam();
    void draw(Graphics &graphics);
    void update(int elapsedTime, Graphics &graphics);
    //slide things
    
    
    //void addUnit(Unit * unit);     //adding node to tree
    //void removeUnit(Unit * unit);
    
    void addUnit(std::shared_ptr<Unit> unit);     //adding node to tree
    void removeUnit(std::shared_ptr<Unit> unit);
    
    
private:
    int _id;
    int _numUnits;
    //std::vector<Unit *> _units;
    
    std::vector< std::shared_ptr<Unit> > _units;
    
    //std::unique_ptr<int> _units1;
    
    
    //level of danger. cover or something
    
    ControlSlide _controlSlide;
    
    //Squad * _parentSquad
    
    
    
    
};


#endif /* fireteam_hpp */
