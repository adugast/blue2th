# Blue2th [![Language: C](https://img.shields.io/badge/Language-C-brightgreen.svg)](https://en.wikipedia.org/wiki/C_(programming_language)) [![Builder: CMake](https://img.shields.io/badge/Builder-CMake-brightgreen.svg)](https://cmake.org/)  [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT)

## Introduction

Blue2th is a bluetooth proxy API.

## Setting up blue2th example

Clone the project to retrieve the sources:
```
$>git clone https://github.com/adugast/blue2th.git
```

Go in the build directory of blue2th:
```
$>cd blue2th/build/
```

Build the project using CMake (from the build directory):
```
$>cmake ..
```

Finally, use make to compile the sources and so generate the binary (still from the build directory):
```
$>make
```

## API Description (blue2th.h content):

```
```

## Example Usage:

Launch blue2th:
```
$>./blue2th
```

## References:

* [Bluetooth programming](http://people.csail.mit.edu/albert/bluez-intro/) - An Introduction to Bluetooth Programming
* [hcitool](https://github.com/pauloborges/bluez/blob/master/tools/hcitool.c) - hcitool source code
* [cltest](https://github.com/pauloborges/bluez/blob/master/tools/cltest.c) - Code example
* [bluez programming](http://docs.cubieboard.org/tutorials/common/development/bluez_programming) - Bluetooth C Programming Quick Start

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

