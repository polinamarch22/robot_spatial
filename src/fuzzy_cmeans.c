#include "fuzzy_cmeans.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

// Евклидово расстояние между двумя точками
static double distance(Point a, Point b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

// Алгоритм Fuzzy C-means
double** fuzzy_cmeans(Point* points, size_t n, int c, int m, int max_iter, double eps, Point* centroids) {
    // Выделение памяти под матрицу принадлежности
    double** U = malloc(c * sizeof(double*));
    for (int i = 0; i < c; i++)
        U[i] = malloc(n * sizeof(double));
    
    // Инициализация центроидов случайными точками
    for (int i = 0; i < c; i++) {
        centroids[i] = points[rand() % n];
    }
    
    // Основной цикл алгоритма
    for (int iter = 0; iter < max_iter; iter++) {
        // Обновление матрицы принадлежности
        for (int i = 0; i < c; i++) {
            for (size_t j = 0; j < n; j++) {
                double d_ij = distance(centroids[i], points[j]);
                if (d_ij < 1e-10) d_ij = 1e-10;
                double sum = 0;
                for (int k = 0; k < c; k++) {
                    double d_kj = distance(centroids[k], points[j]);
                    if (d_kj < 1e-10) d_kj = 1e-10;
                    sum += pow(d_ij / d_kj, 2.0/(m-1));
                }
                U[i][j] = 1.0 / sum;
            }
        }
        
        // Обновление центроидов
        Point new_centroids[c];
        for (int i = 0; i < c; i++) {
            double sum_u = 0, sum_ux = 0, sum_uy = 0;
            for (size_t j = 0; j < n; j++) {
                double u_pow = pow(U[i][j], m);
                sum_u += u_pow;
                sum_ux += u_pow * points[j].x;
                sum_uy += u_pow * points[j].y;
            }
            new_centroids[i].x = sum_ux / sum_u;
            new_centroids[i].y = sum_uy / sum_u;
        }
        
        // Проверка сходимости
        double change = 0;
        for (int i = 0; i < c; i++) {
            change += distance(centroids[i], new_centroids[i]);
            centroids[i] = new_centroids[i];
        }
        if (change < eps) break;
    }
    return U;
}