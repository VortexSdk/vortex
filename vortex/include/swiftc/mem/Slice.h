#pragma once

#include "../numbers.h"

typedef struct Slice {
    void *ptr;
    usize len;
} Slice;
