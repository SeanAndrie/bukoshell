/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:17:31 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/08 01:42:09 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <libft.h>
# include <stdio.h>
# include <stdbool.h>
# include <readline/history.h>
# include <readline/readline.h>

# define DEBUG_MODE true

# define PS1 "bukoshell $> "
# define PS2 "> "

# define GROUP_TOKENS "()\'\""
# define OPERATOR_TOKENS "&|<>"
# define SPECIAL_VARIABLES "?$#"

/*
** Higher-level token category masks.
** These bitmasks represent general groups of tokens.
*/
# define TOKEN_WORD         0x01    // (1 << 0)
# define TOKEN_QUOTE        0x02    // (1 << 1)
# define TOKEN_CTRL_OP      0x04    // (1 << 2)
# define TOKEN_REDIR_OP     0x08    // (1 << 3)
# define TOKEN_METACHAR     0x10    // (1 << 4)
# define TOKEN_GROUP        0x20    // (1 << 5)
# define TOKEN_GROUP_OPEN   0x40    // (1 << 6)
# define TOKEN_GROUP_CLOSE  0x80    // (1 << 7)
# define TOKEN_WHITESPACE   0x100   // (1 << 8)
# define TOKEN_PARAMETER    0x200   // (1 << 9) 
# define TOKEN_NONE         0x400   // (1 << 10)

/*
** Enum defining specific token types.
** Combines one or more category masks with unique bits.
*/
typedef enum e_token_type
{
    T_NONE = TOKEN_NONE,
    T_WORD = TOKEN_WORD,
    T_WHITESPACE = TOKEN_WHITESPACE,
    T_WORD_SQUOTE = TOKEN_WORD | TOKEN_QUOTE | 0x800,
    T_WORD_DQUOTE = TOKEN_WORD | TOKEN_QUOTE | 0x1000,
    T_PIPE = TOKEN_CTRL_OP | TOKEN_METACHAR | 0x2000,
    T_OR = TOKEN_CTRL_OP | TOKEN_METACHAR | 0x4000,
    T_AND = TOKEN_CTRL_OP | TOKEN_METACHAR | 0x8000,
    T_BACKGROUND = TOKEN_CTRL_OP | TOKEN_METACHAR | 0x10000,
    T_REDIR_IN = TOKEN_REDIR_OP | TOKEN_METACHAR | 0x20000,
    T_REDIR_OUT = TOKEN_REDIR_OP | TOKEN_METACHAR | 0x40000,
    T_REDIR_APPEND = TOKEN_REDIR_OP | TOKEN_METACHAR | 0x80000,
    T_HEREDOC = TOKEN_REDIR_OP | TOKEN_METACHAR | 0x100000,
    T_LPAREN = TOKEN_GROUP | TOKEN_GROUP_OPEN | TOKEN_METACHAR | 0x200000,
    T_RPAREN = TOKEN_GROUP | TOKEN_GROUP_CLOSE | TOKEN_METACHAR | 0x400000,
    T_PARAMETER = TOKEN_PARAMETER | 0x800000
} 						t_token_type;

typedef enum e_error_type
{
	ERROR_SYNTAX,
	ERROR_RUNTIME
}						t_error_type;

typedef enum e_node_type
{
	N_COMMAND,
	N_OPERATOR,
	N_SUBSHELL,
}						t_node_type;

typedef struct s_quote_state
{
	bool				in_single;
	bool				in_double;	
}						t_quote_state;

typedef struct s_token
{
	enum e_token_type	type;
	struct s_token		*next;
	char				*lexeme;
}						t_token;

typedef struct s_redirect
{
	int					fd;
	char				*fname;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_node
{
	enum e_node_type	type;
	struct s_node		*left;
	struct s_node		*right;
	char				**argv;
	unsigned int		operator;
	t_redirect			*redirect;
}						t_node;

/*
** Tokenizes the input string into a linked list of tokens.
**
** @param line   The input string to tokenize.
** @return      Pointer to the head of the token list on success,
**              or NULL if an error occurs.
*/
t_token					*create_tokens(char *line);

/*
** Merges consecutive WORD tokens into a single token.
**
** @param head  Pointer to the head of the token list.
** @return      1 on success, 0 if concatenation fails.
*/
int						handle_concatenation(t_token **head);

/*
** Checks if a token's type matches the given category mask.
**
** @param type          The token type to check.
** @param category_mask Bitmask specifying token type(s) to match
** @return             true if the token type matches, false otherwise.
*/
bool					is_token_type(t_token_type type, unsigned int category_mask);

/*
** Concatenates the lexemes of a linked list of tokens into a single string
** and creates a new token of the specified type.
**
** @param head         Pointer to the head of the token list.
** @param concat_type  The token type assigned to the newly created token.
** @return             A new token containing the concatenated string,
**                     or NULL if memory allocation or token creation fails.
*/
t_token					*concat_tokens(t_token *head, t_token_type concat_type);

/*
** Pops the first consecutive sequence of tokens matching the given type
** from the start of the list. Also works as a regular pop when the head
** matches the requested type.
**
** @param head             Pointer to the head of the token list.
**                         Updated to point to the next remaining token.
** @param type_to_extract  The token type(s) to extract (bitmask).
** @return                 Pointer to the extracted sublist of tokens,
**                         or NULL if no matching tokens are found.
*/
t_token					*pop_token_type(t_token **head, t_token_type type);

/*
** Removes and frees all tokens from the list that match the given type.
** Updates the head pointer to maintain list integrity.
**
** @param head          Pointer to the head of the token list.
** @param type_to_strip The token type(s) to remove (bitmask).
** @return              None.
*/
void					remove_tokens(t_token **head,
							t_token_type type_to_remove);

/*
** Builds a syntax tree from a linked list of tokens based on operator precedence.
** Recursively splits the token list around the lowest-precedence operator to
** construct the tree.
**
** @param head  Pointer to the head of the token list.
** @return      Pointer to the root node of the constructed syntax tree,
**              or NULL if memory allocation fails.
*/
t_node					*create_syntax_tree(t_token *head);

/*
** Creates a linked list of redirection nodes from a list of tokens.
** Scans for redirection operators followed by WORD tokens and stores
** them as redirection nodes.
**
** @param head  Pointer to the head of the token list.
** @return      Pointer to the head of the created redirection list,
**              or NULL if memory allocation fails.
*/
t_redirect				*create_redirections(t_token *head);

/*
** Converts a list of tokens into an argv-style array of strings.
** Counts tokens until a redirection or control operator is reached,
** then delegates allocation and copying to alloc_argv().
**
** @param head   Pointer to the head of the token list.
** @return       A NULL-terminated array of strings (argv),
**               or NULL if memory allocation fails.
*/
char					**tokens_to_argv(t_token *head);

unsigned int			create_prompt_mask(t_token *head);

t_token					*find_lowest_precedence(t_token *head);

bool					append_token(t_token **head, char *lexeme,
							t_token_type type);

char 					*process_word(char **line_ptr, t_token_type *type);
char					*process_quotes(char **line_ptr, t_token_type *type);
char					*process_grouping(char **line_ptr, t_token_type *type);
char					*process_operator(char **line_ptr, t_token_type *type);
char					*process_parameter(char **line_ptr, t_token_type *type);

t_token_type			categorize_ctrl_op(char c, bool is_double);
t_token_type			categorize_redirection(char c, bool is_double);

void					free_tokens(t_token **head);
void					free_syntax_tree(t_node **root);
void					free_redirects(t_redirect **head);
void					free_str_arr(char **str_arr, int n);

void					print_syntax_tree(t_node *node, int level);
void					print_tokens(t_token *head, bool show_whitespace);
void    				print_error(t_error_type type, const char *format, ...);

bool					are_valid_tokens(t_token *head);

#endif
