# Blue2th [![Language: C](https://img.shields.io/badge/Language-C-brightgreen.svg)](https://en.wikipedia.org/wiki/C_(programming_language)) [![Builder: CMake](https://img.shields.io/badge/Builder-CMake-brightgreen.svg)](https://cmake.org/)  [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT)

## Introduction:

Blue2th is a bluetooth proxy API on top of libbluetooth.

## Requirements:

libbluetooth must be available on your system:
```
sudo apt install -y libbluetooth-dev
```

## Setting up blue2th example:

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

## Example Usage:

The blue2th example as been created to demonstrate the usage of the API

Launch blue2th:
```
$>./blue2th
```

## Output:

/!\ XX:XX:XX:XX:XX:XX represents bluetooth 48-bit device address
/!\ xxxxxxxxxxxxxxxxx represents bluetooth user friendly string name

```
First local bluetooth controller:[hci0][XX:XX:XX:XX:XX:XX]
4 bluetooth device has been found.
[XX:XX:XX:XX:XX:XX][xxxxxxxxxxxxxxxxx]
[XX:XX:XX:XX:XX:XX][xxxxxxxxxxxxxxxxx]
[XX:XX:XX:XX:XX:XX][xxxxxxxxxxxxxxxxx]
[XX:XX:XX:XX:XX:XX][xxxxxxxxxxxxxxxxx]
Bluetooth iface [SelDeGuérandeAOC] not found.
```

## API Description:

[blue2th.h](https://github.com/adugast/blue2th/blob/master/src/blue2th.h) - Description of the blue2th API

## References:

* [Bluetooth programming](http://people.csail.mit.edu/albert/bluez-intro/) - An Introduction to Bluetooth Programming by Albert Huang (2005-2008)
* [Bluetooth for Programmers](http://people.csail.mit.edu/rudolph/Teaching/Articles/BTBook.pdf) - Bluetooth book by Albert Huang and Larry Rudolph (2005)
* [Basic of bluetooth](https://opensourceforu.com/2015/06/linux-without-wires-the-basics-of-bluetooth/) - Linux without wires the basics of bluetooth
* [hcitool](https://github.com/pauloborges/bluez/blob/master/tools/hcitool.c) - hcitool source code
* [cltest](https://github.com/pauloborges/bluez/blob/master/tools/cltest.c) - Code example
* [bluez programming](http://docs.cubieboard.org/tutorials/common/development/bluez_programming) - Bluetooth C Programming Quick Start
* [Libblueman](https://github.com/blueman-project/blueman/blob/master/module/libblueman.c) - Libblueman

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

