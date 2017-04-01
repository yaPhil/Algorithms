//
// Created by philipp on 30.03.17.
//

#ifndef RAY_TRACING_CAMERA_H
#define RAY_TRACING_CAMERA_H
#pragma once

#include "geometry.h"

class Camera {
public:
    Camera() : camera_(), leftHi_(), rightHi_(), leftDown_(), rightDown_() {}
    Camera(Vector camera, Vector lh, Vector rh, Vector ld) :
            camera_(camera), leftHi_(lh), rightHi_(rh), leftDown_(ld) {
        rightDown_ = rightHi_ + leftDown_ - leftHi_;
    }
    Ray getRay(int resX, int resY, int posX, int posY);
private:
    Vector camera_;
    Vector leftHi_;
    Vector rightHi_;
    Vector leftDown_;
    Vector rightDown_;
};
#endif //RAY_TRACING_CAMERA_H
