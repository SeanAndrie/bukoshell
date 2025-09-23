/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:28:51 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/23 14:34:52 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>
#include <stdbool.h>
#include <stdio.h>

bool	handle_concatenation(t_token **head)
{
	t_token	**curr;
	t_token	*popped;
	t_token	*concat;

	curr = head;
	while (*curr)
	{
		if (is_token_type((*curr)->type, TOKEN_WORD))
		{
			if ((*curr)->next && is_token_type((*curr)->next->type, TOKEN_WORD))
			{
				popped = pop_token_type(curr, TOKEN_WORD);
				if (!popped)
					return (false);
				concat = concat_tokens(popped, TOKEN_WORD);
				free_tokens(&popped);
				if (!concat)
					return (false);
				concat->next = *curr;
				*curr = concat;
			}
		}
		curr = &(*curr)->next;
	}
	return (true);
}

t_token_type	categorize_ctrl_op(char c, bool is_double)
{
	if (c == '|')
	{
		if (is_double)
			return (T_OR);
		return (T_PIPE);
	}
	else if (c == '&')
	{
		if (is_double)
			return (T_AND);
		return (T_BACKGROUND);
	}
	return (TOKEN_NONE);
}

t_token_type	categorize_redirection(char c, bool is_double)
{
	if (c == '<')
	{
		if (is_double)
			return (T_HEREDOC);
		return (T_REDIR_IN);
	}
	else if (c == '>')
	{
		if (is_double)
			return (T_REDIR_APPEND);
		return (T_REDIR_OUT);
	}
	return (TOKEN_NONE);
}
