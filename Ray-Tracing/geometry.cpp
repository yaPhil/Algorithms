//
// Created by philipp on 29.03.17.
//
#pragma once

#include "geometry.h"

Vector Vector::operator-() {
    return Vector(-x_, -y_, -z_);
}

long double Vector::length() {
    return std::sqrt(sqrLength());
}
long double Vector::sqrLength() {
    return x_ * x_ + y_ * y_ + z_ * z_;
}
Vector Vector::normed() {
    return Vector(x_ / length(), y_ / length(), z_ / length());
}
Vector Vector::crossProduct(Vector v) {
    return Vector(y_ * v.z_ - v.y_ * z_, v.x_ * z_ - x_ * v.z_, x_ * v.y_ - v.x_ * y_);
}
long double Vector::dotProduct(Vector v) {
    return v.x_ * x_ + v.y_ * y_ + v.z_ * z_;
}
long double dotProduct(Vector a, Vector b) {
    return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
}
long double Vector::orientVolume(Vector a, Vector b) {
    return x_ * (a.y_ * b.z_ - a.z_ * b.y_) - y_ * (a.x_ * b.z_ - a.z_ * b.x_) + z_ * (a.x_ * b.y_ - a.y_ * b.x_);
}
long double Vector::absVolume(Vector a, Vector b) {
    return std::abs(orientVolume(a, b));
}
int Vector::checkColinear(Vector a) {
    long double eps = 0.000000001;
    if(std::abs(dotProduct(a) - a.length() * length()) <= eps)
        return 1;
    if(std::abs(-dotProduct(a) - a.length() * length()) <= eps)
        return -1;
    return 0;
}
long double absVolume(Vector a, Vector b, Vector c) {
    return a.absVolume(b, c);
}
int checkColinear(Vector a, Vector b) {
    return a.checkColinear(b);
}

Vector operator+(Vector one, const Vector &two) {
    Vector tmp(one);
    tmp.x_ += two.x_; tmp.y_ += two.y_; tmp.z_ += two.z_;
    return tmp;
}

Vector operator- (Vector left, const Vector& right) {
    Vector tmp(left);
    tmp.x_ -= right.x_; tmp.y_ -= right.y_; tmp.z_ -= right.z_;
    return tmp;
}

Vector operator* (Vector left, const long double c) {
    Vector tmp(left);
    tmp.x_ *= c; tmp.y_ *= c; tmp.z_ *= c;
    return tmp;
}

Vector operator*(const long double c, Vector left) {
    Vector tmp(left);
    tmp.x_ *= c; tmp.y_ *= c; tmp.z_ *= c;
    return tmp;
}

bool operator==(const Vector& lhs, const Vector& rhs) {
    return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ == rhs.z_;
}

bool operator!=(const Vector& lhs, const Vector& rhs) {
    return !operator==(lhs, rhs);
}

Vector operator/(Vector left, const long double c) {
    return left * (1.0 / c);
}

Vector operator/(const long double c, Vector rhs) {
    return (1.0 / c) * rhs;
}

Vector Ray::getBegin() const {
    return begin_;
}

Vector Ray::getPoint() const {
    return point_;
}
