# Scratchpad

Main To-Do:

1. Token Validation [PRIORITY]
2. Subshell Handling
3. Environment Variables
4. Builtins
5. Execution

Extra: Signal Handling Unclosed Quotes (PS2)

Parsing Stuff:

1. Environment Variables for Parameters
2. Signal Handling Unclosed Quotes (PS2)
3. Token Validation
3. Subshell Handling for AST

$ (echo hello || echo bye) && echo done

1. ||
2. AND or OR

( -> echo -> hello -> || -> echo -> bye -> ) -> | -> echo -> done -> && -> echo -> something

                        &&
            left (S1)        right (S2)

S1: echo -> hello -> || -> echo : N_SUBSHELL
S2: echo -> done : N_COMMAND


                        &&
            ||
            right (S2)

---

Lexer Context:
    - Structural Context: 
        + Parentheses:  
        + Quotes
    
    - Procedural Context:
        + Command-dependent: ||, &&, |
        + FD-dependent: <, <<, >>, > 

Recursive Descent for Parsing (?)

Simple Commands: [CMD] [ARGS] [REDIR_OP] [FILENAME]
Compound Commands: [LPAREN] (SIMPLE COMMAND) [RPAREN]

A command would often be structured as something like:

(SIMPLE COMMAND) (CTRL_OP) (SIMPLE_COMMAND)

- We parse initial simple command separately
- We consume ctrl op then try to parse the following commands recursively
