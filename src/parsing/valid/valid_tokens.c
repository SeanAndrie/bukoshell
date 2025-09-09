/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 20:56:21 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/10 02:21:19 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing/valid.h>

static bool	is_valid_subshell(t_token *head)
{
	t_token	*curr;
	t_token	*scan;
	int		depth;

	depth = 1;
	curr = head->next;
	scan = curr;
	while (scan && depth > 0)
	{
		track_depth(scan, &depth);
		scan = scan->next;
	}
	if (depth < 0)
		return (false);
	return (true);
}

bool	is_valid_grouping(t_token *prev, t_token *curr, int *depth)
{
	if (is_token_type(curr->type, TOKEN_GROUP_OPEN))
	{
		(*depth)++;
		if (curr->next && is_token_type(curr->next->type, TOKEN_GROUP_CLOSE))
			return (print_error(ERROR_SYNTAX,
					"empty subshell '()' not allowed\n"), false);
		if (!curr->next)
			return (print_error(ERROR_SYNTAX, "nothing after '('\n"), false);
		if (!is_valid_subshell(curr))
			return (print_error(ERROR_SYNTAX, "unmatched ')'\n"), false);
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

bool	is_valid_operator(t_token *curr)
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

bool	is_valid_redirect(t_token *curr)
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

bool	is_valid_parameter(t_token *curr)
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
