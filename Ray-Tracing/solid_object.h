//
// Created by philipp on 29.03.17.
//

#ifndef RAY_TRACING_SOLID_OBJECT_H
#define RAY_TRACING_SOLID_OBJECT_H
#pragma once

#include "geometry.h"
#include "color.h"

class SolidObject {
public:
    SolidObject() : color_(), norm_(true) {}
    SolidObject(Color color) : color_(color), norm_(true) {}

    void reNorm() {
        norm_ = !norm_;
    }
    bool getOrient() {
        return norm_;
    }
    virtual Vector getNorm(Vector p) = 0;
    virtual Vector intersectRay(Ray ray) = 0;

    Color getColor() const {
        return color_;
    }

private:
    Color color_;
    bool norm_;
};

#endif //RAY_TRACING_SOLID_OBJECT_H
