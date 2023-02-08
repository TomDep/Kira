#include "Odometry.hpp"

Odometry::Odometry() {
    this->position = glm::vec2(0.0f, 0.0f);
    this->angle = 0;
    this->controller = new Controller();
}

void Odometry::update() {
    int16_t posX, posY, angle;
    controller->getPosition(&posX, &posY, &angle);
    this->position.x = (double) posX;
    this->position.y = (double) posY;
    this->angle = (double) angle/1000.0f;
    
    
    
    controller->flush();
}
