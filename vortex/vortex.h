#pragma once

#ifndef EXTERN_VORTEX
    // Adds the `static` keyword before every function declartion.
    #define VORTEX_PREFIX static
#else
    // Adds the `extern` keyword before every function declartion.
    #define VORTEX_PREFIX extern
#endif

#include "debug.h"
#include "math.h"
#include "mem/mem.h"
#include "numbers.h"
#include "thread/thread.h"
#include <sys/sys.h>
