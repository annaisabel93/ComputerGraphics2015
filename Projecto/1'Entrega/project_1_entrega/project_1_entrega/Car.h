//
//  Car.h
//  project_1_entrega


#ifndef __project_1_entrega__Car__
#define __project_1_entrega__Car__

#include <stdio.h>

class Car{
private:
public:
    Car();
    ~Car();
    void draw(float cube_size = 2.0f);
    void update();
    void keyUp();
    void keyDown();
    void keyRight();
};

#endif /* defined(__project_1_entrega__Car__) */
