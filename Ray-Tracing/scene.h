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
public:
    Scene() : lights_(), objects_() {}
    void addObject(SolidObject* obj);
    void addLight(LightSource light);
    LightSource getLight(int i);
    int getLightsNumber();
    Intersect traceRay(Ray ray, SolidObject* exception);
private:
    std::vector<LightSource> lights_;
    std::vector<SolidObject*> objects_;
};
#endif //RAY_TRACING_SCENE_H
