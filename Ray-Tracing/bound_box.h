#ifndef BOUND_BOX_H
#define BOUND_BOX_H
#pragma once
#include"geometry.h"
#include <algorithm>

class BoundBox {
public:
    BoundBox() : xUp_(0), xDown_(0), yUp_(0), yDown_(0), zUp_(0), zDown_(0) {}
    BoundBox(long double _xu, long double _xd, long double _yu, long double _yd, long double _zu, long double _zd) :
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

    bool intersect(Ray ray) {
        Vector ray_pos = ray.getBegin();
        Vector inv_dir = Vector(1.0 / (ray.getPoint() - ray_pos).getX(), 1.0 / (ray.getPoint() - ray_pos).getY(),
                                1.0 / (ray.getPoint() - ray_pos).getZ());
        long double lo = inv_dir.getX() * (xDown_ - ray_pos.getX());
        long double hi = inv_dir.getX() * (xUp_ - ray_pos.getX());

        long double tmin  = std::min(lo, hi);
        long double tmax = std::max(lo, hi);

        long double lo1 = inv_dir.getY() * (yDown_ - ray_pos.getY());
        long double hi1 = inv_dir.getY() * (yUp_ - ray_pos.getY());
        tmin  = std::max(tmin, std::min(lo1, hi1));
        tmax = std::min(tmax, std::max(lo1, hi1));

        long double lo2 = inv_dir.getZ() * (zDown_ - ray_pos.getZ());
        long double hi2 = inv_dir.getZ() * (zUp_ - ray_pos.getZ());
        tmin  = std::max(tmin, std::min(lo2, hi2));
        tmax = std::min(tmax, std::max(lo2, hi2));

        return (tmin <= tmax + EPS) && (tmax > -EPS);
    }

    long double getXUp() { return xUp_; }
    long double getXDown() { return xDown_; }

    long double getYUp() { return yUp_; }
    long double getYDown() { return yDown_; }

    long double getZUp() { return zUp_; }
    long double getZDown() { return zDown_; }

    Vector getUpCorner() {
        return Vector(xUp_, yUp_, zUp_);
    }

    Vector getDownCorner() {
        return Vector(xDown_, yDown_, zDown_);
    }

private:
    long double xUp_, xDown_, yUp_, yDown_, zUp_, zDown_;
};
#endif // BOUND_BOX_H
