#include "dbscan.h"
#include <stdlib.h>
#include <math.h>

static double dist(Point a, Point b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

static int* region_query(Point* points, size_t n, Point p, double eps, size_t* count) {
    int* neighbors = malloc(n * sizeof(int));
    *count = 0;
    for (size_t i = 0; i < n; i++) {
        if (dist(p, points[i]) <= eps) {
            neighbors[(*count)++] = i;
        }
    }
    return neighbors;
}

static void expand_cluster(Point* points, size_t n, int* labels, int idx, int cluster_id, double eps, int minPts) {
    size_t neighbor_count;
    int* neighbors = region_query(points, n, points[idx], eps, &neighbor_count);
    if (neighbor_count < minPts) {
        labels[idx] = -1;
        free(neighbors);
        return;
    }
    labels[idx] = cluster_id;
    for (size_t i = 0; i < neighbor_count; i++) {
        int n_idx = neighbors[i];
        if (labels[n_idx] == 0) {
            expand_cluster(points, n, labels, n_idx, cluster_id, eps, minPts);
        }
    }
    free(neighbors);
}

int* dbscan(Point* points, size_t n, double eps, int minPts) {
    int* labels = calloc(n, sizeof(int));
    int cluster_id = 0;
    for (size_t i = 0; i < n; i++) {
        if (labels[i] != 0) continue;
        size_t neighbor_count;
        int* neighbors = region_query(points, n, points[i], eps, &neighbor_count);
        if (neighbor_count < minPts) {
            labels[i] = -1;
        } else {
            cluster_id++;
            expand_cluster(points, n, labels, i, cluster_id, eps, minPts);
        }
        free(neighbors);
    }
    return labels;
}
