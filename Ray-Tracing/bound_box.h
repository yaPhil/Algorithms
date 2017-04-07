#ifndef BOUND_BOX_H
#define BOUND_BOX_H
#include"geometry.h"
#include <algorithm>
class BoundBox {
public:
    BoundBox() : xUp_(0), xDown_(0), yUp_(0), yDown_(0), zUp_(0), zDown_(0) {}
    BoundBox(double _xu, double _xd, double _yu, double _yd, double _zu, double _zd) :
        xUp_(_xu), xDown_(_xd), yUp_(_yu), yDown_(_yd), zUp_(_zu), zDown_(_zd) {}
    BoundBox(Vector first, Vector second) {
        xUp_ = std::max(first.getX(), second.getX());
        xDown_ = std::min(first.getX(), second.getX());

        yUp_ = std::max(first.getY(), second.getY());
        yDown_ = std::min(first.getY(), second.getY());

        zUp_ = std::max(first.getZ(), second.getZ());
        zDown_ = std::min(first.getZ(), second.getZ());
    }

    BoundBox(BoundBox first, BoundBox second) {
        xUp_ = std::max(first.getUpCorner().getX(), second.getUpCorner().getX());
        xDown_ = std::min(first.getDownCorner().getX(), second.getDownCorner().getX());

        yUp_ = std::max(first.getUpCorner().getY(), second.getUpCorner().getY());
        yDown_ = std::min(first.getDownCorner().getY(), second.getDownCorner().getY());

        zUp_ = std::max(first.getUpCorner().getZ(), second.getUpCorner().getZ());
        zDown_ = std::min(first.getDownCorner().getZ(), second.getDownCorner().getZ());
    }

    double getXUp() { return xUp_; }
    double getXDown() { return xDown_; }

    double getYUp() { return yUp_; }
    double getYDown() { return yDown_; }

    double getZUp() { return zUp_; }
    double getZDown() { return zDown_; }

    Vector getUpCorner() {
        return Vector(xUp_, yUp_, zUp_);
    }

    Vector getDownCorner() {
        return Vector(xDown_, yDown_, zDown_);
    }

private:
    double xUp_, xDown_, yUp_, yDown_, zUp_, zDown_;
};
#endif // BOUND_BOX_H
