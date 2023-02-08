#ifndef __ODOMETRY_H__
#define __ODOMETRY_H__

#include "Controller.hpp"

#include <glm/glm.hpp>

class Odometry {

public:
    Odometry();
    void update();

    // ----- Getters & Setters -----
    glm::vec2 getPosition() const { return position; }
    double getAngle() const { return angle; }
private:
    glm::vec2 position;
    double angle;
    Controller* controller; 
};

#endif
