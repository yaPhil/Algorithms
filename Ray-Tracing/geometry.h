//
// Created by philipp on 29.03.17.
//

#ifndef RAY_TRACING_GEOMETRY_H
#define RAY_TRACING_GEOMETRY_H

class Vector {
public:
    Vector() {
        x_ = y_ = z_ = 0;
    }
    Vector(long double x, long double y, long double z) :
            x_(x), y_(y), z_(z) {}
    Vector(const Vector& c) : x_(c.x_), y_(c.y_), z_(c.z_) {}
private:
    long double x_;
    long double y_;
    long double z_;
};

Vector operator+ (Vector one, const Vector& two);
Vector operator- (Vector left, const Vector& right);
Vector operator* (Vector left, const int c);
#endif //RAY_TRACING_GEOMETRY_H
