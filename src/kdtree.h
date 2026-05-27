#ifndef KDTREE_H
#define KDTREE_H

#include "utils.h"

typedef struct KDNode {
    Point point;
    struct KDNode *left, *right;
    int axis;
} KDNode;

KDNode* kd_create_node(Point p, int axis);
KDNode* kd_insert(KDNode* root, Point p, int depth);
Point kd_nearest(KDNode* root, Point target);
void kd_free(KDNode* root);

#endif
