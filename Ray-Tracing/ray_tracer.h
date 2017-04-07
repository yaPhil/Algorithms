//
// Created by philipp on 31.03.17.
//

#ifndef RAY_TRACING_RAY_TRACER_H
#define RAY_TRACING_RAY_TRACER_H
#pragma once

#include "scene.h"
#include "camera.h"
#include "base_object.h"
#include <QMainWindow>
#include <QPainter>
#include <QPicture>

class RayTraicer {
public:
    RayTraicer() : scene_(), camera_(), resX_(0), resY_(0) {}
    RayTraicer(Vector camera, Vector lh, Vector rh, Vector ld, int resX, int resY) :
            scene_(), camera_(camera, lh, rh, ld), resX_(resX), resY_(resY) {}
    RayTraicer(std::string file);
    void addObject(SolidObject* obj);
    void addLight(LightSource light);
    int getResX();
    int getResY();
    void traceRays(int start, QImage &img, int aa);
    void run(QImage &img, int aa);
private:
    void setColor(int x, int y, QImage &img, int aa);
    Vector getColor(Ray ray, int depth);
    Vector getIllumination(Ray ray, Vector point, SolidObject* obj);
    Scene scene_;
    Camera camera_;
    int resX_;
    int resY_;
};

const int MAX_RECURSIVE_DEPTH = 10;
const long double EPS_MARGIN = 0.00000001;
#endif //RAY_TRACING_RAY_TRACER_H
