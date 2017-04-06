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
    friend Vector operator* (Vector left, const double c);
    friend Vector operator* (const double c, Vector left);
    friend bool operator==(const Vector& lhs, const Vector& rhs);
    friend double dotProduct(Vector a, Vector b);
public:
    Vector() {
        x_ = y_ = z_ = 0.0;
    }
    Vector(double x, double y, double z) :
            x_(x), y_(y), z_(z) {}
    Vector(const Vector& c) : x_(c.x_), y_(c.y_), z_(c.z_) {}

    double getX() { return x_; }
    double getY() { return y_; }
    double getZ() { return z_; }

    Vector operator-();

    double length() ;
    double sqrLength();
    Vector normed();
    Vector crossProduct(Vector v);
    double dotProduct(Vector v);
    double orientVolume(Vector a, Vector b);
    double absVolume(Vector a, Vector b);
    int checkColinear(Vector a);
private:
    double x_;
    double y_;
    double z_;
};
double dotProduct(Vector a, Vector b);
double absVolume(Vector a, Vector b, Vector c);
int checkColinear(Vector a, Vector b);
Vector operator+ (Vector one, const Vector& two);
Vector operator- (Vector left, const Vector& right);
Vector operator* (Vector left, const double c);
Vector operator*(const double c, Vector left);
Vector operator/ (Vector left, const double c);
Vector operator/ (const double c, Vector rhs);
bool operator==(const Vector& lhs, const Vector& rhs);
bool operator!=(const Vector& lhs, const Vector& rhs);

const double EPS = 0.000000001;

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
