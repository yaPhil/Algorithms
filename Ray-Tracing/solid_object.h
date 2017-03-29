//
// Created by philipp on 29.03.17.
//

#ifndef RAY_TRACING_SOLID_OBJECT_H
#define RAY_TRACING_SOLID_OBJECT_H
#pragma once

#include "geometry.h"

class SolidObject {
public:
    SolidObject() : color_(0), norm_() {}
    SolidObject(int color, Vector n) : color_(color), norm_(n) {}
    SolidObject(int color) : color_(color), norm_() {}

    void reNorm() {
        norm_ = norm_ * (-1);
    }
    void setNorm(Vector n) {
        norm_ = n;
    }
    Vector getNorm() {
        return norm_;
    }
private:
    int color_;
    Vector norm_;
};

#endif //RAY_TRACING_SOLID_OBJECT_H
