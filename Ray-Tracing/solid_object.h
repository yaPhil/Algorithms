//
// Created by philipp on 29.03.17.
//

#ifndef RAY_TRACING_SOLID_OBJECT_H
#define RAY_TRACING_SOLID_OBJECT_H
#pragma once

#include "geometry.h"
#include "color.h"
#include "light_source.h"
#include "bound_box.h"

class Intersect;
class ReflectedLightSource;
class LightSource;

class SolidObject {
public:
    SolidObject() : color_(), norm_(true), box_() {}
    SolidObject(Color color) : color_(color), norm_(true), box_() {}

    void reNorm() {
        norm_ = !norm_;
    }
    bool getOrient() {
        return norm_;
    }
    virtual Vector getNorm(Vector p) = 0;
    virtual Intersect intersectRay(Ray ray) = 0;
    virtual Vector projectPoint(Vector p) = 0;
    virtual ReflectedLightSource getSecondaryLight(LightSource light) = 0;

    virtual bool between(Vector first, Vector second) = 0;

    Color getColor() const {
        return color_;
    }
    BoundBox getBox() const {
        return box_;
    }
    void setBox(BoundBox box) {
        box_ = box;
    }

    bool xCompare(SolidObject *first, SolidObject *second) {
        return first->box_.getDownCorner().getX() < second->box_.getDownCorner().getX();
    }

    bool yCompare(SolidObject *first, SolidObject *second) {
        return first->box_.getDownCorner().getY() < second->box_.getDownCorner().getY();
    }

    bool zCompare(SolidObject *first, SolidObject *second) {
        return first->box_.getDownCorner().getZ() < second->box_.getDownCorner().getZ();
    }
private:
    Color color_;
    bool norm_;
    BoundBox box_;
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
