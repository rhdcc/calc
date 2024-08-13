# calc

Attempts to evaluate simple mathematical expressions using the shunting yard algorithm.

## Information about the usage of 'calc'

- The parser only supports the following four binary operations: addition (+), subtraction (-), multiplication (*) and division (/).
  - The unary negation operator is not supported.
- The system in the lexer to find the numerical lexemes is not comphrehensive.

## Building and Running

This project uses the [clang](https://clang.llvm.org/) compiler, but really, you can use any C compiler you want that supports the C11 standard.

Since I use Windows, I included a `build.bat` file that you can run in the command prompt to build the project (If you are using Windows).

This will create a new directory named `bin`, and within it, an executable named `calc.exe`.

Then simply run `calc.exe` to start the repl!

```console
> build.bat
> bin\calc.exe
```

## Resources
 - https://en.wikipedia.org/wiki/Shunting_yard_algorithm
