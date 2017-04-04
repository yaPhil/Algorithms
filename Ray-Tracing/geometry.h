//
// Created by philipp on 29.03.17.
//

#ifndef RAY_TRACING_GEOMETRY_H
#define RAY_TRACING_GEOMETRY_H
#pragma once

#include <cmath>

class Vector {
    friend Vector operator+ (Vector one, const Vector& two);
    friend Vector operator- (Vector left, const Vector& right);
    friend Vector operator* (Vector left, const long double c);
    friend Vector operator* (const long double c, Vector left);
    friend bool operator==(const Vector& lhs, const Vector& rhs);
    friend long double dotProduct(Vector a, Vector b);
public:
    Vector() {
        x_ = y_ = z_ = 0.0;
    }
    Vector(long double x, long double y, long double z) :
            x_(x), y_(y), z_(z) {}
    Vector(const Vector& c) : x_(c.x_), y_(c.y_), z_(c.z_) {}

    long double getX() { return x_; }
    long double getY() { return y_; }
    long double getZ() { return z_; }

    Vector operator-();

    long double length() ;
    long double sqrLength();
    Vector normed();
    Vector crossProduct(Vector v);
    long double dotProduct(Vector v);
    long double orientVolume(Vector a, Vector b);
    long double absVolume(Vector a, Vector b);
    int checkColinear(Vector a);
private:
    long double x_;
    long double y_;
    long double z_;
};
long double dotProduct(Vector a, Vector b);
long double absVolume(Vector a, Vector b, Vector c);
int checkColinear(Vector a, Vector b);
Vector operator+ (Vector one, const Vector& two);
Vector operator- (Vector left, const Vector& right);
Vector operator* (Vector left, const long double c);
Vector operator*(const long double c, Vector left);
Vector operator/ (Vector left, const long double c);
Vector operator/ (const long double c, Vector rhs);
bool operator==(const Vector& lhs, const Vector& rhs);
bool operator!=(const Vector& lhs, const Vector& rhs);

const long double EPS = 0.000000001;

class Ray {
public:
    Ray() : begin_(), point_() {}
    Ray(Vector b, Vector p) : begin_(b), point_(p) {}

    Vector getBegin() const;

    Vector getPoint() const;


private:
    Vector begin_;
    Vector point_;
};
#endif //RAY_TRACING_GEOMETRY_H
