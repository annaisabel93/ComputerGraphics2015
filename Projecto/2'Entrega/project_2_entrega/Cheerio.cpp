//
//  project_1_entrega - Cheerio.cpp
//
#include "Game.h"
#include "Cheerio.h"

Cheerio::Cheerio() : StaticObject(){
    setRadius(0.6f);
};

Cheerio::~Cheerio(){};
void Cheerio::draw(){
    glPushMatrix();
    glTranslatef(getPosition()->getX(), getPosition()->getY(), getPosition()->getZ());
    
    if(COLISION_SPHERE){
        glColor3f(1.0f, 1.0f, 1.0f);
        glutWireSphere(getRadius(), 10, 10);
    }
    glColor3f(1.0f, 0.5f, 0.0f);
    glutSolidTorus(.2f, 0.4f, 10.0, 100.0f);
    glPopMatrix();
};
