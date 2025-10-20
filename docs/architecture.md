# Architecture 

This section provides a high-level overview of the overall data-flow and execution steps involved in the program's interactive mode (from parsing, execution, to memory management). Project structure and design principles are also covered.

## 1. Overall Data Flow

### 1.1. Initialization

In a project with multiple subsystems involved (lexer, parser, execution, etc.), it is essential to have a central point from which they can store and retrieve information. A main shell structure is implemented to do just that. Conceptually, it primarily functions as an intermediary storage of data for the parser and executor.

The main shell structure (`t_shell`) includes the following fields:

|Field|Data Type|Description|
|-|-|-|
|`map`|`struct s_map`|Hashmap for environment variables|
|`line`|`char *`|Line of text returned by `readline` from the user|
|`head`|`struct s_token`|Head node of the token linked-list|
|`root`|`struct s_node`|Root node of the abstract syntax tree|
|`status`|`int`|Last exit status|
|`envp`|`char **`|A copy of the parent process' environment|
|`cwd`|`char [PATH_MAX]`| A string buffer to store the current working directory| 

At startup, the structure is first allocated and its fields are initialized. The major fields that are first initialized at this point are `map` and `envp`, this is because much of the shell's core 
functionality such as 

- *parameter and wildcard expansion*; and
- *built-in and external command execution*

are highly-dependent on these fields since they hold essential information about the system environment in order for them to function as intended (i.e., system binary path).

### 1.2. Parsing

Once the main structure has been initialized, a formatted prompt is displayed onto the terminal from which it can now receive input from the user. Parsing user input is mainly done through *lexical analysis*.

### - Lexical Analysis 

This phase of parsing involves reading through the input and extracting meaningful chunks and categorizing them into specific token types. Instead of assigning each token to a flat enum in isolation, Bukoshell uses bitmask-based categories to express hierchical and relational properties of tokens.

This means each token can belong to multiple categories simultaneously, allowing for more expressive and efficient parsing logic.

### - Category Masks (High-Level Groups and Markers)

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

### - Specific Token Types (Composite Masks)

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

````C
t_bool	is_token_type(t_token_type type, unsigned int category_mask)
{
	return ((type & category_mask) == category_mask);
}
````

This performs a bit AND operation on the given token `type` and `category_mask` which returns `TRUE` if the `category_mask` is present in `type`, `FALSE` if otherwise.
