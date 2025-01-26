#pragma once

#include "linux/io_uring.hpp"
#include "linux/syscall/SysRes.hpp"
#include "Map.hpp"
#include "mem/Allocator.hpp"
#include "metap/structs.hpp"
#include "writer.hpp"

struct FsMap {
    PIN_STRUCT(
        FsMap, host_dir, Slice<char>(), off, Fd(), wd, 0_i32, br, IoUringBufRing(), fs_map,
        (Map<const char *, Fd>())
    )

    template <AllocatorStrategy U>
    SysRes<None> load_dir_recursive(
        this FsMap &self, Allocator<U> *pa, Allocator<U> *ta, Fd cd, const Slice<char> &path
    ) {
        open_how foh = {.flags = O_RDONLY, .mode = 0, .resolve = 0};
        open_how doh = {.flags = O_DIRECTORY | O_RDONLY, .mode = 0, .resolve = 0};
        Slice<u8> dirent_buf =
            ta->alloc_aligned((sizeof(linux_dirent64) + 256) * 128, alignof(linux_dirent64));
        auto *dirent_ptr = reinterpret_cast<linux_dirent64 *>(dirent_buf.ptr);
        usize read       = TRY(getdents64(cd, dirent_ptr, static_cast<u32>(dirent_buf.len)));
        for (usize idx = 0; idx < read;) {
            auto *el               = reinterpret_cast<linux_dirent64 *>(dirent_buf.ptr + idx);
            Slice<const char> name = Slice<const char>::init(strlen(el->d_name) + 1, el->d_name);
            if ((name.len == 2 && *name [0] == '.') ||
                (name.len == 3 && *name [0] == '.' && *name [1] == '.')) {
                idx += el->d_reclen;
                continue;
            }

            Allocator<U> *a      = el->d_type == DT_DIR ? ta : pa;
            Slice<char> obj_path = a->template alloc<char>(path.len + name.len);
            memcpy(obj_path.ptr, path.ptr, path.len - 1);
            obj_path.ptr [path.len - 1] = '/';
            memcpy(obj_path.ptr + path.len, name.ptr, name.len);

            if (el->d_type == DT_REG) {
                if (self.fs_map.insert<U>(
                        pa, obj_path.ptr,
                        TRY(openat2(AT_FDCWD, obj_path.ptr, &foh, sizeof(open_how)))
                    ) != MapInsertRes::Success)
                    return SysResKind::NOMEM;
            } else if (el->d_type == DT_DIR) {
                Fd subdir_cd = TRY(openat2(AT_FDCWD, obj_path.ptr, &doh, sizeof(open_how)));
                TRY(self.load_dir_recursive(pa, ta, subdir_cd, obj_path));
            }

            idx += el->d_reclen;
        }

        return None();
    }

    template <AllocatorStrategy U>
    SysRes<None> load_dir(this FsMap &self, Allocator<U> *pa, Allocator<U> *ta) {
        open_how oh = {.flags = O_DIRECTORY | O_RDONLY, .mode = 0, .resolve = 0};
        Fd cd       = TRY(openat2(AT_FDCWD, self.host_dir.ptr, &oh, sizeof(open_how)));

        return self.load_dir_recursive(pa, ta, cd, self.host_dir);
    }

    template <AllocatorStrategy U>
    SysRes<None> process_cqe(this FsMap &self, Allocator<U> *pa, const CqeRes &cqe) {
        Slice<u8> raw_buf = self.br.get_buf(cqe);
        Slice<inotify_event> buf =
            Slice<inotify_event>::init(raw_buf.len, reinterpret_cast<inotify_event *>(raw_buf.ptr));
        for (usize i = 0; i < buf.len; i++) {
            inotify_event b = (*buf [i]);
            if (b.mask == 0) break;

            println(reinterpret_cast<const char *>((*buf [i]).name));
        }
        self.br.recycle_buf(cqe);

        return None();
    }

    Fd *find(this FsMap &self, const char *path) {
        return self.fs_map.get(path);
    }

    template <AllocatorStrategy U>
    static SysRes<FsMap>
    init(Allocator<U> *pa, IoUring *r, u64 read_ud, Fd off, u16 bgid, const char *host_dir) {
        FsMap self;
        self.off = TRY(inotify_init1(IN_NONBLOCK));
        self.wd  = static_cast<i32>(TRY(inotify_add_watch(
            self.off, host_dir, IN_MODIFY | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO
        )));

        TRY(r->update_file(off, self.off, 0));
        self.off        = off;
        self.br         = TRY(IoUringBufRing::init(r, bgid, 1024, 10));
        auto fs_map_res = Map<const char *, Fd>::init(pa, 1024);
        if (fs_map_res.is_err()) {
            self.br.deinit(r);
            return ErrSysRes(fs_map_res.kind);
        }

        self.fs_map   = fs_map_res.unwrap();
        self.host_dir = pa->dup(Slice<const char>::init(strlen(host_dir) + 1, host_dir));
        if (self.host_dir.ptr == null) [[unlikely]] {
            self.br.deinit(r);
            self.fs_map.deinit(pa);
            return SysResKind::NOMEM;
        }
        self.host_dir.ptr [self.host_dir.len - 1] = '\0';

        // TODO: HANDLE THE ERROR.
        IO_TRY_SUBMIT((*r), r->read_multishot(read_ud, off, bgid));

        return self;
    }

    SysRes<None> deinit(this FsMap &) {
        return None();
    }
};
