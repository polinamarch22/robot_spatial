#ifndef CSV_READER_H
#define CSV_READER_H

#include "utils.h"
#include <stddef.h>

Point* read_points(const char* filename, size_t* count);

#endif
