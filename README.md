# Pascal-Like Language Parser & Interpreter

A C++ implementation of a lexical analyzer, recursive-descent parser, parse tree, and interpreter for a Pascal-like programming language.

The project demonstrates the major stages involved in processing a small programming language: source code is tokenized with Flex, parsed according to a grammar, represented as a parse tree, and then interpreted by recursively evaluating the tree.

## Features

* Lexical analysis using **Flex**
* Hand-written **recursive-descent parser** in C++
* Parse tree construction
* Parse tree interpretation
* Symbol table for variables
* `INTEGER` and `REAL` variables
* Variable declarations and assignments
* Arithmetic expressions
* Comparison and logical operators
* `IF / ELSE` statements
* `WHILE` loops
* Nested loops and compound statements
* `READ` input
* `WRITE` output
* Syntax-error reporting with line and lexeme information
* Example programs for normal and error-case testing

## Technologies

* **C++11**
* **Flex**
* **GNU Make**
* Pascal-like programming language

## How It Works

```text
Pascal-like Source Code
          |
          v
        Flex
          |
          v
     Token Stream
          |
          v
Recursive-Descent Parser
          |
          v
      Parse Tree
          |
          v
     Interpreter
          |
          v
        Output
```

### 1. Lexical Analysis

`src/exp-rules.l` contains the Flex rules used to recognize keywords, identifiers, literals, punctuation, operators, and other tokens.

The token definitions are shared through `src/lexer.h`.

### 2. Parsing

`src/parser.cpp` implements a recursive-descent parser based on the language grammar. The parser recognizes program structure, variable declarations, assignments, expressions, conditionals, loops, input, and output.

### 3. Parse Tree

`src/parse_tree_nodes.cpp` and `src/parse_tree_nodes.h` define the nodes used to represent the parsed program. The tree separates syntax from execution and allows the program to be interpreted by traversing the nodes.

### 4. Interpretation

The interpreter evaluates the parse tree and maintains a symbol table containing variables and their values. Statements such as assignments, conditionals, loops, `READ`, and `WRITE` are executed during the tree traversal.


Generated files are intentionally kept out of the repository. Running `make` creates the `build/` directory and generated Flex source automatically.

## Requirements

You need the following installed and available on your `PATH`:

* `g++`
* `flex`
* `make`

### Windows

The project works well in an **MSYS2 UCRT64** environment. Make sure the GCC, Flex, and Make packages are installed in the environment you use to build the project.

### Linux

Install your distribution's C++ compiler, Flex, and GNU Make packages. For Debian/Ubuntu-based systems, for example:

```bash
sudo apt update
sudo apt install g++ flex make
```

## Building

From the repository root, run:

```bash
make
```

The executable will be created at:

```text
build/tips
```

On Windows/MSYS2, you can run the generated executable with:

```bash
./build/tips
```

To remove generated files:

```bash
make clean
```

## Running an Example

Pass a Pascal-like source file as the final command-line argument:

```bash
./build/tips examples/1-hello.pas
```

Example output:

```text
INFO: Using the examples/1-hello.pas file for input
*** Interpret the Tree ***
Hello Happy World!
```

Programs that require user input can be run the same way and will prompt for values using `READ(...)`.

## Example Programs

The `examples/` directory contains programs designed to exercise different parts of the language implementation:

| File                | Purpose                                                       |
| ------------------- | ------------------------------------------------------------- |
| `1-hello.pas`       | Basic output / Hello World                                    |
| `2-areas.pas`       | Variables, real values, arithmetic, and input                 |
| `3-bad_prog.pas`    | Invalid program for syntax/error testing                      |
| `4-if_convert.pas`  | `IF / ELSE` and arithmetic expressions                        |
| `5-multiples.pas`   | `WHILE` loops and multiplication                              |
| `6-lotsovar.pas`    | Large variable declarations and duplicate-declaration testing |
| `7-bmi_calc.pas`    | Arithmetic, input, real values, and nested conditionals       |
| `8-mult_table.pas`  | Nested `WHILE` loops and conditionals                         |
| `9-input_test.pas`  | Input, arithmetic, loops, and logical expressions             |
| `10-threedim.pas`   | Three nested loops                                            |
| `11-divisors.pas`   | Nested loops, arithmetic, and comparisons                     |
| `12-not_sample.pas` | Logical `NOT` expressions                                     |

## Command-Line Options

The driver contains support for optional debugging/diagnostic flags used during development, including options for parse output, parse-tree output, symbol-table output, and tree-deletion output.

These options are controlled in `src/driver.cpp` and the associated parser/tree implementation.

## Concepts Demonstrated

This project applies several core compiler and programming-language concepts:

* Lexical analysis and tokenization
* Context-free grammars
* Recursive-descent parsing
* Parse-tree construction
* Symbol tables
* Expression evaluation
* Interpretation
* Syntax-error handling
* Object-oriented C++ design
* Dynamic memory management
* Makefiles and build automation

## Author

**Joseph Knowles**

Computer Science

