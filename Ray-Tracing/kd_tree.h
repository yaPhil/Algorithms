#ifndef KD_TREE_H
#define KD_TREE_H
#pragma once
#include"solid_object.h"
#include<vector>
#include<algorithm>

class KdTree {
public:
    KdTree(std::vector<SolidObject*>::iterator begin, std::vector<SolidObject*>::iterator end, int currCoord);

    Intersect intersectRay(Ray ray, KdTree *node);

    SolidObject* getObject();
    KdTree* getLeft();
    KdTree* getRight();
    BoundBox getBox();
    int getCoordinate();
private:
    KdTree *left_, *right_;
    SolidObject *object_;
    BoundBox box_;
    int currCoordinate_;

    int nextCoordinate_(int curr) {
        return (curr + 1) % 3;
    }

};

#endif // KD_TREE_H
