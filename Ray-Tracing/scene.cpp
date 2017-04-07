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

void Scene::addSecondaryLight(ReflectedLightSource light)
{
    secondaryLights_.push_back(light);
}

LightSource Scene::getLight(int i) {
    return lights_[i];
}

int Scene::getLightsNumber() {
    return lights_.size();
}

ReflectedLightSource Scene::getSecondaryLight(int i)
{
    return secondaryLights_[i];
}

int Scene::getSecondaryLightNumber()
{
    return secondaryLights_.size();
}

int Scene::getObjectsNumber()
{
    return objects_.size();
}

SolidObject *Scene::getObject(int i)
{
    return objects_[i];
}

Intersect Scene::traceRay(Ray ray, SolidObject *exception) {
//    Intersect ans = Intersect();
//    long double dist = 1.7E+307;
//    for(size_t i = 0; i < objects_.size(); ++i) {
//        if(objects_[i] == exception)
//            continue;
//        Intersect intersection = objects_[i]->intersectRay(ray);
//        if(intersection.getResult() && (intersection.getPoint() - ray.getBegin()).length() < dist) {
//            dist = (intersection.getPoint() - ray.getBegin()).length();
//            ans = intersection;
//        }
//    }
//    return ans;
    return root_->intersectRay(ray, root_);
}

void Scene::setRoot()
{
    root_ = new KdTree(objects_.begin(), objects_.end(), 0);
}
