//
// Created by philipp on 30.03.17.
//

#ifndef RAY_TRACING_SCENE_H
#define RAY_TRACING_SCENE_H
#pragma once

#include "solid_object.h"
#include "light_source.h"
#include <vector>

class Scene {
    class Intersect;
public:
    Scene() : lights_(), objects_() {}
    void addObject(SolidObject* obj);
    void addLight(LightSource light);
    Scene::Intersect traceRay(Ray ray);
private:
    std::vector<LightSource> lights_;
    std::vector<SolidObject*> objects_;
    class Intersect {
    public:
        Intersect() : point_(), object_(nullptr) {}
        Intersect(Vector p, SolidObject* obj) : point_(p), object_(obj) {}
        Vector getPoint() const {
            return point_;
        }
        SolidObject* getObject() const {
            return object_;
        }
    private:
        Vector point_;
        SolidObject* object_;
    };
};
#endif //RAY_TRACING_SCENE_H
