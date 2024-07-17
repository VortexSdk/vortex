# Vortex
This is a std library for Zig that's focused on performance and minimalism. It currently only supports x86_64 and aarch64 Linux. This project is open for contribution and extending this project to other operating systems. However, 32bit architectures will never be supported.

### Usage
#### 1. Add Vortex to your `build.zig.zon`
```zig
.{
    .name = "<your_apps_name>",
    .version = "<your_apps_vesion>",
    .dependencies = .{
        // Vortex v0.1.0
        .vortex = .{
            .url = "https://github.com/VortexSdk/vortex/archive/refs/tags/v0.4.0.tar.gz",
            .hash = "12208f395f60bbb782d91309c7b6d5d0da64a41db784e227f28dfd22ae996c05abaa",
        },
    },
}
```
#### 2. Add Vortex to your `build.zig`
```zig
const vortex = b.dependency("vortex", .{
    .target = target,
    .optimize = optimize,
});
exe.addModule("vortex", Vortex.module("vortex"));
```

### License
This library is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
