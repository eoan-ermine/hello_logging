# hello_logging


## Build

```shell
mkdir build && cd build
conan install .. -of .
cmake --preset conan-release ..
cmake --build .
```