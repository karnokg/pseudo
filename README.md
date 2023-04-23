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
	- [ ] ~~Add ENDOFFILE token~~
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
	- [x] Matrices
	- [x] Function calls

- [ ] Semantics 
	- [x] Tab based indentation only, error on space!
- [ ] Code generation
- [ ] Debug informations

- [ ] Tests
 
- [ ] GUI to run + debug

## Technical stuff

### Requirements

* flex 2.6.4
* bison 3.8.2
* LLVM version
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

### Compiling your code written in Pseudo

1. In terminal: 
	* `./pseudo <filename>`

2. Run compiled code:
	* `TODO`


