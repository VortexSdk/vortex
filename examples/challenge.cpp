#define VORTEX_NO_ARGS      true
#define VORTEX_NO_NAMESPACE true

#include <vortex/vortex.hpp>

static SysRes<None> main_w() {
    IoUring r = TRY(IoUring::init(4096, IORING_SETUP_SINGLE_ISSUER | IORING_SETUP_SQPOLL));
    TRY(r.register_sparse_files(1024));

    PageAllocator p1    = TRY(PageAllocator::init(256));
    PageAllocator p2    = TRY(PageAllocator::init(256));
    Allocator<Arena> pa = Allocator<Arena>::init(p1.len, p1.ptr);
    Allocator<Arena> ta = Allocator<Arena>::init(p2.len, p2.ptr);

    FsMap fm            = TRY(FsMap::init(&pa, &r, 0, 0, 0, "vortex"));
    TRY(fm.load(&pa, &ta));

    TRY(p1.deinit());
    TRY(p2.deinit());
    TRY(r.deinit());

    return None();
}

static u8 main() {
    SysRes<None> res = main_w();
    if (res.is_err()) {
        eprintln("Server failed to start with the code: ", static_cast<u8>(res.kind));
        eprintln("File: ", res.file, " Line: ", res.line);

        return 1;
    }

    return 0;
}
