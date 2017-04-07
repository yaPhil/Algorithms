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
    long double dist = (a_ - beg).absVolume(b_ - beg, c_ - beg) /
            (a_ - b_).crossProduct(c_ - b_).length();
    long double cos = std::abs(dotProduct(point - beg, getNorm())) / (point - beg).length();
    long double coef = dist / cos;
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
    long double dist = (a_ - p).absVolume(b_ - p, c_ - p) / (a_ - b_).crossProduct(c_ - b_).length();
    Vector norm = getNorm();
    if(dotProduct(norm, a_ - p) < 0) {
        norm = -norm;
    }
    return p + norm * dist;
}

ReflectedLightSource Triangle::getSecondaryLight(LightSource light)
{
    Vector point = light.getPosition();
    long double distance = (a_ - point).absVolume(b_ - point, c_ - point) / (a_ - b_).crossProduct(c_ - b_).length();
    Vector rightNorm = getNorm();
    if((a_ - point).dotProduct(rightNorm) < -EPS) {
        rightNorm = - rightNorm;
    }
    Vector reflection = point + rightNorm * 2 * distance;
    return ReflectedLightSource(reflection, light.getIntense() * getColor().getReflection(), light, this);
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
    long double sqrHeigth = (center_ - begin).crossProduct(point - begin).sqrLength() /
            (point - begin).sqrLength();
    if(sqrHeigth > rad_ * rad_) {
        return  Intersect();
    }
    if((center_ - begin).dotProduct(point - begin) < 0 &&
            (center_ - begin).sqrLength() > rad_ * rad_) {
        return Intersect();
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
    return Intersect(begin + (point - begin).normed() * len, this, true);
}

Vector Sphere::projectPoint(Vector p) {
    long double curLength = (p - center_).length();
    return p + ((p - center_).normed() * (rad_ - curLength));
}

ReflectedLightSource Sphere::getSecondaryLight(LightSource light)
{
    Vector point = light.getPosition();
    Ray toCenter = Ray(point, center_);
    Intersect inter = intersectRay(toCenter);
    Vector reflection = inter.getPoint() + (center_ - point).normed() * rad_ / 2.0;
    return ReflectedLightSource(reflection, light.getIntense() * getColor().getReflection(), light, this);
}

Vector Quadrangle::getNorm(Vector p)
{
    Triangle born = Triangle(this->getColor(), a_, b_, c_);
    return born.getNorm();
}

Intersect Quadrangle::intersectRay(Ray ray)
{
    Triangle withoutA = Triangle(getColor(), b_, c_, d_);
    Triangle withoutB = Triangle(getColor(), c_, d_, a_);
    Triangle withoutC = Triangle(getColor(), d_, a_, b_);
    Triangle withoutD = Triangle(getColor(), a_, b_, c_);
    Triangle one = withoutB, two = withoutD;
    if(withoutA.checkPoint(a_)) {
        one = withoutB;
        two = withoutD;
    }
    if(withoutB.checkPoint(b_)) {
        one = withoutA;
        two = withoutC;
    }
    if(withoutC.checkPoint(c_)) {
        one = withoutB;
        two = withoutD;
    }
    if(withoutD.checkPoint(d_)) {
        one = withoutA;
        two = withoutC;
    }
    Intersect oneInter = one.intersectRay(ray);
    Intersect twoInter = two.intersectRay(ray);
    if(!oneInter.getResult() && !twoInter.getResult()) {
        return Intersect();
    }
    if(oneInter.getResult() && !twoInter.getResult()) {
        return Intersect(oneInter.getPoint(), this, true);
    }
    if(twoInter.getResult() && !oneInter.getResult()) {
        return Intersect(twoInter.getPoint(), this, true);
    }
    return Intersect(oneInter.getPoint(), this, true);
}

Vector Quadrangle::projectPoint(Vector p)
{
    Triangle born = Triangle(this->getColor(), a_, b_, c_);
    return born.projectPoint(p);
}

ReflectedLightSource Quadrangle::getSecondaryLight(LightSource light)
{
    Triangle born = Triangle(this->getColor(), a_, b_, c_);
    return born.getSecondaryLight(light);
}
