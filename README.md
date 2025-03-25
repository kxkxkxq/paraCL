# paraCl
ParaCL (paraC language) is a relatively simple programming language  
The goal is to develop frontend and simulator of this language

## Example Program
```paraCL
// Computes n-th Fibonacci number
a = 0;
b = 1;
n = ?; // Input from user

while (n > 0) {
  temp = a;
  a = b;
  b = b + temp;
  n = n - 1;
}
print b;
```

```markdown
[Progress and Internals](DEVELOPMENT.md)

