/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:56:21 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/27 01:38:36 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing/valid.h>
#include <boolean.h>

t_bool		parse_command_list(t_token **curr);
static t_bool	parse_command(t_token **curr);
static t_bool	parse_compound_command(t_token **curr);

t_bool	is_valid_metachar(t_token *token)
{
	if (!is_token_type(token->type, TOKEN_METACHAR))
		return (TRUE);
	if (!token->next)
		return (log_error(ERROR_SYNTAX, "near unexpected token 'newline'\n"),
			FALSE);
	if (is_token_type(token->type, T_PIPE))
	{
		if (!is_token_type(token->next->type, TOKEN_WORD)
			&& !is_token_type(token->next->type, TOKEN_GROUP_OPEN))
			return (log_error(ERROR_SYNTAX, "near unexpected token '%s'\n",
					token->next->lexeme), FALSE);
	}
	return (TRUE);
}

static t_bool	parse_simple_command(t_token **curr)
{
	while (*curr && is_token_type((*curr)->type, T_WORD))
		consume(curr);
	while (*curr && is_token_type((*curr)->type, TOKEN_REDIR_OP))
	{
		if (!is_valid_metachar(*curr))
			return (FALSE);
		consume(curr);
		if (!*curr || !is_token_type((*curr)->type, T_WORD))
			return (log_error(ERROR_SYNTAX, "redirection: missing target\n"),
				FALSE);
		consume(curr);
	}
	return (TRUE);
}

static t_bool	parse_compound_command(t_token **curr)
{
	if (!*curr || !is_token_type((*curr)->type, TOKEN_GROUP_OPEN))
		return (FALSE);
	consume(curr);
	if (!*curr)
		return (log_error(ERROR_SYNTAX, "nothing after '('\n"), FALSE);
	if (is_token_type((*curr)->type, TOKEN_GROUP_CLOSE))
		return (log_error(ERROR_SYNTAX, "empty subshell '()' not allowed\n"),
			FALSE);
	if (!parse_command_list(curr))
		return (FALSE);
	if (!*curr || !is_token_type((*curr)->type, TOKEN_GROUP_CLOSE))
		return (log_error(ERROR_SYNTAX, "unmatched ')'\n"), FALSE);
	consume(curr);
	return (TRUE);
}

static t_bool	parse_command(t_token **curr)
{
	if (!*curr)
		return (FALSE);
	if (is_token_type((*curr)->type, TOKEN_GROUP_OPEN))
		return (parse_compound_command(curr));
	else
		return (parse_simple_command(curr));
}

t_bool	parse_command_list(t_token **curr)
{
	t_token	*op;

	if (!parse_command(curr))
		return (FALSE);
	while (*curr && is_token_type((*curr)->type, TOKEN_CTRL_OP))
	{
		op = *curr;
		consume(curr);
		if (!*curr || is_token_type((*curr)->type, TOKEN_GROUP_CLOSE))
			return (log_error(ERROR_SYNTAX, "near unexpected token '%s'\n",
					op->lexeme), FALSE);
		if (!parse_command(curr))
			return (FALSE);
	}
	return (TRUE);
}
