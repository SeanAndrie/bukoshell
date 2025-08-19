/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 21:17:31 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/20 03:26:06 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <bukoshell.h>

// Basic literal word
# define TOKEN_WORD 0x0001

// Any quoted word
# define TOKEN_QUOTE 0x0002

// Control flow: | & && || ;
# define TOKEN_CTRL_OP 0x0004

// Redirections: < > << >>
# define TOKEN_REDIR_OP 0x0008

// Any special character (CTRL_OP | REDIR_OP | QUOTE, etc.)
# define TOKEN_METACHAR 0x0010

// End of input
# define TOKEN_EOF 0x0020

typedef enum e_token_type
{
	T_WORD = TOKEN_WORD,
	T_WORD_SQUOTE = TOKEN_WORD | TOKEN_QUOTE | 0x0100,
	T_WORD_DQUOTE = TOKEN_WORD | TOKEN_QUOTE | 0X0200,
	T_PIPE = TOKEN_CTRL_OP | TOKEN_METACHAR | 0X0400,
	T_AND = TOKEN_CTRL_OP | TOKEN_METACHAR | 0x0800,
	T_OR = TOKEN_CTRL_OP | TOKEN_METACHAR | 0x1000,
	T_SEMICOLON = TOKEN_CTRL_OP | TOKEN_METACHAR | 0x2000,
	T_BACKGROUND = TOKEN_CTRL_OP | TOKEN_METACHAR | 0x4000,
	T_REDIR_IN = TOKEN_REDIR_OP | TOKEN_METACHAR | 0x8000,
	T_REDIR_OUT = TOKEN_REDIR_OP | TOKEN_METACHAR | 0x10000,
	T_REDIR_APPEND = TOKEN_REDIR_OP | TOKEN_METACHAR | 0x20000,
	T_HEREDOC = TOKEN_REDIR_OP | TOKEN_METACHAR | 0x40000,
	T_EOF = TOKEN_EOF,
}						t_token_type;

typedef struct s_token
{
	enum e_token_type	type;
	struct s_token		*next;
	char				*lexeme;
}						t_token;

// Lexical Analysis
t_token	*create_tokens(char *line);
void	print_tokens(t_token *head);

// Token Utilities
void	append_token(t_token **head, char *lexeme, t_token_type type);
t_token	*create_token(char *lexeme, t_token_type type);
void	free_tokens(t_token **head);

#endif
