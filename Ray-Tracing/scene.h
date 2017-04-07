//
// Created by philipp on 30.03.17.
//

#ifndef RAY_TRACING_SCENE_H
#define RAY_TRACING_SCENE_H
#pragma once

#include "solid_object.h"
#include "light_source.h"
#include <vector>
#include "kd_tree.h"

class Scene {
public:
    Scene() : lights_(), objects_(), secondaryLights_(), root_() {}
    void addObject(SolidObject* obj);
    void addLight(LightSource light);
    void addSecondaryLight(ReflectedLightSource light);
    LightSource getLight(int i);
    int getLightsNumber();
    ReflectedLightSource getSecondaryLight(int i);
    int getSecondaryLightNumber();

    int getObjectsNumber();
    SolidObject* getObject(int i);
    Intersect traceRay(Ray ray, SolidObject* exception);

    void setRoot();
private:
    std::vector<LightSource> lights_;
    std::vector<SolidObject*> objects_;
    std::vector<ReflectedLightSource> secondaryLights_;
    KdTree *root_;
};
#endif //RAY_TRACING_SCENE_H
