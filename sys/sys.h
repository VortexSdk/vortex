#pragma once

#ifdef __APPLE__
    #define PAGE_SIZE (16 * 1024)
#else
    #define PAGE_SIZE (4 * 1024)
#endif

#include "linux/linux.h"
