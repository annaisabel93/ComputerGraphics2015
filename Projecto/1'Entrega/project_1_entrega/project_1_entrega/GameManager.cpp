//
//  project_1_entrega - GameManager.cpp
//

#include "Game.h"
#include "GameManager.h"

#include "Car.h"
#include "Track.h"
#include "Cheerio.h"
#include "Orange.h"
#include "Butter.h"

GameManager::GameManager(){
    logger.debug("GameManager::GameManager()");
    // init time tracking
    _previous_time = 0, _current_time = 0;
    // init vector of gameobjects, dynamic and static
    std::vector<DynamicObject *> _dynamic_objects;
    std::vector<StaticObject *> _static_objects;
    
    Car * car = new Car();
    this->_dynamic_objects.push_back(car);
    
    Track * track = new Track(32, 0.4, 0.6);
    this->_static_objects.push_back(track);
    Cheerio * cheerio = new Cheerio();
    this->_static_objects.push_back(cheerio);
    Orange * orange = new Orange();
    this->_dynamic_objects.push_back(orange);
    Butter * butter = new Butter();
    this->_static_objects.push_back(butter);
};
GameManager::~GameManager(){logger.debug("GameManager::~GameManager()");};

//  ------------------------------------------------------------ keyPress()
//  game manager wrapper to handle all objects keyPress
void GameManager::keyPress(int key){
    logger.debug("GameManager::keyPress()");
    for(DynamicObject * obj: _dynamic_objects){
        obj->keyPress(key);
    }
};

//  ---------------------------------------------------------- keyRelease()
//  game manager wrapper to handle all objects keyRelease
void GameManager::keyRelease(int key){
    logger.debug("GameManager::keyRelease()");
    for(DynamicObject * obj: _dynamic_objects){
        obj->keyRelease(key);
    }
};

//  ------------------------------------------------------------- drawAll()
//  Method that handles the drawing of all objects in the display
void GameManager::drawAll(){
    logger.debug("GameManager::drawAll()");
    for(GameObject * obj: _dynamic_objects){
        obj->draw();
    }
    for(GameObject * obj: _static_objects){
        obj->draw();
    }
};

//  ----------------------------------------------------------- updateAll()
//  Method that handle all the updates, calculations and what
//  not of each object in the display
void GameManager::updateAll(){
    logger.debug("GameManager::updateAll()");
    _current_time = glutGet(GLUT_ELAPSED_TIME);
    for(GameObject * obj : _dynamic_objects){
        obj->update(_current_time - _previous_time);
    }
    _previous_time = glutGet(GLUT_ELAPSED_TIME);
};

//  ----------------------------------------------------------- onReshape()
//  Custom reshape function used when "glutReshapeFunc"
//  triggers an event. This handles the change in screen size
void GameManager::onReshape(GLsizei w, GLsizei h){
    logger.debug("GameManager::onReshape()");
    // define size of the viewport
    // args: x, y, weight, height
    // x and y are measure from the bottom left corner of the screen
    // weight and height are the actual size of the viewport
    float xmin = -1., xmax = 1., ymin = -1., ymax = 1.;
    float ratio = (xmax - xmin) / (ymax - ymin);
    float aspect = (float) w / h;
    if (aspect > ratio)
        glViewport((w-h*ratio)/2, 0, h*ratio, h);
    else
        glViewport(0, (h-w/ratio)/2, w, w/ratio);
    // changes the pointer for which Matrix we want to work on. GL_PROJECTION
    glMatrixMode(GL_PROJECTION);
    // puts the Identity Matrix as the top matrix of the stack GL_PROJECTION
    glLoadIdentity();
    // changes back the pointer to the GL_MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    // and set's the top matrix of that stack to be the Identity Matrix
    glLoadIdentity();
    // define Ortho2d projection
    gluOrtho2D(xmin, xmax, ymin, ymax);
};

//  ----------------------------------------------------------- onDisplay()
//  Custom display function used when "glutDisplayFunc"
//  triggers an event. This handles the drawing of the scenes
void GameManager::onDisplay(){
    logger.debug("GameManager::onDisplay()");
    // Fundamental steps
    // #1 Clear all buffers
    // #2 Draw all lines, dots and polygons
    // #3 Force drawing
    // set color to black when buffer get clean
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // actually cleans buffer (Color buffer)
    glClear((ENABLE_DEPTH) ? GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT: GL_COLOR_BUFFER_BIT);
    
    // enable depth
    if(ENABLE_DEPTH){ glEnable(GL_DEPTH_TEST); } else { glDisable(GL_DEPTH_TEST); }
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // gluPerspective(fov, aspect_ration, near_plane, far_plane)
    // Field of View (degrees). the amount of "zoom". eg: 90°-extra wide|30°-zoomed in
    // Aspect Ratio. Width/Height eg: 4/3 or 16/9
    // Near clipping plane.
    // Far clipping plane.
    gluPerspective(P_FOV, P_ASPECT_RATIO, P_NEAR, P_FAR);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(POSCAM->getX(), POSCAM->getY(), POSCAM->getZ(), // the position of your camera, in world space
              0.0f, 0.0f, 0.0f,  // where is the camera pointing to
              AXIS->getX(), AXIS->getY(), AXIS->getZ()); // which axis is the up
    // draw all objects
    gm.drawAll();
    // force the execution of the GL commands
    (ENABLE_DOUBLE_BUFFER) ? glutSwapBuffers() : glFlush();
    
};

//  -------------------------------------------------------------- onIdle()
//  Custom keyboard function used when "glutIdleFunc" triggers
//  an event. This runs all logic inside if glut has no events
//  to run
void GameManager::onIdle(){
    gm.updateAll();
    glutPostRedisplay();
}

//  ---------------------------------------------------------- onKeyboard()
//  Custom keyboard function used when "glutKeyboardFunc"
//  triggers an event. This handles the keyboardPress
void GameManager::onKeyboard(unsigned char key, int x, int y){
    // change to wireframe
    if (key == 'A' || key == 'a'){
        int polygonMode;
        glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
        if (polygonMode == GL_LINE){
            logger.debug("Changin to GL_FILL (Polygons)");
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else{
            logger.debug("Changin to GL_LINE (Wireframe)");
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        glutPostRedisplay();
    }
    else{
        switch(key){
            case '1' : POSCAM->setX(POSCAM->getX() + ROTATION_SPEED);
                    logger.info("Moving +X");
                break;
            case '2' : POSCAM->setX(POSCAM->getX() + ROTATION_SPEED); logger.info("Moving +Y"); break;
            case '3' : POSCAM->setZ(POSCAM->getZ()+ ROTATION_SPEED); logger.info("Moving +Z"); break;
            case '8' : POSCAM->setX(POSCAM->getX() - ROTATION_SPEED); logger.info("Moving -X"); break;
            case '9' : POSCAM->setY(POSCAM->getY() - ROTATION_SPEED); logger.info("Moving -Y"); break;
            case '0' : POSCAM->setZ(POSCAM->getZ()- ROTATION_SPEED); logger.info("Moving -Z"); break;
            case 'X' : AXIS->setX(1.0f); AXIS->setY(0.0f); AXIS->setZ(0.0f); break;
            case 'Y' : AXIS->setX(0.0f); AXIS->setY(1.0f); AXIS->setZ(0.0f); break;
            case 'Z' : AXIS->setX(0.0f); AXIS->setY(0.0f); AXIS->setZ(1.0f); break;
        }
        std::cout << "Key press: "<< key << std::endl;
        glutPostRedisplay();
    }
};

//  ------------------------------------------------------- onSpecialKeys()
//  Custom keyboard function used when "glutSpecialFunc"
//  triggers an event. This handles the special keys like
//  F1, Esc, Left arrow, Right Arrow...
void GameManager::onSpecialKeys(int key, int x, int y){
    gm.keyPress(key);
}

//  ----------------------------------------------------- onSpecialUpKeys()
//  Custom keyboard function used when "glutSpecialUpFunc"
//  triggers an event.
void GameManager::onSpecialKeysUp(int key, int x, int y){
    gm.keyRelease(key);
}


