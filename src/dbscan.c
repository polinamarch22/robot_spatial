#include "dbscan.h"
#include <stdlib.h>
#include <math.h>

// Евклидово расстояние
static double dist(Point a, Point b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

// Поиск всех точек в радиусе eps
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

// Расширение кластера рекурсивно
static void expand_cluster(Point* points, size_t n, int* labels, int idx, int cluster_id, double eps, int minPts) {
    size_t neighbor_count;
    int* neighbors = region_query(points, n, points[idx], eps, &neighbor_count);
    if (neighbor_count < minPts) {
        labels[idx] = -1;  // Шум
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

// Основная функция DBSCAN
int* dbscan(Point* points, size_t n, double eps, int minPts) {
    int* labels = calloc(n, sizeof(int));  // 0 - непосещённая точка
    int cluster_id = 0;
    for (size_t i = 0; i < n; i++) {
        if (labels[i] != 0) continue;  // Уже обработана
        size_t neighbor_count;
        int* neighbors = region_query(points, n, points[i], eps, &neighbor_count);
        if (neighbor_count < minPts) {
            labels[i] = -1;  // Шум
        } else {
            cluster_id++;
            expand_cluster(points, n, labels, i, cluster_id, eps, minPts);
        }
        free(neighbors);
    }
    return labels;
}