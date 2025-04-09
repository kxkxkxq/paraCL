# paraCl
ParaCL (paraC language) is a relatively simple C-like programming language with current grammar in the scope of : arithmetic operations, conditional statements and loops 

## Example Program
```paraCL
// Computes n Fibonacci numbers
a = 0;
b = 1;
i = 0;
n = ?; // Input from user

while (i < n) 
{
  print a;
  temp = a + b;
  a = b;
  b = temp;
  i = i + 1;
}
```

## Short description 
The programme implements a frontend for paraCL, and also it a simulator. 

After the input file has been correctly parse processed it is immediately executed.

Also implemented diagnostics of errors in lexical, syntactic and semantic analysis and error reporting during the process of input file execution.

## How to install
use 
```bush
git clone git@github.com:kxkxkxq/paraCL.git

cd ./paraCL/
```

## How to build
use 
```bush
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build

cmake --build build
```

## How to run
general view of the programme call 
```bush
./build/paraCL <filename>
```

to run end to end tests use 
```bush
ctest --test-dir ./build/tests/end-to-end-tests/
```

[Progress and Internals](./DEVELOPMENT.md)
