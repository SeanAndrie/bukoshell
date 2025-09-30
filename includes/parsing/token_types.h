/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_types.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:15:10 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/27 03:27:39 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_TYPES_H
# define TOKEN_TYPES_H

# define METACHARS "()\'\"&|<>"
# define GROUP_TOKENS "()\'\""
# define OPERATOR_TOKENS "&|<>"
# define SPECIAL_VARIABLES "?$#"

/*
** Higher-level token category masks.
** These bitmasks represent general groups of tokens.
*/
# define TOKEN_WORD 0x01
# define TOKEN_QUOTE 0x02
# define TOKEN_CTRL_OP 0x04
# define TOKEN_REDIR_OP 0x08
# define TOKEN_METACHAR 0x10
# define TOKEN_ARITH 0x20    // marks arithmetic contexts (( ... ))
# define TOKEN_SUBSHELL 0x40 // marks subshell/group contexts ( ... )
# define TOKEN_GROUP 0x100
# define TOKEN_GROUP_OPEN 0x200
# define TOKEN_GROUP_CLOSE 0x400
# define TOKEN_WHITESPACE 0x800
# define TOKEN_PARAMETER 0x1000
# define TOKEN_NONE 0x2000

/*
** Enum defining specific token types.
** Combines one or more category masks with unique bits.
*/
typedef enum e_token_type
{
	T_NONE = TOKEN_NONE,
	T_WORD = TOKEN_WORD,
	T_WHITESPACE = TOKEN_WHITESPACE,
	T_WORD_SQUOTE = TOKEN_WORD | TOKEN_QUOTE | 0x10000,
	T_WORD_DQUOTE = TOKEN_WORD | TOKEN_QUOTE | 0x20000,
	T_PIPE = TOKEN_CTRL_OP | TOKEN_METACHAR | 0x40000,
	T_OR = TOKEN_CTRL_OP | TOKEN_METACHAR | 0x80000,
	T_AND = TOKEN_CTRL_OP | TOKEN_METACHAR | 0x100000,
	T_BACKGROUND = TOKEN_CTRL_OP | TOKEN_METACHAR | 0x200000,
	T_REDIR_IN = TOKEN_REDIR_OP | TOKEN_METACHAR | 0x400000,
	T_REDIR_OUT = TOKEN_REDIR_OP | TOKEN_METACHAR | 0x800000,
	T_REDIR_APPEND = TOKEN_REDIR_OP | TOKEN_METACHAR | 0x1000000,
	T_HEREDOC = TOKEN_REDIR_OP | TOKEN_METACHAR | 0x2000000,
	T_LPAREN = TOKEN_GROUP | TOKEN_GROUP_OPEN | TOKEN_METACHAR | 0x4000000,
	T_RPAREN = TOKEN_GROUP | TOKEN_GROUP_CLOSE | TOKEN_METACHAR | 0x8000000,
	T_PARAMETER = TOKEN_WORD | TOKEN_PARAMETER | 0x10000000,
}	t_token_type;

#endif
