# paraCl
ParaCL (paraC language) is a relatively simple programming language  
The goal is to develop frontend and simulator of this language

## Example Program
```paraCL
// Computes n Fibonacci numbers
a = 0;
b = 1;
i = 0
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

[Progress and Internals](./DEVELOPMENT.md)

