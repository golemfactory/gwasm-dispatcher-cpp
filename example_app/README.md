# Example app

This is an example app that uses gwasm_dispatcher_cpp.

It uses a separate meson.build file to test the full process of building
gwasm_dispatcher_cpp library and then building an application with it.


## Compilation for native execution

``` shell
prefix_path="${HOME}/gwasm_native_prefix"
cd ..
meson setup . build --prefix "$prefix_path"
ninja -C build install

PKG_CONFIG_PATH="${prefix_path}/lib64/pkgconfig" \
    meson setup example_app build_example_app --prefix "$prefix_path"
ninja -C build_example_app install
```


## Compilation for gwasm-runner execution

- cd ..
- use meson >= 0.52
- create a prefix path
- add prefix/include/nlohmann/json.hpp
- create a `wasm.txt` file:
``` ini
[paths]
prefix = '/a/path/to/gwasm_prefix/'

[binaries]
c = '/a/path/to/emscripten/emcc'
cpp = '/a/path/to/emscripten/em++'
ar = '/a/path/to/emscripten/emar'
pkgconfig = '/usr/bin/pkg-config'

[properties]
cpp_args = ['-I/a/path/to/gwasm_prefix/include']

[host_machine]
system = 'emscripten'
cpu_family = 'wasm32'
cpu = 'wasm32'
endian = 'little'
```
- run commands

``` shell
meson setup . build_wasm --cross-file wasm.txt -Ddefault_library=static
ninja -C build_wasm install

PKG_CONFIG_PATH="/a/path/to/gwasm_prefix/lib/pkgconfig" \
    meson setup example_app build_wasm_example_app --cross-file wasm.txt
ninja -C build_wasm_example_app install
```

