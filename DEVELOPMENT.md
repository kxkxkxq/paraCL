## Current implementation status

### Lexical Analyzer
- Implemented token recognition for:
  - Keywords: `if`, `while`, `print`
  - Identifiers: ASCII alphabetic names
  - Literals: integer constants
  - Operators: 
    - Arithmetic: `+`, `-`, `*`, `/`, etc.
    - Relational: `>`, `<`, `>=`, `<=`, `==`, `!=`
    - Assignment: `=`
  - Delimiters: `;`, `{`, `}`, `(`, `)`, `,`

### Syntax Parser
- Currently parses:
  - Arithmetic expressions with operator precedence
  - Variable declarations and assignments
  - Block statements (scopes)
  - Basic I/O operations:
    - Input statement: `variable = ?;`
    - Output statement: `print expression;`

- In development:
  - Control flow structures:
    - Conditional statements (`if-else`)
    - Loop statements (`while`)
  - Symbol table management
  - Error diagnostics system
  - AST generation and visualization

### Simulator 
- Currently executes:
  - All arithmetic operations
  - Variable assignments
  - Print statements

- Pending implementation:
  - Control flow simulation
  - Input statement processing
  - Runtime diagnostics
  - Advanced debugging features
