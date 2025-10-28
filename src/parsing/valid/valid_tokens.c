/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:56:21 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/28 12:17:17 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <boolean.h>
#include <debug.h>
#include <libft.h>
#include <parsing/tokens.h>
#include <parsing/tree.h>
#include <parsing/valid.h>

static t_bool	parse_command(t_token **curr, int *depth);
t_bool			parse_command_list(t_token **curr, int *depth);
static t_bool	parse_compound_command(t_token **curr, int *depth);

static t_bool	parse_simple_command(t_token **curr)
{
	while (*curr)
	{
		if (is_token_type((*curr)->type, T_WORD))
			consume(curr);
		else if (is_token_type((*curr)->type, TOKEN_REDIR_OP))
		{
			if (!is_valid_metachar(*curr))
				return (FALSE);
			consume(curr);
			if (!*curr || !is_token_type((*curr)->type, T_WORD))
			{
				log_error(ERROR_SYNTAX, ERR_BASE,
					"redirection: missing target\n");
				return (FALSE);
			}
			consume(curr);
		}
		else
			break ;
	}
	return (TRUE);
}

static t_bool	parse_compound_command(t_token **curr, int *depth)
{
	if (!*curr || !is_token_type((*curr)->type, TOKEN_GROUP_OPEN))
		return (FALSE);
	consume(curr);
	if (!*curr)
	{
		log_error(ERROR_SYNTAX, ERR_BASE, "nothing after '('\n");
		return (FALSE);
	}
	if (is_token_type((*curr)->type, TOKEN_GROUP_CLOSE))
	{
		log_error(ERROR_SYNTAX, ERR_BASE, "empty subshell '()' not allowed\n");
		return (FALSE);
	}
	if (!parse_command_list(curr, depth))
		return (FALSE);
	if (!*curr || !is_token_type((*curr)->type, TOKEN_GROUP_CLOSE))
	{
		log_error(ERROR_SYNTAX, ERR_BASE, "unmatched ')'\n");
		return (FALSE);
	}
	consume(curr);
	(*depth)--;
	return (TRUE);
}

static t_bool	parse_command(t_token **curr, int *depth)
{
	if (!*curr)
		return (FALSE);
	if (is_token_type((*curr)->type, TOKEN_GROUP_OPEN))
	{
		(*depth)++;
		return (parse_compound_command(curr, depth));
	}
	else
		return (parse_simple_command(curr));
}

t_bool	parse_command_list(t_token **curr, int *depth)
{
	t_token	*op;

	if (!parse_command(curr, depth))
		return (FALSE);
	if (!is_separated_command(*curr))
		return (FALSE);
	while (*curr && is_token_type((*curr)->type, TOKEN_CTRL_OP))
	{
		op = *curr;
		consume(curr);
		if (!*curr || is_token_type((*curr)->type, TOKEN_GROUP_CLOSE))
		{
			log_error(ERROR_SYNTAX, ERR_BASE, "near unexpected token '%s'\n",
				op->lexeme);
			return (FALSE);
		}
		if (!parse_command(curr, depth))
			return (FALSE);
	}
	return (TRUE);
}
