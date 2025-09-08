/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:56:21 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/08 01:18:57 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static bool	is_valid_grouping(t_token *prev, t_token *curr, int *depth)
{
	if (is_token_type(curr->type, TOKEN_GROUP_OPEN))
	{
		(*depth)++;
        if (curr->next && is_token_type(curr->next->type, TOKEN_GROUP_CLOSE))
            return (print_error(ERROR_SYNTAX, "empty subshell '()' not allowed\n"), false);
		if (!curr->next)
			return (print_error(ERROR_SYNTAX, "nothing after '('\n"), false);
	}
	else if (is_token_type(curr->type, TOKEN_GROUP_CLOSE))
	{
		(*depth)--;
		if (*depth < 0)
			return (print_error(ERROR_SYNTAX, "unmatched ')'\n"), false);
		if (prev && is_token_type(prev->type, TOKEN_CTRL_OP))
			return (print_error(ERROR_SYNTAX, "operator before ')'\n"), false);
	}
	return (true);
}

static bool	is_valid_operator(t_token *curr)
{
	if (is_token_type(curr->type, TOKEN_CTRL_OP))
	{
		if (!curr->next || is_token_type(curr->next->type, TOKEN_CTRL_OP))
		{
			print_error(ERROR_SYNTAX, "unexpected operator '%s'\n",
				curr->lexeme);
			return (false);
		}
	}
	return (true);
}

static bool	is_valid_redirect(t_token *curr)
{
	if (is_token_type(curr->type, TOKEN_REDIR_OP))
	{
		if (!curr->next || !is_token_type(curr->next->type, TOKEN_WORD))
		{
			print_error(ERROR_SYNTAX, "redirection '%s' missing target\n",
				curr->lexeme);
			return (false);
		}
	}
	return (true);
}

static bool	is_valid_parameter(t_token *curr)
{
	if (is_token_type(curr->type, TOKEN_PARAMETER))
	{
		if (!curr->lexeme || !curr->lexeme[1])
		{
			print_error(ERROR_SYNTAX, "invalid parameter '%s'\n", curr->lexeme);
			return (false);
		}
	}
	return (true);
}

bool	are_valid_tokens(t_token *head)
{
	t_token	*prev;
	t_token	*curr;
	int		depth;

	prev = NULL;
	curr = head;
	depth = 0;
	while (curr)
	{
		if (!is_valid_grouping(prev, curr, &depth) || !is_valid_operator(curr)
			|| !is_valid_redirect(curr) || !is_valid_parameter(curr))
			return (false);
		curr = curr->next;
	}
	if (depth != 0)
    {
        print_error(ERROR_SYNTAX, "unmatched '('\n");
		return (false);
    }
	return (true);
}
