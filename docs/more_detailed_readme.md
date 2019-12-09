# Gwasm runner interface

[gwasm runner](https://github.com/golemfactory/gwasm-runner/) expects
a binary implements the
[interface](https://github.com/golemfactory/gwasm-runner/wiki/Wasm-runner-interface).
This library implements required argument parsing and handling files.

The user of this library is required to implement 3 callables: split,
execute and merge.


## Split

The required interface of split callable:

``` c++
Iterable<SplitResultTuple> split(int argc, char** argv, SplitContext& context);
```

Every element of the returned iterable represents a single computation
task. It's a tuple of any elements. It's only required that they must
be serializable and deserializable using
[nlohmann/json](https://github.com/nlohmann/json) library.

To save data to a file, use `context.new_file()`. It returns an
`gwasm::Output` object, which can be used to write data. If you
convert it to a `gwasm::Blob` by using `to_blob` method and return
it in a tuple, the contents of this file, will be transfered for
execution.

If your execution step is supposed to return any data in a file, you
must create a placeholder file for results. To do this, return the
output object directly, without converting to blob.


## Execute

The required intefrace of execute callable:

``` c++
ExecuteResultTuple execute(const SplitResultTupleArgs&...);
```

The arguments must have the same types as tuple elements, returned from
split. This is checked at compilation time.

The result tuple may, as in split, contain any types, that are
serializable to json.

When returning data, by writing to a file, use `gwasm::Output`
objects, created in split step. Then convert the object to blob, and
return it in result tuple.


## Merge

The required intefrace of merge callable:

``` c++
void merge(
    int argc, char** argv,
    std::vector<std::pair<SplitResultTuple, ExecuteResultTuple>>&& results);
```

The results vector contains pairs of tuples, where first one is from split, second
from execute. This is also checked at compilation time.
