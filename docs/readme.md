# vodka documentation

## Introduction

Welcome to the documentation for the Vodka transcoder and vodka-lib, the C++ library designed to provide gateway for Vodka code transcodage to Kernel Code to the Vodka transcoder and other third-party programs. We recommand you read the [Vodka codebase documentation](https://github.com/lolo859/vodka/docs/vodka-codebase.md) first before aptempting to use 

## Dependencies

The Vodka transcoder and vodka-lib have the following dependencies in order to be build :
- termcolor by ikalnytskyi
- nlohmann/json
- base85
- xxHash by Cyan4973
- XoshiroCpp by Reputeless
- blake3

All the files are included into the repository.

## Installation

For the moment, there aren't any official package file so you will have ether to use the binaries included in each release or compile yourself the program. We provide a .sh file for easier compiling.

### vodka

To install vodka, you can clone the repository:

```sh
git clone https://github.com/lolo859/vodka.git
```

Compile the program with:

```sh
comp.sh
```

This generate a static executable in the working directory. Please ensure you are in the root folder of the repository.

Warning: the compiling script use g++ for compiling each file. Compiling with other compiler hasn't been tested and aren't officialy supported.

### vodka-lib

To install vodka-lib, you can clone the repository:

```sh
git clone https://github.com/lolo859/vodka.git
```

Include the header in your C++ source file:

```cpp
#include "vodka-lib.h"
```

Compile the library with ```comp.sh library```. Don't forget to include the .a file by passing ```-L. -lvodka``` to your compiler. Please ensure you are in the root folder of the repository.

## References

Please see any of these file for more details:
- [Vodka language syntax and codebase](https://github.com/lolo859/vodka/docs/vodka-codebase.md)
- [Kernel code syntax and codebase](https://github.com/lolo859/vodka/docs/kernel-codebase.md)
- [vodka-lib docs](https://github.com/lolo859/vodka/docs/vodka-lib-usage.md)

## License

This project is licensed under the GNU General Public License v3.0 (GPLv3).  
For full details, please refer to the [LICENSE](./LICENSE) file included in this repository.  You can also use `--license` on the executable file to see the license.

Note that this project depends on third-party libraries all of which are compatible with the GPLv3 license.