# Shell Quote Handling: Design and Logic

## Overview

When building a shell, handling quotes (`'` and `"`) in user input is essential for supporting multiline commands and matching bash-like behavior. This document explains the logic and best practices for handling unclosed quotes in your shell's prompt logic.

---

## Key Principles

### 1. **Quotes Do Not Nest**
- In POSIX shells, quotes do **not** nest.
- If you start a single quote (`'`), only another single quote closes it.
- Double quotes (`"`) inside single quotes are treated as literal characters, and vice versa.

### 2. **Tracking Quote State**
- Use two flags: `in_squote` (inside single quote) and `in_dquote` (inside double quote).
- When scanning input:
  - If you see a single quote and are **not** inside a double quote, toggle `in_squote`.
  - If you see a double quote and are **not** inside a single quote, toggle `in_dquote`.
- Only the matching quote closes the quoted section.

### 3. **Prompting for More Input**
- If you reach the end of the input and are still inside a quote, prompt the user again with a secondary prompt (e.g., `>`).
- Read the next line, append it to your input, and continue scanning.
- Repeat until all quotes are closed.

---

## Example: Bash Behavior

```bash
echo 'hi
> '"'
> "'"
> "'
hi
'
"
"
```

- The shell keeps prompting for more input until the single quote is closed.
- Double quotes and additional single quotes inside the single-quoted string are treated as literal characters.
- When the single quote is finally closed, the shell executes the command and prints the entire quoted string.

---

## Implementation Tips

- **Scan input character by character.**
- **Toggle quote flags only when not inside the other quote type.**
- **Append each new line to the existing input when prompting for more.**
- **Do not treat quotes as nested.**
- **Return the full, combined input only when all quotes are closed.**

---

## Summary

- Shell quotes do **not** nest.
- Only the matching quote closes the quoted section.
- Your prompt logic should keep reading and appending lines until all quotes are closed.
- Double quotes inside single quotes (and vice versa) are treated as literal characters.

This approach will ensure your shell matches bash’s behavior and handles complex multiline input correctly.