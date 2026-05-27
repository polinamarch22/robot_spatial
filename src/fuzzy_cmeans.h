#ifndef FUZZY_CMEANS_H
#define FUZZY_CMEANS_H

#include <stddef.h>   // <-- ЭТУ СТРОКУ ДОБАВИТЬ
#include "utils.h"

double** fuzzy_cmeans(Point* points, size_t n, int c, int m, int max_iter, double eps, Point* centroids);

#endif
