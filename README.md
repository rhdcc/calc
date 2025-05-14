# calc

Attempts to evaluate simple mathematical expressions using the shunting yard algorithm.

## Information about the usage of 'calc'

- The parser only supports the following four binary operations: addition (+), subtraction (-), multiplication (*) and division (/).
  - The unary negation operator is not supported.
- The system in the lexer to find the numerical lexemes is not comphrehensive.

## Building and Running

This project uses the [clang](https://clang.llvm.org/) compiler, but really, you can use any C compiler you want that supports the C11 standard.

I included build scripts that you can run in a terminal to build the project (`build.bat` for Windows, `build.sh` for Linux).

This will create a new directory named `bin`, and within it, an executable named `calc` (`calc.exe` on Windows).

Then simply run `calc` (or `calc.exe`) to start the repl!

### Windows
```console
> build.bat
> bin\calc.exe
```

### Linux
```
$ ./build.sh
$ bin/calc
```

## Resources
 - https://en.wikipedia.org/wiki/Shunting_yard_algorithm
