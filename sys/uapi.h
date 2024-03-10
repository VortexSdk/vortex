#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include <linux/futex.h>
#include <linux/mman.h>
#include <linux/posix_types.h>
#include <linux/sched.h>
#include <linux/types.h>

typedef struct futex_waitv futex_waitv;
typedef struct robust_list robust_list;
typedef struct robust_list_head robust_list_head;
typedef struct cachestat_range cachestat_range;
typedef struct cachestat cachestat;
typedef struct clone_args clone_args;

#pragma clang diagnostic pop
