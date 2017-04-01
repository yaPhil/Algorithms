//
// Created by philipp on 30.03.17.
//

#ifndef RAY_TRACING_COLOR_H
#define RAY_TRACING_COLOR_H
class Color {
public:
    Color() : red_(0), green_(0), blue_(0), alpha_(0) {}
    Color(long double r, long double g, long double b, long double a) :
            red_(r), green_(g), blue_(b), alpha_(a) {}

    long double getRed() const {
        return red_;
    }

    long double getGreen() const {
        return green_;
    }

    long double getBlue() const {
        return blue_;
    }

    long double getAlpha() const {
        return alpha_;
    }

private:
    long double red_;
    long double green_;
    long double blue_;
    long double alpha_;
};
#endif //RAY_TRACING_COLOR_H
