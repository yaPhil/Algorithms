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
    LightSource(Vector v, long double i) : position_(v), intense_(i) {}

    Vector getPosition() const {
        return position_;
    }

    long double getIntense() const {
        return intense_;
    }
    static const long double baseShining;
private:
    Vector position_;
    long double intense_;
};
#endif //RAY_TRACING_LIGHT_SOURCE_H
