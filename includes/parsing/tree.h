/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:19:50 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/10 02:00:47 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TREE_H
# define TREE_H

# include <tokens.h>

/*
** Enum representing the different types of nodes in the syntax tree.
**
** @enum N_COMMAND   - A command node (e.g., "echo hello").
** @enum N_OPERATOR  - An operator node (e.g., "&&", "||", "|").
** @enum N_SUBSHELL  - A subshell node, representing grouped commands (e.g., "(cmd1 && cmd2)").
*/
typedef enum e_node_type
{
	N_COMMAND,
	N_OPERATOR,
	N_SUBSHELL,
}						t_node_type;

/*
** Represents a single I/O redirection in a command.
**
** Each node corresponds to a redirection operator (>, >>, <, <<) and
** the associated file or descriptor.
**
** @field fd     - File descriptor associated with the redirection (e.g., 0 for stdin, 1 for stdout).
** @field fname  - Target file name for the redirection.
** @field next   - Pointer to the next redirection in the linked list.
*/
typedef struct s_redirect
{
	int					fd;
	char				*fname;
	struct s_redirect	*next;
}						t_redirect;

/*
** Represents a node in the abstract syntax tree (AST) used by the parser.
**
** Each node corresponds to either a command, an operator, or a subshell.
**
** @field type      - The type of node (command, operator, or subshell).
** @field left      - Pointer to the left child node (used for binary operators).
** @field right     - Pointer to the right child node.
** @field argv      - Null-terminated array of strings representing command arguments.
** @field operand   - The token/operator associated with this node (e.g., &&, ||, |).
** @field redirect  - Linked list of I/O redirections for this command.
*/
typedef struct s_node
{
	enum e_node_type	type;
	struct s_node		*left;
	struct s_node		*right;
	char				**argv;
	enum e_token_type	operand;
	t_redirect			*redirect;
}						t_node;

/*
** Recursively constructs a syntax tree from a linked list of tokens.
**
** The function identifies the lowest-precedence operator within the given
** token range and creates a corresponding operator node. It then recursively
** builds the left and right subtrees using the tokens before and after the
** operator. If no operators are found, the function determines whether to
** create a subshell node or a simple command node.
**
** @param start  Pointer to the first token in the current range.
** @param end    Pointer to the token marking the end boundary (exclusive).
** @return       The root node of the constructed syntax tree,
**               or NULL if the range is empty or memory allocation fails.
*/
t_node					*create_syntax_tree(t_token *start, t_token *end);

/*
** Creates a linked list of redirection nodes from a list of tokens.
**
** The function scans the token list and identifies valid redirection patterns:
** a redirection operator (e.g., <, >, >>, <<) followed by a WORD token
** representing the file or delimiter. For each valid pair, it creates a
** t_redirect node and appends it to the list of redirections.
**
** If memory allocation or appending fails, the function frees all allocated
** redirection nodes and returns NULL.
**
** @param head   Pointer to the head of the token list.
** @return      A linked list of t_redirect nodes representing redirections,
**              or NULL on allocation failure.
*/
t_redirect				*create_redirections(t_token *head);

/*
** Converts a list of tokens into a NULL-terminated argv array.
**
** The function counts the number of consecutive non-redirection tokens
** starting from the given head and allocates an array of strings to store
** their lexemes. It stops processing when it encounters a redirection operator,
** as these are handled separately and should not be part of argv.
**
** @param head   Pointer to the head of the token list.
** @return      A NULL-terminated array of strings (argv) representing the
**              command and its arguments, or NULL if allocation fails.
*/
char					**tokens_to_argv(t_token *head);

/*
** Advances the token pointer past a balanced set of parentheses.
**
** Starting at an opening parenthesis, this function traverses the token list
** until the corresponding closing parenthesis is reached. Nested parentheses
** are handled correctly using the depth counter.
**
** The resulting token pointer will point to the token immediately after
** the closing parenthesis, or NULL if the list ends prematurely.
**
** @param head   Address of the current token pointer.
** @return      None.
*/
void					skip_parentheses(t_token **head);

/*
** Updates the current parentheses depth based on the given token.
**
** If the token is a left parenthesis '(', the depth counter is incremented.
** If the token is a right parenthesis ')', the depth counter is decremented.
** Tokens that are not parentheses do not affect the depth.
**
** @param head    Pointer to the current token.
** @param depth   Pointer to an integer tracking the current depth.
** @return       None.
*/
void					track_depth(t_token *head, int *depth);

/*
** Finds the lowest-precedence operator in a given token range.
**
** The function scans the tokens from 'start' up to (but not including) 'end'
** while tracking the current grouping depth (parentheses, braces, etc.).
** Only operators at depth 0 (outside nested groups) are considered.
**
** Operator precedence:
**   1. Logical OR (||) and Logical AND (&&) — lowest precedence.
**   2. Pipe (|) — next lowest precedence.
**
** If an OR/AND operator is found at depth 0, it is returned immediately.
** Otherwise, the first pipe operator found at depth 0 is returned.
** If no matching operators exist, the function returns NULL.
**
** @param start   Pointer to the first token in the range.
** @param end     Pointer to the token marking the end boundary (exclusive).
** @return       A pointer to the lowest-precedence operator token,
**               or NULL if none is found.
*/
t_token					*find_lowest_precedence(t_token *start, t_token *end);

#endif
