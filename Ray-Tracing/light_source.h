//
// Created by philipp on 30.03.17.
//

#ifndef RAY_TRACING_LIGHT_SOURCE_H
#define RAY_TRACING_LIGHT_SOURCE_H
#pragma once

#include "geometry.h"

class LightSource {
public:
    LightSource() : position_(), intense_(0) {}
    LightSource(Vector v, double i) : position_(v), intense_(i) {}

    Vector getPosition() const {
        return position_;
    }

    double getIntense() const {
        return intense_;
    }

private:
    Vector position_;
    double intense_;
};
#endif //RAY_TRACING_LIGHT_SOURCE_H
