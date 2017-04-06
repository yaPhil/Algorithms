//
// Created by philipp on 30.03.17.
//

#ifndef RAY_TRACING_COLOR_H
#define RAY_TRACING_COLOR_H
class Color {
public:
    Color() : red_(0), green_(0), blue_(0), alpha_(0), reflection_(0), refraction_(0) {}
    Color(double r, double g, double b, double a, double refl, double refr) :
            red_(r), green_(g), blue_(b), alpha_(a), reflection_(refl), refraction_(refr) {}

    double getRed() const {
        return red_;
    }

    double getGreen() const {
        return green_;
    }

    double getBlue() const {
        return blue_;
    }

    double getAlpha() const {
        return alpha_;
    }
    double getReflection() const {
        return reflection_;
    }
    double getRefraction() const {
        return refraction_;
    }
private:
    double red_;
    double green_;
    double blue_;
    double alpha_;
    double reflection_;
    double refraction_;
};
#endif //RAY_TRACING_COLOR_H
