//
// Created by philipp on 29.03.17.
//

#ifndef RAY_TRACING_SOLID_OBJECT_H
#define RAY_TRACING_SOLID_OBJECT_H
#pragma once

#include "geometry.h"
#include "color.h"

class Intersect;

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
    virtual Intersect intersectRay(Ray ray) = 0;
    virtual Vector projectPoint(Vector p) = 0;
    Color getColor() const {
        return color_;
    }

private:
    Color color_;
    bool norm_;
};

class Intersect {
public:
    Intersect() : point_(), object_(nullptr), isIntersec_(false) {}
    Intersect(Vector p, SolidObject* obj, bool res) : point_(p), object_(obj), isIntersec_(res) {}
    Vector getPoint() const {
        return point_;
    }
    SolidObject* getObject() const {
        return object_;
    }
    bool getResult() {
        return isIntersec_;
    }

private:
    Vector point_;
    SolidObject* object_;
    bool isIntersec_;
};

#endif //RAY_TRACING_SOLID_OBJECT_H
