#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

#include "numbers.hpp"

namespace vortex {

// Important Ordering.
#include "metap/metap.hpp"

// Important Ordering.
#include "linux/syscall.hpp"

// Important Ordering.
#include "debug.hpp"
#include "mem/mem.hpp"

} // namespace vortex

#include "start.hpp"

#pragma clang diagnostic pop
