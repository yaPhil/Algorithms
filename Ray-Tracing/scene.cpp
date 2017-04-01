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

Scene::Intersect Scene::traceRay(Ray ray) {
    Intersect ans = Intersect();
    long double dist = 9999999999999;
    for(size_t i = 0; i < objects_.size(); ++i) {
        Vector intersection = objects_[i]->intersectRay(ray);
        if(intersection != Vector(0, 0, 0) && (intersection - ray.getBegin()).sqrLength() < dist) {
            dist = (intersection - ray.getBegin()).sqrLength();
            ans = Intersect(intersection, objects_[i]);
        }
    }
    return ans;
}
