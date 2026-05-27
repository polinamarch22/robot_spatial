#ifndef DBSCAN_H
#define DBSCAN_H

#include <stddef.h>   // <-- ЭТУ СТРОКУ ДОБАВИТЬ
#include "utils.h"
#include "kdtree.h"

int* dbscan(Point* points, size_t n, double eps, int minPts);

#endif