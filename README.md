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

5. Run the tests with `ctest .`

6. Execute the example with `./example/modbus_example`