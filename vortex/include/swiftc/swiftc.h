#pragma once

#ifndef SWIFTC_EXTERN
    // Adds the `static` keyword before every function declartion.
    #define FNDECL_PREFIX static
#else
    // Adds the `extern` keyword before every function declartion.
    #define FNDECL_PREFIX extern
#endif

#include "debug.h"
#include "math.h"
#include "mem/mem.h"
#include "numbers.h"
#include "os/os.h"
