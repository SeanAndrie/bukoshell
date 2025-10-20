## 1. Initialization

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

are highly-dependent on these fields since they hold essential information about the system environment (i.e., system binary path).