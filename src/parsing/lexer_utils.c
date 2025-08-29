/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:28:51 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/29 18:07:36 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

int	is_token_type(t_token_type type, int category_mask)
{
	return ((type & category_mask) == (unsigned int)category_mask);
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
