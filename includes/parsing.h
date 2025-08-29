/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:17:31 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/29 18:08:02 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <libft.h>
# include <stdio.h>
# include <stdbool.h>
# include <readline/history.h>
# include <readline/readline.h>

# define PS1 "bukoshell> "
# define PS2 ">"

# define GROUP_TOKENS "{()}\'\""
# define OPERATOR_TOKENS "&|<>"

// Higher Category Masks
# define TOKEN_WORD (1 << 0)
# define TOKEN_QUOTE (1 << 1)   
# define TOKEN_CTRL_OP (1 << 2) 
# define TOKEN_REDIR_OP (1 << 3)
# define TOKEN_METACHAR (1 << 4)
# define TOKEN_GROUP (1 << 5)
# define TOKEN_WHITESPACE (1 << 6)
# define TOKEN_NONE (1 << 7)

// Specific Token Definitions
typedef enum e_token_type
{
	// Whitespace
	T_WHITEPSACE = TOKEN_WHITESPACE,
	
	// Basic Words
	T_WORD = TOKEN_WORD,
	T_WORD_SQUOTE = TOKEN_WORD | TOKEN_QUOTE | (1 << 10),
	T_WORD_DQUOTE = TOKEN_WORD | TOKEN_QUOTE | (1 << 11),

	// Control Operators (Control Flow + Metacharacter)
	T_PIPE = TOKEN_CTRL_OP | TOKEN_METACHAR | (1 << 12),
	T_OR = TOKEN_CTRL_OP | TOKEN_METACHAR | (1 << 13),
	T_AND = TOKEN_CTRL_OP | TOKEN_METACHAR | (1 << 14),
	T_BACKGROUND = TOKEN_CTRL_OP | TOKEN_METACHAR | (1 << 15),

	// Redirection Operators (Redirection + Metacharacter)
	T_REDIR_IN = TOKEN_REDIR_OP | TOKEN_METACHAR | (1 << 16),
	T_REDIR_OUT = TOKEN_REDIR_OP | TOKEN_METACHAR | (1 << 17),
	T_REDIR_APPEND = TOKEN_REDIR_OP | TOKEN_METACHAR | (1 << 18),
	T_HEREDOC = TOKEN_REDIR_OP | TOKEN_METACHAR | (1 << 19),

	// Grouping Operators (Group + Metacharacter)
	T_LPAREN = TOKEN_GROUP | TOKEN_METACHAR | (1 << 20),
	T_RPAREN = TOKEN_GROUP | TOKEN_METACHAR | (1 << 21),
	T_LBRACE = TOKEN_GROUP | TOKEN_METACHAR | (1 << 22),
	T_RBRACE = TOKEN_GROUP | TOKEN_METACHAR | (1 << 23),
}						t_token_type;

typedef struct s_token
{
	enum e_token_type	type;
	struct s_token		*next;
	char				*lexeme;
}						t_token;

// Lexer
t_token					*create_tokens(char *line);

// Lexer Utils
t_token_type			categorize_ctrl_op(char **line_ptr, bool is_double);
t_token_type			categorize_redirection(char **line_ptr, bool is_double);
int						is_token_type(t_token_type type, int category_mask);

// Token Utilities
void					free_tokens(t_token **head);
t_token					*concat_tokens(t_token **head);
t_token					*create_token(char *lexeme, t_token_type type);
t_token					*pop_tokens(t_token **head, t_token_type type);
void					append_token(t_token **head, char *lexeme,
							t_token_type type);

#endif
