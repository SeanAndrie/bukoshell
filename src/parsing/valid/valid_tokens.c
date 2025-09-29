/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:56:21 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/28 16:33:46 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <boolean.h>
#include <parsing/valid.h>

t_bool			parse_command_list(t_token **curr, t_bool in_group);
static t_bool	parse_command(t_token **curr, t_bool in_group);
static t_bool	parse_compound_command(t_token **curr, t_bool in_group);

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

static t_bool	parse_simple_command(t_token **curr, t_bool in_group)
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
	if (*curr && !in_group && is_token_type((*curr)->type, TOKEN_GROUP_CLOSE))
		return (log_error(ERROR_SYNTAX, "near unexpected token '%s'\n",
				(*curr)->lexeme), FALSE);
	return (TRUE);
}

static t_bool	parse_compound_command(t_token **curr, t_bool in_group)
{
	if (!*curr || !is_token_type((*curr)->type, TOKEN_GROUP_OPEN))
		return (FALSE);
	consume(curr);
	if (!*curr)
		return (log_error(ERROR_SYNTAX, "nothing after '('\n"), FALSE);
	if (is_token_type((*curr)->type, TOKEN_GROUP_CLOSE))
		return (log_error(ERROR_SYNTAX, "empty subshell '()' not allowed\n"),
			FALSE);
	if (!parse_command_list(curr, in_group))
		return (FALSE);
	if (!*curr || !is_token_type((*curr)->type, TOKEN_GROUP_CLOSE))
		return (log_error(ERROR_SYNTAX, "unmatched ')'\n"), FALSE);
	consume(curr);
	return (TRUE);
}

static t_bool	parse_command(t_token **curr, t_bool in_group)
{
	if (!*curr)
		return (FALSE);
	if (is_token_type((*curr)->type, TOKEN_GROUP_OPEN))
	{
		in_group = TRUE;
		return (parse_compound_command(curr, in_group));
	}
	else
		return (parse_simple_command(curr, in_group));
}

t_bool	parse_command_list(t_token **curr, t_bool in_group)
{
	t_token	*op;

	if (!parse_command(curr, in_group))
		return (FALSE);
	while (*curr && is_token_type((*curr)->type, TOKEN_CTRL_OP))
	{
		op = *curr;
		consume(curr);
		if (!*curr || is_token_type((*curr)->type, TOKEN_GROUP_CLOSE))
			return (log_error(ERROR_SYNTAX, "near unexpected token '%s'\n",
					op->lexeme), FALSE);
		if (!parse_command(curr, in_group))
			return (FALSE);
	}
	return (TRUE);
}
