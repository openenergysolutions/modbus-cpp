# modbus-cpp [![CircleCI](https://circleci.com/gh/aegis4ics/modbus-cpp/tree/master.svg?style=svg&circle-token=e0a370a9fddf904b93ff2d29f54ed36371ef0eb3)](https://circleci.com/gh/aegis4ics/modbus-cpp/tree/master)

Modbus TCP master library in modern C++.

## Requirements
- [CMake](https://cmake.org/download/) 3.8 and higher
- A modern C++ compiler supporting at least C++14

## Building
1. Clone the repository recursively with `git clone https://github.com/aegis4ics/modbus-cpp.git --recursive`

2. Create a `build` directory inside the project (all folders starting with `build` are ignored by git)

3. Inside the build directory, configure the project using `cmake ..` or the interactive `ccmake ..` The default options should work out of the box.

4. Build the project with `cmake --build .`

5. Execute the example with `./example/modbus_example`

## Tests
To run the tests, simply execute `ctest .` in your build directory.

**Note**: `AsioTcpConnectionTest` requires that the port 8888 is available. If for any reason this port is not available, either skip the tests by appending `~AsioTcpConnection` to the command line arguments, or change the `test_port` variable in `AsioTcpConnectionTest`.

To run performance tests, simply build and run target `modbus_perf_tests`. It will create 1000 channels (from port 8000 to 8999) with a scheduled request every 2 seconds. An easy way to have 1000 slave devices is to use [Tony Rogvall Modbus implementation in Erlang](https://github.com/tonyrog/modbus) and start the slave devices with the following code (of course, `modbus` application must be started) :

```
load() ->
    load(0).

load(1000) -> ok;
load(Count) ->
    Port = Count + 8000,
    modbus_tcp_server:start_link([{port, Port}, {callback, modbus_demo_callback}]),
    load(Count + 1).
```

## Documentation
The documentation of the `master` branch is available online here: https://aegis4ics.github.io/modbus-cpp/

To build the basic Doxygen documentation, simply run `doxygen` in the root directory of the repository.
The documentation will be generated in the `build/doc` folder.

To build the documentation with the [custom theme](http://mcss.mosra.cz/doxygen/), the following dependencies are needed:
* Doxygen 1.8.14 or higher
* Python 3.6
* Jinja2 (installable by `pip`)
* Pygments (installable by `pip`)
* m.css submodule (available in this repository)

A Docker image with all the required dependency is available here: https://hub.docker.com/r/emgre/circleci-doxygen/

To build the documentation with the theme, simply run `deps/m.css/doxygen/dox2html5.py Doxyfile-mcss`. The documentation will be generated in
the `build/doc` folder.
