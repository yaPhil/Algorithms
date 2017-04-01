//
// Created by philipp on 30.03.17.
//
#pragma once

#include "base_object.h"
#include "geometry.h"

Vector Triangle::getNorm(Vector p) {
    if (getOrient())
        return ((b_ - a_).crossProduct(c_ - b_)).normed();
    else
        return -((b_ - a_).crossProduct(c_ - b_)).normed();
}

Vector Triangle::intersectRay(Ray ray) {
    Vector beg = ray.getBegin();
    Vector point = ray.getPoint();
    long double eps = 0.0000001;
    if (std::abs(dotProduct(point - beg, getNorm())) <= eps) {
        return Vector();
    }
    long double dist = (a_ - beg).absVolume(b_ - beg, c_ - beg) /
            (a_ - b_).crossProduct(c_ - b_).length();
    long double cos = std::abs(dotProduct(point - beg, getNorm())) / (point - beg).length();
    long double coef = dist / cos;
    Vector rightNorm = getNorm();
    if(dotProduct(rightNorm, a_ - beg) < -eps) {
        rightNorm = -rightNorm;
    }
    if (dotProduct(rightNorm, point - beg) < -eps) {
        return Vector();
    }
    Vector candidate = (point - beg).normed() * coef;
    if (checkPoint(candidate)) {
        return candidate;
    }
    else {
        return Vector();
    }
}

bool Triangle::checkPoint(Vector p) {
//    if(absVolume(a_ - p, b_ - p, c_ - p) != 0) {
//        return false;
//    }
    Vector one = (a_ - p).crossProduct(b_ - p);
    Vector two = (b_ - p).crossProduct(c_ - p);
    Vector three = (c_ - p).crossProduct(a_ - p);
    return one.checkColinear(two) == 1 && one.checkColinear(three) == 1 && two.checkColinear(three) == 1;
}

Vector Sphere::getNorm(Vector p) {
    if (getOrient())
        return (p - center_).normed();
    else
        return -(p - center_).normed();
}

Vector Sphere::intersectRay(Ray ray) {
    Vector begin = ray.getBegin();
    Vector point = ray.getPoint();
    long double sqrHeigth = (center_ - begin).crossProduct(point - begin).sqrLength() /
            (point - begin).sqrLength();
    if(sqrHeigth > rad_ * rad_) {
        return  Vector();
    }
    if((center_ - begin).dotProduct(point - begin) < 0 &&
            (center_ - begin).sqrLength() > rad_ * rad_) {
        return Vector();
    }
    long double len = 0;
    if((center_ - begin).sqrLength() > rad_ * rad_ ||
            (center_ - begin).dotProduct(point - begin) < 0) {
        len = std::abs(std::sqrt((center_ - begin).sqrLength() - sqrHeigth) -
                       std::sqrt(rad_ * rad_ - sqrHeigth));
    }
    else {
        len = std::sqrt((center_ - begin).sqrLength() - sqrHeigth) +
              std::sqrt(rad_ * rad_ - sqrHeigth);
    }
    return (point - begin).normed() * len;
}
