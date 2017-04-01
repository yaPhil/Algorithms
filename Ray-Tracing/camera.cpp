//
// Created by philipp on 30.03.17.
//

#include "camera.h"

Ray Camera::getRay(int resX, int resY, int posX, int posY) {
    Vector dx = (rightHi_ - leftHi_) / resX;
    Vector dy = (leftDown_ - leftHi_) / resY;
    return Ray(camera_, leftHi_ + dx * posX + dy * posY);
}
