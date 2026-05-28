#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"
#include "kdtree.h"
#include "fuzzy_cmeans.h"
#include "dbscan.h"

int main(int argc, char* argv[]) {
    // Проверка аргументов
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <file.csv> <-kd_nearest x,y | -cmeans k | -dbscan eps,minPts>\n", argv[0]);
        return 1;
    }
    
    // Чтение точек из файла
    size_t n;
    Point* points = read_points(argv[1], &n);
    if (!points) {
        perror("Failed to read points");
        return 1;
    }
    
    // ===== 1. ПОИСК БЛИЖАЙШЕЙ ТОЧКИ =====
    if (strcmp(argv[2], "-kd_nearest") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Need target point: x,y\n");
            return 1;
        }
        double x, y;
        sscanf(argv[3], "%lf,%lf", &x, &y);
        Point target = {x, y};
        
        // Построение дерева
        KDNode* tree = NULL;
        for (size_t i = 0; i < n; i++)
            tree = kd_insert(tree, points[i], 0);
        
        // Поиск и вывод
        Point nearest = kd_nearest(tree, target);
        printf("Nearest point: %.6f,%.6f\n", nearest.x, nearest.y);
        kd_free(tree);
    }
    
    // ===== 2. FUZZY C-MEANS =====
    else if (strcmp(argv[2], "-cmeans") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Need number of clusters\n");
            return 1;
        }
        int c = atoi(argv[3]);
        Point* centroids = malloc(c * sizeof(Point));
        
        // Запуск кластеризации
        double** U = fuzzy_cmeans(points, n, c, 2, 100, 1e-5, centroids);
        
        // Вывод центров кластеров
        printf("Centroids:\n");
        for (int i = 0; i < c; i++)
            printf("%.6f,%.6f\n", centroids[i].x, centroids[i].y);
        
        // Очистка памяти
        for (int i = 0; i < c; i++) free(U[i]);
        free(U);
        free(centroids);
    }
    
    // ===== 3. DBSCAN =====
    else if (strcmp(argv[2], "-dbscan") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Need eps,minPts\n");
            return 1;
        }
        double eps;
        int minPts;
        sscanf(argv[3], "%lf,%d", &eps, &minPts);
        
        // Запуск DBSCAN
        int* labels = dbscan(points, n, eps, minPts);
        
        // Вывод меток кластеров
        printf("Cluster labels:\n");
        for (size_t i = 0; i < n; i++)
            printf("Point %zu: cluster %d\n", i, labels[i]);
        
        free(labels);
    }
    
    // ===== НЕИЗВЕСТНАЯ ОПЕРАЦИЯ =====
    else {
        fprintf(stderr, "Unknown operation: %s\n", argv[2]);
    }
    
    free(points);
    return 0;
}