#include "kdtree.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>

KDNode* kd_create_node(Point p, int axis) {
    KDNode* node = (KDNode*)malloc(sizeof(KDNode));
    node->point = p;
    node->left = node->right = NULL;
    node->axis = axis;
    return node;
}

static double dist_sq(Point a, Point b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx*dx + dy*dy;
}

KDNode* kd_insert(KDNode* root, Point p, int depth) {
    if (!root) {
        return kd_create_node(p, depth % 2);
    }
    int axis = root->axis;
    if ((axis == 0 && p.x < root->point.x) || (axis == 1 && p.y < root->point.y))
        root->left = kd_insert(root->left, p, depth + 1);
    else
        root->right = kd_insert(root->right, p, depth + 1);
    return root;
}

static void nearest(KDNode* root, Point target, KDNode** best, double* best_dist, int depth) {
    if (!root) return;
    double d = dist_sq(root->point, target);
    if (d < *best_dist) {
        *best_dist = d;
        *best = root;
    }
    int axis = depth % 2;
    KDNode *first, *second;
    if ((axis == 0 && target.x < root->point.x) || (axis == 1 && target.y < root->point.y)) {
        first = root->left;
        second = root->right;
    } else {
        first = root->right;
        second = root->left;
    }
    nearest(first, target, best, best_dist, depth + 1);
    double diff = (axis == 0) ? target.x - root->point.x : target.y - root->point.y;
    if (diff * diff < *best_dist) {
        nearest(second, target, best, best_dist, depth + 1);
    }
}

Point kd_nearest(KDNode* root, Point target) {
    if (!root) return (Point){0,0};
    KDNode* best = root;
    double best_dist = dist_sq(root->point, target);
    nearest(root, target, &best, &best_dist, 0);
    return best->point;
}

void kd_free(KDNode* root) {
    if (!root) return;
    kd_free(root->left);
    kd_free(root->right);
    free(root);
}