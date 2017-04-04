//
// Created by philipp on 30.03.17.
//

#include <cstdio>
#include "scene.h"

void Scene::addObject(SolidObject *obj) {
    objects_.push_back(obj);
}

void Scene::addLight(LightSource light) {
    lights_.push_back(light);
}

LightSource Scene::getLight(int i) {
    return lights_[i];
}

int Scene::getLightsNumber() {
    return lights_.size();
}

Intersect Scene::traceRay(Ray ray, SolidObject *exception) {
    Intersect ans = Intersect();
    long double dist = 1.7E+307;
    for(size_t i = 0; i < objects_.size(); ++i) {
        if(objects_[i] == exception)
            continue;
        Intersect intersection = objects_[i]->intersectRay(ray);
        if(intersection.getResult() && (intersection.getPoint() - ray.getBegin()).length() < dist) {
            dist = (intersection.getPoint() - ray.getBegin()).length();
            ans = intersection;
        }
    }
    return ans;
}
