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
    Triangle(Color color, Vector a, Vector b, Vector c) : SolidObject(color), a_(a), b_(b), c_(c)
    {}

    Vector getNorm(Vector p=Vector()) override;
    Vector intersectRay(Ray ray) override;
    bool checkPoint (Vector p);

private:
    Vector a_;
    Vector b_;
    Vector c_;
};

class Sphere : public SolidObject {
public:
    Sphere() : SolidObject(), center_(), rad_(0) {}
    Sphere(Color color, Vector c, double r) : SolidObject(color), center_(c), rad_(r) {}

    Vector getNorm(Vector p) override;

    Vector intersectRay(Ray ray) override;

private:
    Vector center_;
    double rad_;
};

#endif //RAY_TRACING_BASE_OBJECT_H
