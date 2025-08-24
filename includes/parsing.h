/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:17:31 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/24 14:50:07 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <libft.h>
# include <stdbool.h>

# define GROUP_TOKENS "{()}\'\""
# define OPERATOR_TOKENS "&|<>"

// Higher Category Masks
# define TOKEN_WORD (1 << 0)     // 0000 0000 0000 0001
# define TOKEN_QUOTE (1 << 1)    // 0000 0000 0000 0010
# define TOKEN_CTRL_OP (1 << 2)  // 0000 0000 0000 0100
# define TOKEN_REDIR_OP (1 << 3) // 0000 0000 0000 1000
# define TOKEN_METACHAR (1 << 4) // 0000 0000 0001 0000
# define TOKEN_GROUP (1 << 5)    // 0000 0000 0010 0000
# define TOKEN_EOF (1 << 6)      // 0000 0000 0100 0000
# define TOKEN_NONE (1 << 7)  // 0000 0000 1000 0000

// Specific Token Definitions
typedef enum e_token_type
{
	// Basic Words
	T_WORD = TOKEN_WORD,
	T_WORD_SQUOTE = TOKEN_WORD | TOKEN_QUOTE | (1 << 10),
	T_WORD_DQUOTE = TOKEN_WORD | TOKEN_QUOTE | (1 << 11),

	// Control Operators (Control Flow + Metacharacter)
	T_PIPE = TOKEN_CTRL_OP | TOKEN_METACHAR | (1 << 12),
	T_OR = TOKEN_CTRL_OP | TOKEN_METACHAR | (1 << 13),
	T_AND = TOKEN_CTRL_OP | TOKEN_METACHAR | (1 << 14),
	T_BACKGROUND = TOKEN_CTRL_OP | TOKEN_METACHAR | (1 << 15),
	// T_SEMICOLON = TOKEN_CTRL_OP | TOKEN_METACHAR | (1 << 16),

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

	// Special
	T_EOF = TOKEN_EOF,
}						t_token_type;

typedef struct s_token
{
	enum e_token_type	type;
	struct s_token		*next;
	char				*lexeme;
}						t_token;

// Tokenizer
t_token					*create_tokens(char *line);

t_token_type	categorize_ctrl_op(char **line_ptr, bool is_double);
t_token_type	categorize_redirection(char **line_ptr, bool is_double);

// Token Utilities
void					free_tokens(t_token **head);
t_token					*create_token(char *lexeme, t_token_type type);
void					append_token(t_token **head, char *lexeme,
							t_token_type type);
int						is_token_type(t_token_type token, int category_mask);

#endif
