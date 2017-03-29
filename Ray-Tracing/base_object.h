//
// Created by philipp on 30.03.17.
//

#ifndef RAY_TRACING_BASE_OBJECT_H
#define RAY_TRACING_BASE_OBJECT_H
#pragma once
#include "solid_object.h"

class Triangle : public SolidObject {
public:
    Triangle() : SolidObject(), a_(), b_(), c_() {}
    Triangle(int color, Vector a, Vector b, Vector c) : SolidObject(color), a_(a), b_(b), c_(c)
    {}

private:
    Vector a_;
    Vector b_;
    Vector c_;
};

#endif //RAY_TRACING_BASE_OBJECT_H
