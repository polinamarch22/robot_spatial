#include "csv_reader.h"
#include <stdio.h>
#include <stdlib.h>

Point* read_points(const char* filename, size_t* count) {
    FILE* f = fopen(filename, "r");
    if (!f) return NULL;
    Point* points = malloc(sizeof(Point) * 1000);
    *count = 0;
    double x, y;
    while (fscanf(f, "%lf,%lf", &x, &y) == 2) {
        points[*count].x = x;
        points[*count].y = y;
        (*count)++;
        if (*count % 1000 == 0)
            points = realloc(points, sizeof(Point) * (*count + 1000));
    }
    fclose(f);
    return points;
}
