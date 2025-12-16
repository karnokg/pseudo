# pseudo

## About project (ver2)

Pseudo is a theoretical 'programming language' designed to introduce first year
students to algorithms and data structures in a language-agnostic way at Obuda
University.

Most of the time, this language-agnostic choice causes more trouble for
students, since the language has its own quirks. This project aims to
bridge this gap by allowing students to compile and debug algorithms
straight from the lecture notes!

This project was my Bsc thesis project, so version 1 implementation is
questionable. I gained a lot of experience in the past 3 years as a C++
programmer, therefore my goal is to refactor this project to meet
production-quality standards.


### Refactoring goals:

- use the board so issues are trackable in the git log
- fix build system weakness: introduce conan as a dependency manager
- add support to multiple compilers: gcc, clang, and msvc
- use correct memory management
- reevaluate current architecture design
- upgrade thirdparties to recent versions
- add documentation
- research GUI systems: add option for graphical debugging and a way to visualize memory layouts

## About project (ver1)

### Features

### Types
- [x] Integer
- [x] Rational
- [x] String
- [x] Logical
- [x] Variable length arrays
- [x] ~~Pointer~~

### Syntax

### Example codes

### TODOs
- [x] Use cmake
- [x] Restructure project folder like `https://gitlab.com/CLIUtils/modern-cmake/-/tree/master/examples/extended-project/src`
- [x] Rewrite building source section
- [x] Lexer
	- [x] Tab based indentation
	- [x] Tokens
    - [x] ~~Add ENDOFFILE token~~
- [x] Parser (Grammar)
	- [x] Variable declaration statement
	- [x] Function declaration statement
	- [x] Variable assignment statement
	- [x] Integer variable type
	- [x] Rational variable type
	- [x] Logical variable type
	- [x] Binary operators
	- [x] Identifiers
	- [x] If statement
	- [x] For statements
	- [x] Return statements
	- [x] Vectors
	- [x] Function calls

- [x] Semantics
	- [x] Tab based indentation only, error on space!
- [x] Code generation
- [x] Debug informations

- [x] Tests

## Technical stuff

### Requirements

* flex 2.6.4
* bison 3.8.2
* LLVM version 14.0.6
* (Doxygen 1.9.1 and graphviz 2.43.0 for pretty diagrams)

### Building the Source code

Only tested on linux!

1. Checkout Pseudo:
	* `git clone https://github.com/karnokg/pseudo.git`

2. Create build directory:
	* `mkdir -p build`

3. Run CMake:
	* `cd build`
	* `cmake ..`
	* `cmake --build .`

3. Done!

### Run your code written in Pseudo

The executable dumps to LLVM IR to err stream, which can be directed into a file.
Once you've got the file, you can compile it with clang.

* To executable:
    * `./pseudo <filename> 2> program.ll`
	* `clang program.ll -g -O0 -o program.out externfunctions.cpp`
	* `./program.out`

* Run directly:
    * `./pseudo <filename> --run`

* To debug:
	* `lldb-15 program.out`
	* `gdb program.out`
	* ...



