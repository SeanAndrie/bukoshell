# 🥥 *buko*shell

*As beautiful as a coconut shell — and twice as hard to crack! (probably)*

This project is a small Bash-like shell that can parse and execute user commands, handle built-in functions, manage environment variables, run external programs, and support pipes, redirections, and proper signal handling.

## Features
- **Command parsing** with single/double quotes, `$` variable expansion, and `$?` exit status  
- **Built-in functions**: `echo` (-n), `cd`, `pwd`, `export`, `unset`, `env`, `exit`  
- **Environment variable management** (from `environ`, dynamic updates)  
- **External program execution** via `PATH` lookup and `execve`  
- **Pipes** (`|`) with multi-stage chaining  
- **Redirections**: `<`, `>`, `>>`, `<<` (heredoc with delimiter expansion control)  
- **Signal handling**: `SIGINT` (Ctrl+C), `SIGQUIT` (Ctrl+\) — parent ignores, children behave like Bash  
- **Robust error handling** and exit codes (126, 127, syntax errors)  
- **Memory safety**: No leaks, no undefined behavior — Valgrind clean  

### Bonus Features (as per 42 subject)
These are **optional** and **not required** for the base project:

- **Wildcard expansion** — supports `*` (matches any sequence in filenames)  
- **Logical operators** — `&&` (and), `||` (or) with short-circuit evaluation  
- **Subshells** — `(cmd1 && cmd2)` with isolated environment and I/O

## Compilation & Usage

```bash
# Clone repo and enter
git clone https://github.com/SeanAndrie/bukoshell.git
cd bukoshell

# Initialize and update libft submodule
git submodule update --init --recursive

# Compile bukoshell
make

# Run bukoshell
./minishell
