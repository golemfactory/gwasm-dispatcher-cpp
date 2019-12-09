# gwasm dispatcher cpp

`gwasm_dispatcher_cpp` is a library that makes it easy to run an application
with
[gwasm_runner](https://github.com/golemfactory/gwasm-runner/wiki/Wasm-runner-interface).

There is also a similar library for rust:
[gwasm-dispatcher](https://golemfactory.github.io/gwasm-runner/gwasm_dispatcher/index.html).


## Usage

``` c++
#include <gwasm.hpp>

std::vector<std::tuple<gwasm::Blob, int, gwasm::Output>>
my_splitter(int argc, char* argv[], gwasm::SplitContext& context);

std::tuple<int, gwasm::Blob>
my_executor(gwasm::Blob&& blob, int i, gwasm::Output&& output);

void
my_merger(int argc,
          char* argv[],
          std::vector<std::pair<std::tuple<gwasm::Blob, int, gwasm::Output>,
                                std::tuple<int, gwasm::Blob>>>&& results);

int main(int argc, char* argv[])
{
    return gwasm::run(argc, argv, my_splitter, my_executor, my_merger);
}
```

See [docs/more_detailed_readme.md](docs/more_detailed_readme.md) for more
details.

You can run the executable manually, according to the
[gwasm runner_interface](https://github.com/golemfactory/gwasm-runner/wiki/Wasm-runner-interface),
or compile to wasm and run with
[gwasm-runner](https://github.com/golemfactory/gwasm-runner#running-a-wasm-binary)
itself.


## Dependencies

For using the library:

  - a compiler supporting C++17.
  - [nlohmann/json](https://github.com/nlohmann/json)

Additionally, For running the test suite: [Boost](https://www.boost.org/).


## Installation

Use [meson](https://mesonbuild.com/) for building and installing:

``` shell
meson setup . build
ninja -C build
sudo ninja -C build install
```


## Code style

Code style is managed by
[clang-format](https://clang.llvm.org/docs/ClangFormat.html).


## Testing

Run tests using ninja:

``` shell
ninja -C build test
```


## License

[LGPLv3 or later](https://www.gnu.org/licenses/lgpl-3.0.html)
