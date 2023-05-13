# pseudo

## About project

### Features

### Types
- [x] Integer
- [x] Rational
- [x] ~~String~~ 
- [x] Logical
- [x] Array
- [ ] ~~Pointer~~

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

- [ ] Tests

## Technical stuff

### Requirements

* flex 2.6.4
* bison 3.8.2
* LLVM version 14.0.6
* (Doxygen 1.9.1 and graphviz 2.43.0 for pretty diagrams)

### Building the Source code

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
Once you've got the file, you can compile it with the clang.

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



