#pragma once

#include "linux/io_uring.hpp"
#include "linux/os_string.hpp"
#include "linux/stat.h"
#include "linux/syscall/SysRes.hpp"
#include "Map.hpp"
#include "mem/Allocator.hpp"
#include "mem/Arena.hpp"
#include "metap/structs.hpp"
#include "Vec.hpp"
#include "writer.hpp"

struct Mimetype {
    ImOsString extension;
    ImOsString mimetype;
};

static Mimetype mimetype_map [] = {
    // Common Text Types
    Mimetype("txt", "text/plain"),
    Mimetype("html", "text/html"),
    Mimetype("htm", "text/html"),
    Mimetype("css", "text/css"),
    Mimetype("js", "text/javascript"),
    Mimetype("json", "application/json"),
    Mimetype("xml", "application/xml"),
    Mimetype("csv", "text/csv"),
    Mimetype("rtf", "application/rtf"),

    // Images
    Mimetype("png", "image/png"),
    Mimetype("jpg", "image/jpeg"),
    Mimetype("jpeg", "image/jpeg"),
    Mimetype("gif", "image/gif"),
    Mimetype("bmp", "image/bmp"),
    Mimetype("svg", "image/svg+xml"),
    Mimetype("webp", "image/webp"),
    Mimetype("tiff", "image/tiff"),

    // Audio
    Mimetype("mp3", "audio/mpeg"),
    Mimetype("wav", "audio/wav"),
    Mimetype("ogg", "audio/ogg"),
    Mimetype("mp4", "audio/mp4"),
    Mimetype("aac", "audio/aac"),
    Mimetype("flac", "audio/flac"),

    // Video
    Mimetype("mp4", "video/mp4"),
    Mimetype("avi", "video/x-msvideo"),
    Mimetype("mov", "video/quicktime"),
    Mimetype("mpeg", "video/mpeg"),
    Mimetype("webm", "video/webm"),
    Mimetype("mkv", "video/x-matroska"),

    // Documents
    Mimetype("pdf", "application/pdf"),
    Mimetype("doc", "application/msword"),
    Mimetype("docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"),
    Mimetype("xls", "application/vnd.ms-excel"),
    Mimetype("xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"),
    Mimetype("ppt", "application/vnd.ms-powerpoint"),
    Mimetype("pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"),
    Mimetype("odt", "application/vnd.oasis.opendocument.text"),
    Mimetype("ods", "application/vnd.oasis.opendocument.spreadsheet"),
    Mimetype("odp", "application/vnd.oasis.opendocument.presentation"),

    // Archives
    Mimetype("zip", "application/zip"),
    Mimetype("rar", "application/x-rar-compressed"),
    Mimetype("gz", "application/gzip"),
    Mimetype("tar", "application/x-tar"),
    Mimetype("7z", "application/x-7z-compressed"),

    // Fonts
    Mimetype("ttf", "font/ttf"),
    Mimetype("woff", "font/woff"),
    Mimetype("woff2", "font/woff2"),
    Mimetype("otf", "font/otf"),

    // Other
    Mimetype("ico", "image/x-icon"),
    Mimetype("css", "text/css"),
    Mimetype("js", "text/javascript"),
    Mimetype("json", "application/json"),
    Mimetype("xml", "application/xml"),
    Mimetype("rss", "application/rss+xml"),
    Mimetype("atom", "application/atom+xml"),
    Mimetype("sql", "text/x-sql"),
    Mimetype("c", "text/x-c"),
    Mimetype("cpp", "text/x-c++"),
    Mimetype("java", "text/x-java"),
    Mimetype("php", "application/x-httpd-php"),
    Mimetype("py", "text/x-python"),
    Mimetype("sh", "application/x-sh"),
    Mimetype("md", "text/markdown"),
    Mimetype("pdf", "application/pdf"),
    Mimetype("exe", "application/x-msdownload"),
    Mimetype("msi", "application/x-msdownload"),
};

struct MappedFile {
    VAL_STRUCT(MappedFile, fd, Fd(), file_size, 0_usize, mimetype, reinterpret_cast<Mimetype *>(0))
};

struct DirStack {
    VAL_STRUCT(DirStack, cd, Fd(), path, OsString())
};

struct FileServer {
    PIN_STRUCT(FileServer, host_dir, OsString(), fs_map, (Map<OsString, MappedFile>()))

    static Mimetype *mimetype_from_file(ImOsString file) {
        Mimetype *mime = &mimetype_map [0];
        usize i        = file.len() - 1;
        for (;;) {
            if (*file [i] == '.') break;
            if (*file [i] == '/' || i == 0) return mime;
            i -= 1;
        }

        for (auto &m : mimetype_map) {
            if (!m.extension.len()) break;
            usize file_ext_len = file.len() - i - 1;
            if (file_ext_len == m.extension.len() &&
                !memcmp(m.extension.ptr(), file [i + 1], m.extension.len())) {
                mime = &m;
                break;
            }
        }

        return mime;
    }

    static inline __attribute__((always_inline)) open_how open_config(u64 flags) {
        open_how oh;
        memset(&oh, 0, sizeof(oh));
        oh.flags = flags;
        return oh;
    }

    SysRes<None> load_dir_iterative(
        this FileServer &self, IoUring *r, Allocator<Arena> *pa, Allocator<Arena> *ta, Fd root_cd,
        const OsString &root_path, Slice<OsString> headers
    ) {
        Vec<DirStack> stack = TRY(Vec<DirStack>::init(ta, 64));
        if (stack.push(ta, DirStack(root_cd, root_path))) return SysResKind::NOMEM;

        u32 files_count = 0;
        auto foh        = FileServer::open_config(O_RDONLY);
        auto doh        = FileServer::open_config(O_RDONLY | O_DIRECTORY);
        while (stack.len) {
            DirStack current_task = TRY(stack.pop());
            Fd cd                 = current_task.cd;
            OsString path         = current_task.path;

            Slice<u8> dirent_buf =
                ta->alloc_aligned((sizeof(linux_dirent64) + 256) * 128, alignof(linux_dirent64));
            auto *dirent_ptr = reinterpret_cast<linux_dirent64 *>(dirent_buf.ptr);
            usize read       = TRY(getdents64(cd, dirent_ptr, static_cast<u32>(dirent_buf.len)));

            for (usize idx = 0; idx < read;) {
                auto *el        = reinterpret_cast<linux_dirent64 *>(dirent_buf.ptr + idx);
                ImOsString name = ImOsString::init(el->d_name);

                if ((name.len() == 2 && *name [0] == '.') ||
                    (name.len() == 3 && *name [0] == '.' && *name [1] == '.')) {
                    idx += el->d_reclen;
                    continue;
                }

                Allocator<Arena> *a  = el->d_type == DT_DIR ? ta : pa;
                Slice<char> obj_path = a->template alloc<char>(path.len() + name.len());
                bufwrite(obj_path, path.as_slice(), "/", name);

                if (el->d_type == DT_REG) {
                    IO_TRY_ADD(r->openat2(
                        reinterpret_cast<usize>(obj_path.ptr), AT_FDCWD, obj_path.ptr, &foh, false
                    ));
                    files_count += 1;
                } else if (el->d_type == DT_DIR) {
                    Fd subdir_cd = TRY(openat2(AT_FDCWD, obj_path.ptr, &doh, sizeof(open_how)));
                    if (stack.push(ta, DirStack(subdir_cd, OsString::init(obj_path))))
                        return SysResKind::NOMEM;
                }

                idx += el->d_reclen;
            }

            IO_TRY_ADD(r->skip_success(r->close(0, cd)));
        }

        Slice<CqeRes> results = ta->template alloc<CqeRes>(files_count);
        if (results.ptr == null) return SysResKind::NOMEM;
        TRY(r->submit_and_wait(files_count));
        TRY(r->copy_cqes(results, files_count));

        Slice<statx_t> files_statx = ta->template alloc<statx_t>(files_count);
        if (files_statx.ptr == null) return SysResKind::NOMEM;
        for (usize i = 0; i < files_count; i++) {
            CqeRes cqe_res = IR_TRY(*results [i]);
            Fd new_fd      = Fd(cqe_res.res);
            char *fpath    = reinterpret_cast<char *>(cqe_res.user_data);
            if (self.fs_map.insert<Arena>(
                    pa, OsString::init(fpath),
                    MappedFile(new_fd, i, FileServer::mimetype_from_file(OsString::init(fpath)))
                ) != MapInsertRes::Success)
                return SysResKind::NOMEM;
            IO_TRY_ADD(
                r->statx(cqe_res.user_data, new_fd, "", AT_EMPTY_PATH, STATX_SIZE, files_statx [i])
            );
        }

        TRY(r->submit_and_wait(files_count));
        TRY(r->copy_cqes(results, files_count));
        for (usize i = 0; i < files_count; i++) {
            CqeRes cqe_res          = IR_TRY(*results [i]);
            char *fpath             = reinterpret_cast<char *>(cqe_res.user_data);
            MappedFile *mapped_file = self.fs_map.get(OsString::init(fpath));
            if (mapped_file == null) [[unlikely]]
                return SysResKind::OWNERDEAD;

            usize size             = files_statx [mapped_file->file_size]->stx_size;
            mapped_file->file_size = size;
        }

        return None();
    }

    SysRes<None> load_dir(
        this FileServer &self, IoUring *r, Allocator<Arena> *pa, Allocator<Arena> *ta,
        Slice<OsString> headers
    ) {
        open_how oh      = {.flags = O_DIRECTORY | O_RDONLY, .mode = 0, .resolve = 0};
        Fd cd            = TRY(openat2(AT_FDCWD, self.host_dir.ptr(), &oh, sizeof(open_how)));

        SysRes<None> res = self.load_dir_iterative(r, pa, ta, cd, self.host_dir, headers);
        IO_TRY_SUBMIT((*r), r->skip_success(r->close(0, cd)));

        return res;
    }

    MappedFile *find(this FileServer &self, ImOsString path) {
        return self.fs_map.get(path);
    }

    static SysRes<FileServer> init(Allocator<Arena> *pa, ImOsString host_dir) {
        FileServer self;
        self.fs_map   = TRY((Map<const char *, MappedFile>::init(pa, 1024)));
        self.host_dir = OsString::init(pa->dup(host_dir.as_slice_with_nullchar()));
        if (self.host_dir.is_empty()) [[unlikely]] {
            self.fs_map.deinit(pa);
            return SysResKind::NOMEM;
        }

        return self;
    }

    SysRes<None> deinit(this FileServer &self, Allocator<Arena> *pa) {
        self.fs_map.deinit(pa);

        return None();
    }
};
