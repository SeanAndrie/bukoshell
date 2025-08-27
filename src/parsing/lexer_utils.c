/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:28:51 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/27 16:36:29 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

t_token_type	categorize_grouping(char quote, char **line_ptr)
{
	if (**line_ptr && ft_isascii(**line_ptr) && !ft_isspace(**line_ptr))
	{
		while (**line_ptr && ft_isascii(**line_ptr) && !ft_isspace(**line_ptr))
			(*line_ptr)++;
		if (!*line_ptr)
			return (TOKEN_NONE);
		return (T_WORD);
	}
	if (quote == '\'')
		return (T_WORD_SQUOTE);
	else if (quote == '\"')
		return (T_WORD_DQUOTE);
	return (TOKEN_NONE);
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
