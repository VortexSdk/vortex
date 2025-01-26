#pragma once

#include "numbers.hpp"

#ifndef VORTEX_NO_NAMESPACE
namespace vortex {

#include "Array.hpp"
#include "Cloneable.hpp"
#include "debug.hpp"
#include "fsmap.hpp"
#include "linux/io_uring.hpp"
#include "linux/net.hpp"
#include "linux/signal.hpp"
#include "linux/syscall/syscall.hpp"
#include "linux/thread.hpp"
#include "Map.hpp"
#include "mem/mem.hpp"
#include "metap/metap.hpp"
#include "string.hpp"
#include "Vec.hpp"
#include "writer.hpp"

} // namespace vortex
#else
#include "Array.hpp"
#include "Cloneable.hpp"
#include "debug.hpp"
#include "fsmap.hpp"
#include "linux/io_uring.hpp"
#include "linux/net.hpp"
#include "linux/signal.hpp"
#include "linux/syscall/syscall.hpp"
#include "linux/thread.hpp"
#include "Map.hpp"
#include "mem/mem.hpp"
#include "metap/metap.hpp"
#include "start.hpp"
#include "string.hpp"
#include "Vec.hpp"
#include "writer.hpp"
#endif

#include "ns.hpp"
#include "start.hpp"
