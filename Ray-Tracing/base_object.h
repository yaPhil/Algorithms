//
// Created by philipp on 30.03.17.
//

#ifndef RAY_TRACING_BASE_OBJECT_H
#define RAY_TRACING_BASE_OBJECT_H
#pragma once
#include "solid_object.h"

class Triangle : public SolidObject {
public:
    Triangle() : SolidObject(), a_(), b_(), c_() {}
    Triangle(Color color, Vector a, Vector b, Vector c) : SolidObject(color), a_(a), b_(b), c_(c) {
        long double xu = std::max(std::max(a_.getX(), b_.getX()), c_.getX());
        long double xd = std::min(std::min(a_.getX(), b_.getX()), c_.getX());
        long double yu = std::max(std::max(a_.getY(), b_.getY()), c_.getY());
        long double yd = std::min(std::min(a_.getY(), b_.getY()), c_.getY());
        long double zu = std::max(std::max(a_.getZ(), b_.getZ()), c_.getZ());
        long double zd = std::min(std::min(a_.getZ(), b_.getZ()), c_.getZ());
        setBox(BoundBox(xu, xd, yu, yd, zu, zd));
    }

    Vector getNorm(Vector p=Vector()) override;
    Intersect intersectRay(Ray ray) override;
    bool checkPoint (Vector p);
    Vector projectPoint(Vector p) override;
    ReflectedLightSource getSecondaryLight(LightSource light) override;
    //bool between(Vector first, Vector second) override;

private:
    Vector a_;
    Vector b_;
    Vector c_;
};

class Sphere : public SolidObject {
public:
    Sphere() : SolidObject(), center_(), rad_(0) {}
    Sphere(Color color, Vector c, long double r) : SolidObject(color), center_(c), rad_(r) {
        long double xu = center_.getX() + rad_, xd = center_.getX() - rad_;
        long double yu = center_.getY() + rad_, yd = center_.getY() - rad_;
        long double zu = center_.getZ() + rad_, zd = center_.getZ() - rad_;
        setBox(BoundBox(xu, xd, yu, yd, zu, zd));
    }

    Vector getNorm(Vector p) override;

    Intersect intersectRay(Ray ray) override;
    Vector projectPoint(Vector p) override;
    ReflectedLightSource getSecondaryLight(LightSource light) override;
    //bool between(Vector first, Vector second) override;
private:
    Vector center_;
    long double rad_;
};

class Quadrangle : public SolidObject {
public:
    Quadrangle() : a_(), b_(), c_(), d_() {}
    Quadrangle(Color color, Vector a, Vector b, Vector c, Vector d) : SolidObject(color), a_(a), b_(b), c_(c), d_(d) {
        long double xu = std::max(std::max(a_.getX(), b_.getX()), std::max(c_.getX(), d_.getX()));
        long double xd = std::min(std::min(a_.getX(), b_.getX()), std::min(c_.getX(), d_.getX()));
        long double yu = std::max(std::max(a_.getY(), b_.getY()), std::max(c_.getY(), d_.getY()));
        long double yd = std::min(std::min(a_.getY(), b_.getY()), std::min(c_.getY(), d_.getY()));
        long double zu = std::max(std::max(a_.getZ(), b_.getZ()), std::max(c_.getZ(), d_.getZ()));
        long double zd = std::min(std::min(a_.getZ(), b_.getZ()), std::min(c_.getZ(), d_.getZ()));
        setBox(BoundBox(xu, xd, yu, yd, zu, zd));
    }
    Vector getNorm(Vector p) override;
    Intersect intersectRay(Ray ray) override;
    Vector projectPoint(Vector p) override;
    ReflectedLightSource getSecondaryLight(LightSource light) override;
    //bool between(Vector first, Vector second) override;
private:
    Vector a_;
    Vector b_;
    Vector c_;
    Vector d_;
};

#endif //RAY_TRACING_BASE_OBJECT_H
