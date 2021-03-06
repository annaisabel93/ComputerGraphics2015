//
//  project_3_entrega - PerspectiveCamera.h
//
#ifndef __PERSPECTIVECAMERA_H_INCLUDED__
#define __PERSPECTIVECAMERA_H_INCLUDED__
#include "Camera.h"

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(GLdouble fov, GLdouble near, GLdouble far);
    ~PerspectiveCamera();
    
    void update();
    void computeProjectionMatrix();
};
#endif /* PerspectiveCamera_h */
