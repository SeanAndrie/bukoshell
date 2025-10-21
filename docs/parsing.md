# Parsing

Once the main structure has been initialized, a formatted prompt is displayed onto the terminal from which it can now receive input from the user. Parsing user input is mainly done through *lexical analysis*. Below is a general workflow for parsing (in order):

1. **Lexer** - scan through input line and create *lexemes*.
2. **Tokenizer** - categorize lexemes into token types and store in linked list.

	- **Normalization** - concatenates adjacent `TOKEN_WORD` tokens, strips `TOKEN_WHITESPACE` tokens, applies expansions (wildcard, parameter, or tilde), and applies group contexts (subshell or arithmetic).
	- **Validation** - validates token list syntax. 

3. **Abstract Syntax Tree** - creates a hierarchical syntactic tree structure from tokens.  

# 1. Lexer

This phase of parsing involves reading through the input and extracting syntactically meaningful chunks and categorizing them into specific token types. Instead of assigning each token to a flat enum in isolation, Bukoshell uses bitmask-based categories to express hierchical and relational properties of tokens.

This means each token can belong to multiple categories simultaneously, allowing for more expressive and efficient parsing logic.

## 1.1. Category Masks (High-Level Groups and Markers)

|Category Mask|Bit Value|Meaning|
|-|-|-|
|`TOKEN_WORD`|`0x01`|Regular word (command, argument, or filename)|
|`TOKEN_QUOTE`|`0x02`|Quoted word (`'word'`, `"word"`)|
|`TOKEN_CTRL_OP`|`0x04`|Control operator (e.g., `AND`, `OR`, `PIPE`)|
|`TOKEN_REDIR_OP`|`0x08`|Redirection operator (`<`, `>`, `>>`, `<<`)|
|`TOKEN_METACHAR`|`0x10`|Metacharacter symbols used by the shell|
|`TOKEN_ARITH`|`0x20`|Arithmetic expansion context ( `(( ... ))` )|
|`TOKEN_SUBSHELL`|`0x40`|Subshell content ( `( ... )` )|
|`TOKEN_GROUP`|`0x100`|Grouping token (parentheses in generall)|
|`TOKEN_GROUP_OPEN`|`0x200`|Opening group token ( `(` )|
|`TOKEN_GROUP_CLOSE`|`0x400`|Closing group token ( `)` )|
|`TOKEN_WHITESPACE`|`0x800`|Whitespace token (for spacing rules)|
|`TOKEN_PARAMETER`|`0x1000`|Parameter or variable (`$VAR`)|
|`TOKEN_AFTER_REDIR`|`0x2000`|Marks a word following a redirection|
|`TOKEN_NONE`|`0x4000`| Undefined/placeholder token|

## 1.2. Specific Token Types (Composite Masks)

|Token Type|Composition|Description|
|-|-|-|
|`T_WORD`|`TOKEN_WORD`|Generic unquoted word|
|`T_WORD_SQUOTE`|`TOKEN_WORD` `TOKEN_QUOTE`|Word inside single quotes|
|`T_WORD_DQUOTE`|`TOKEN_WORD` `TOKEN_QUOTE`|Control operator (e.g., `AND`, `OR`, `PIPE`)|
|`T_PIPE`|`TOKEN_CTRL_OP` `TOKEN_METACHAR`|Pipe operator|
|`T_AND`|`TOKEN_CTRL_OP` `TOKEN_METACHAR`|Logical AND|
|`T_OR`|`TOKEN_CTRL_OP` `TOKEN_METACHAR`|Logical OR|
|`T_BACKGROUND`|`TOKEN_CTRL_OP` `TOKEN_METACHAR`|Background execution (currently not supported)|
|`T_REDIR_IN`|`TOKEN_REDIR_OP` `TOKEN_METACHAR`|Input redirection (`<`)|
|`T_REDIR_OUT`|`TOKEN_REDIR_OP` `TOKEN_METACHAR`|Output redirection (`>`)|
|`T_REDIR_APPEND`|`TOKEN_REDIR_OP` `TOKEN_METACHAR`|Append redirection (`>>`)|
|`T_HEREDOC`|`TOKEN_REDIR_OP` `TOKEN_METACHAR`|Heredoc (`<<`)|
|`T_LPAREN`|`TOKEN_GROUP` `TOKEN_GROUP_OPEN` `TOKEN_METACHAR`|Left parenthesis|
|`T_RPAREN`|`TOKEN_GROUP` `TOKEN_GROUP_CLOSE` `TOKEN_METACHAR`|Right parenthesis|
|`T_PARAMETER`|`TOKEN_WORD` `TOKEN_PARAMETER`|Variable or substitution (`$VAR`)|

Each bit in a bitmask corresponds to a unique power of two so that each flag can be represented and combined using bitwise operations without overlap. In order to identify if a token belongs to a specific group of tokens or *is* a particular token type, we use the following function:

```C
t_bool	is_token_type(t_token_type type, unsigned int category_mask)
{
	return ((type & category_mask) == category_mask);
}
```

This performs a bit AND operation on the given token `type` and `category_mask` and checks if the result is equal to our `category_mask`, indicating that `category_mask` is present in `type`.

# 2. Tokenizer

The extraction, categorization, and organization of these syntactically meaningful chunks is called *tokenization*. The tokens generated from this process are stored in a linked list (`t_token`). 

Below are some simple prompts along with a visualization of their corresponding linked list:

```bash
bukoshell [user@hostname ~] ❯ echo hello world

Tokens:
[echo : WORD] -> [hello : WORD] -> [world : WORD]

bukoshell [user@hostname ~] ❯ echo "foo" 'bar'

Tokens:
[echo : WORD] -> [foo : WORD_DQUOTE] -> [bar : WORD_SQUOTE]

bukoshell [user@hostname ~] ❯ cat Makefile > out

Tokens:
[cat : WORD] -> [Makefile : WORD] -> [> : REDIR_OUT] -> [out : WORD]
```

## 2.1. Normalization

### Adjacent Word Token Concatenation

This describes the behaviour of concatenating consecutive adjacent word-like tokens. For example:

```bash
bukoshell [user@hostname ~] ❯ echo foo"bar"'bazz'

Tokens:
[echo : WORD] -> [foobarbazz : WORD]
```

According to the Bash manual, a 'word' is **a sequence of characters treated as a unit** (words may not include unquoted metacharacters). In practice, many shells allow adjacent literal segments (e.g., quoted + unquoted) to form a single command-word argument. Though the Bash manual doesn't explicitly say 'adjacent word tokens are concatenated', we've implemented this behaviour in our normalization step to match observed shell semantics.

Concatenation is done by:

1. Traversing through the token list.
2. If there are two adjacent `TOKEN_WORD` tokens, 
	- pop all consecutive matching tokens from the list,
	- create a new token from concatenating the lexemes of all the popped tokens,
	- insert the new token back into the list.
3. Otherwise, continue traversal.

Token types with the `TOKEN_WORD` category mask include: `T_WORD`, `T_WORD_SQUOTE`, `T_WORD_DQUOTE`, and `T_PARAMETER`. It is to note that `T_WHITESPACE` tokens will still be present during this step to preserve proper spacing.

### Expansions

Bukoshell supports three types of expansions: (1) *parameter* (`$`), (2) *wildcard* (`*`), and (3) and *tilde* (`~`) expansion.

**Parameter expansion** refers to replacing variable references (like `$USER` or `$?`) with their values before executing the command. For example:

```bash
bukoshell [user@hostname ~] ❯ echo $USER

Tokens:
[echo : WORD] -> [seang : PARAMETER]

Syntax Tree:
└── Command: echo seang
```

Parameter expansion has the following quoting rules:
- If the parameter is enclosed in *double quotes*, allow expansion.
- If parameter is enclosed in *single quotes*, skip expansion.

Parameters can either be in the form of sole `T_PARAMETER` tokens or parameters enclosed in double quotes (`T_WORD_DQUOTE`). 

For sole `T_PARAMETER` tokens, we simply use the variable as a key and check if an entry that corresponds to it already exists in our shell environment hash map. If an entry is found, we replace the token's lexeme with the key's value. Otherwise, we leave the lexeme as is and continue to traverse the list. 

For enclosed parameters, since quote characters are stripped beforehand when tokens are created initially, we can safely create tokens out of the target token's lexeme. Hence, turning each parameter instance into a sole `T_PARAMETER` token. We can then traverse through this list and expand parameters normally.

**Wildcard expansion** is the process of matching a given pattern with special characters against filenames in the current working directory. For the bonus part of this project, we're only meant to handle the asterisk (`*`) character which can represent zero or more characters.

Example:

```bash
bukoshell [seang@deus-ex-machina Temporary] ❯ echo *
file1 file2 file3

bukoshell [seang@deus-ex-machina Temporary] ❯ echo .*
.dotfile1 .dotfile2 .dotfile3

bukoshell [seang@deus-ex-machina Temporary] ❯ echo invalid-pattern*
invalid-pattern*
```
```
```
```

For pattern matching, we use iterative backtracking. This process works by scanning both the pattern and the string from left to right while remembering the position of the last `*` in the pattern.

When characters match normally, both pointers advance. When a `*` is encountered,

    - the algorithm skips over it,
    - records where it occurred in the pattern, and
    - remembers the corresponding position in the string via a backtracking pointer.

If a mismatch happens later,

    - it backtracks to the last `*`,
    - shifts the backtrack pointer one character forward in the string, and
    - retries the match from there.

This process repeats until the pattern and string both reach the end successfully (match) or all backtracking options are exhausted (no match).

**Tilde expansion** involves replacing a tilde (`~`) at the start of a word with a specific directory path -- most commonly, a user's home directory. Bukoshell currently only implemented tilde expansion on

- the user's home directory (`~`),
- the current working directory (`~+`), and
- the previous working directory (`~-`).

Remainders directly after the tilde character are also preserved.

Example:

```bash
bukoshell [seang@deus-ex-machina bukoshell] ❯ echo ~
/home/seang

bukoshell [seang@deus-ex-machina bukoshell] ❯ echo ~/Desktop
/home/seang/Desktop
```

