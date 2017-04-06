//
// Created by philipp on 30.03.17.
//
#include "base_object.h"
#include "geometry.h"

Vector Triangle::getNorm(Vector p) {
    if (getOrient())
        return ((b_ - a_).crossProduct(c_ - b_)).normed();
    else
        return -((b_ - a_).crossProduct(c_ - b_)).normed();
}

Intersect Triangle::intersectRay(Ray ray) {
    Vector beg = ray.getBegin();
    Vector point = ray.getPoint();
    if (std::abs(dotProduct(point - beg, getNorm())) <= EPS) {
        return Intersect();
    }
    double dist = (a_ - beg).absVolume(b_ - beg, c_ - beg) /
            (a_ - b_).crossProduct(c_ - b_).length();
    double cos = std::abs(dotProduct(point - beg, getNorm())) / (point - beg).length();
    double coef = dist / cos;
    Vector rightNorm = getNorm();
    if(dotProduct(rightNorm, a_ - beg) < -EPS) {
        rightNorm = -rightNorm;
    }
    if (dotProduct(rightNorm, point - beg) < -EPS) {
        return Intersect();
    }
    Vector candidate = beg + (point - beg).normed() * coef;
    if (checkPoint(candidate)) {
        return Intersect(candidate, this, true);
    }
    else {
        return Intersect();
    }
}

Vector Triangle::projectPoint(Vector p) {
    double dist = (a_ - p).absVolume(b_ - p, c_ - p) / (a_ - b_).crossProduct(c_ - b_).length();
    Vector norm = getNorm();
    if(dotProduct(norm, a_ - p) < 0) {
        norm = -norm;
    }
    return p + norm * dist;
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

Intersect Sphere::intersectRay(Ray ray) {
    Vector begin = ray.getBegin();
    Vector point = ray.getPoint();
    double sqrHeigth = (center_ - begin).crossProduct(point - begin).sqrLength() /
            (point - begin).sqrLength();
    if(sqrHeigth > rad_ * rad_) {
        return  Intersect();
    }
    if((center_ - begin).dotProduct(point - begin) < 0 &&
            (center_ - begin).sqrLength() > rad_ * rad_) {
        return Intersect();
    }
    double len = 0;
    if((center_ - begin).sqrLength() > rad_ * rad_ ||
            (center_ - begin).dotProduct(point - begin) < 0) {
        len = std::abs(std::sqrt((center_ - begin).sqrLength() - sqrHeigth) -
                       std::sqrt(rad_ * rad_ - sqrHeigth));
    }
    else {
        len = std::sqrt((center_ - begin).sqrLength() - sqrHeigth) +
              std::sqrt(rad_ * rad_ - sqrHeigth);
    }
    return Intersect(begin + (point - begin).normed() * len, this, true);
}

Vector Sphere::projectPoint(Vector p) {
    double curLength = (p - center_).length();
    return p + ((p - center_).normed() * (rad_ - curLength));
}
