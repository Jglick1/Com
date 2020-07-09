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
#include "graph.hpp"

class Unit;

class Fireteam {
public:
    Fireteam();
    Fireteam(Graphics &graphics, bool allied, Vector2 spawnPoint);
    ~Fireteam();
    void draw(Graphics &graphics);
    void update(int elapsedTime, Graphics &graphics);
    //slide things
    
    
    //void addUnit(Unit * unit);     //adding node to tree
    //void removeUnit(Unit * unit);
    
    void addUnit(std::shared_ptr<Unit> unit);     //adding node to tree
    void removeUnit(std::shared_ptr<Unit> unit);

    int checkSlideCollision(int xm, int ym);
    void handleSlideMovement(int xm, int ym, double angle, float levelx, float levely, Graphics &graphics);
    
    void centerSlideToZero();
    bool isSelected();
    
    void moveToSlidePosition(Graph & graph, Graphics &graphics);
    void moveToSlidePosition(Graph & graph, Graphics &graphics, std::vector<Vector2> positions);
    bool isCenterSelected();
    void moveToSlideAngle();
    
    Vector2 getSlideCenter();
    double getSlideAngle();
    
    Vector2 checkUnitCollision(double shotBeginX, double shotBeginY, double shotEndX, double shotEndY);
    bool linePoint(double x1, double y1, double x2, double y2, double px, double py);
    double dist(double x1, double y1, double x2, double y2);
    
    int getSize();
    
    std::shared_ptr<Unit> getUnitPointer();
    
    void checkIfPlayerInLOS(Graphics &graphics);
    void updateUnitFocus(Fireteam &fireteamToBeChecked);
    
    
    std::shared_ptr<Unit> getPointerToAUnit(int unitIndex);
    
    
private:
    int _id;
    int _numUnits;
    
    std::vector< std::shared_ptr<Unit> > _units;
    

    //level of danger. cover or something
    ControlSlide _controlSlide;
    
    //Squad * _parentSquad
    
    double _playerInLOSCheckTime;
    double _timeSinceLastLOSCheck;
    
};



#endif /* fireteam_hpp */
