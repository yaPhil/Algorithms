//
// Created by philipp on 29.03.17.
//
#pragma once

#include "geometry.h"

Vector operator+(Vector one, const Vector &two) {
    one.x_ += two.x_; one.y_ += two.y_; one.z_ += two.z_;
    return one;
}

Vector operator- (Vector left, const Vector& right) {
    left.x_ -= right.x_; left.y_ -= right.y_; left.z_ -= right.z_;
    return left;
}

Vector operator* (Vector left, const int c) {
    left.x_ *= c; left.y_ *= c; left.z_ *= c;
    return left;
}


