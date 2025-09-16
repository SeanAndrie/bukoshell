/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:56:21 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/16 20:13:34 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing/valid.h>

bool				parse_command_list(t_token **curr);
static bool			parse_command(t_token **curr);
static bool			parse_compound_command(t_token **curr);

static inline void	consume(t_token **curr)
{
	if (*curr)
		*curr = (*curr)->next;
}

static bool	parse_simple_command(t_token **curr)
{
	if (!*curr || !is_token_type((*curr)->type, T_WORD))
		return (false);
	while (*curr && is_token_type((*curr)->type, T_WORD))
		consume(curr);
	while (*curr && is_token_type((*curr)->type, TOKEN_REDIR_OP))
	{
		if (!is_valid_metachar(*curr))
			return (false);
		consume(curr);
		if (!*curr || !is_token_type((*curr)->type, T_WORD))
			return (print_error(ERROR_SYNTAX, "redirection: missing target\n"),
				false);
		consume(curr);
	}
	return (true);
}

static bool	parse_compound_command(t_token **curr)
{
	if (!*curr || !is_token_type((*curr)->type, TOKEN_GROUP_OPEN))
		return (false);
	consume(curr);
	if (!*curr)
		return (print_error(ERROR_SYNTAX, "nothing after '('\n"), false);
	if (is_token_type((*curr)->type, TOKEN_GROUP_CLOSE))
		return (print_error(ERROR_SYNTAX, "empty subshell '()' not allowed\n"),
			false);
	if (!parse_command_list(curr))
		return (false);
	if (!*curr || is_token_type((*curr)->type, TOKEN_GROUP_CLOSE))
		return (print_error(ERROR_SYNTAX, "unmatched ')'\n"), false);
	consume(curr);
	return (true);
}

static bool	parse_command(t_token **curr)
{
	if (!*curr)
		return (false);
	if (is_token_type((*curr)->type, TOKEN_GROUP_OPEN))
		return (parse_compound_command(curr));
	else
		return (parse_simple_command(curr));
	return (true);
}

bool	parse_command_list(t_token **curr)
{
	t_token	*op;

	if (!parse_command(curr))
		return (false);
	while (*curr && is_token_type((*curr)->type, TOKEN_CTRL_OP))
	{
		op = *curr;
		consume(curr);
		if (!parse_command(curr))
			return (print_error(ERROR_SYNTAX, "near unexpected token '%s'\n",
					op->lexeme), false);
	}
	return (true);
}
