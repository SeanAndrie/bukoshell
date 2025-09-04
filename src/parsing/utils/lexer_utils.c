/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:28:51 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/04 18:36:49 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

int	is_token_type(t_token_type type, int category_mask)
{
	return ((type & category_mask) == (unsigned int)category_mask);
}

int	handle_concatenation(t_token **head)
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
					return (0);
				concat = concat_tokens(popped, TOKEN_WORD);	
				free_tokens(&popped);            	
				if (!concat)                    	
					return (0);                    	
				concat->next = *curr;           	
				*curr = concat;                 	
			}
		}
		curr = &(*curr)->next;
	}
	return (1);
}

t_token_type	categorize_ctrl_op(char **line_ptr, bool is_double)
{
	if (**line_ptr == '|')
	{
		if (is_double)
			return (T_OR);
		return (T_PIPE);
	}
	else if (**line_ptr == '&')
	{
		if (is_double)
			return (T_AND);
		return (T_BACKGROUND);
	}
	return (TOKEN_NONE);
}

t_token_type	categorize_redirection(char **line_ptr, bool is_double)
{
	if (**line_ptr == '<')
	{
		if (is_double)
			return (T_HEREDOC);
		return (T_REDIR_IN);
	}
	else if (**line_ptr == '>')
	{
		if (is_double)
			return (T_REDIR_APPEND);
		return (T_REDIR_OUT);
	}
	return (TOKEN_NONE);
}
