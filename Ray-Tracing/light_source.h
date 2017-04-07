//
// Created by philipp on 30.03.17.
//

#ifndef RAY_TRACING_LIGHT_SOURCE_H
#define RAY_TRACING_LIGHT_SOURCE_H
#pragma once

#include "geometry.h"
#include "color.h"
#include "solid_object.h"
class SolidObject;

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

class ReflectedLightSource {
public:
    ReflectedLightSource() : position_(), intense_(0), originLightSource_(), object_() {}
    ReflectedLightSource(Vector v, long double inten, LightSource source, SolidObject* obj) :
        position_(v), intense_(inten), originLightSource_(source), object_(obj) {}
    Vector getPosition() const {
        return position_;
    }
    long double getIntence() {
        return intense_;
    }
    LightSource getSource() const {
        return originLightSource_;
    }
    SolidObject* getObject() const {
        return object_;
    }
private:
    Vector position_;
    long double intense_;
    LightSource originLightSource_;
    SolidObject* object_;
};

#endif //RAY_TRACING_LIGHT_SOURCE_H
