
#include"kd_tree.h"
#include<algorithm>

BoundBox KdTree::getBox() {
    return box_;
}

KdTree::KdTree(std::vector<SolidObject*>::iterator begin, std::vector<SolidObject*>::iterator end, int currCoord) {
    currCoordinate_ = currCoord;
    if (end != begin + 1) {
        int n = (end - begin) / 2;
        if (currCoordinate_ == 0) {
            std::nth_element(begin, begin + n, end, xCompare);
        } else if (currCoordinate_ == 1) {
            std::nth_element(begin, begin + n, end, yCompare);
        } else {
            std::nth_element(begin, begin + n, end, zCompare);
        }
        object_ = *(begin + n);
        right_ = NULL;
        left_ = new KdTree(begin, begin + n, nextCoordinate_(currCoordinate_));
        box_ = BoundBox(object_->getBox(), left_->getBox());
        if (end != begin + 2) {
            right_ = new KdTree(begin + n + 1, end, nextCoordinate_(currCoordinate_));
            box_ = BoundBox(box_, right_->getBox());
        }
    } else {
        object_ = *begin;
        box_ = object_->getBox();
        left_ = NULL;
        right_ = NULL;
    }
}

Intersect KdTree::intersectRay(Ray ray, KdTree *node)
{
    if(!node || !node->getBox().intersect(ray)) {
        return Intersect();
    }
    Intersect inter = node->getObject()->intersectRay(ray), leftInter = intersectRay(ray, node->left_),
            rightInter = intersectRay(ray, node->right_);
    long double dist, leftDist, rightDist;
    if(!inter.getResult()) { dist = 1.7E+307; }
    else { dist = (inter.getPoint() - ray.getBegin()).length(); }
    if(!leftInter.getResult()) { leftDist = 1.7E+307; }
    else { leftDist = (leftInter.getPoint() - ray.getBegin()).length(); }
    if(!rightInter.getResult()) { rightDist = 1.7E+307; }
    else { rightDist = (rightInter.getPoint() - ray.getBegin()).length(); }
    if (dist <= leftDist + EPS && dist <= rightDist + EPS) {
        return inter;
    }
    if(leftDist <= dist + EPS && leftDist <= rightDist + EPS) {
        return leftInter;
    }
    if(rightDist <= dist + EPS && rightDist <= leftDist + EPS) {
        return rightInter;
    }
}

SolidObject *KdTree::getObject() {
    return object_;
}

KdTree *KdTree::getLeft() {
    return left_;
}

KdTree *KdTree::getRight() {
    return right_;
}

int KdTree::getCoordinate() {
    return currCoordinate_;
}
