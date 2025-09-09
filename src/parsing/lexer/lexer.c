/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:52:14 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/09 20:43:55 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <parsing/lexer.h>

char	*process_operator(char **line_ptr, t_token_type *type)
{
	size_t	len;
	char	*lexeme;
	bool	is_double;

	len = 1;
	is_double = (*(*line_ptr + 1) && *(*line_ptr + 1) == **line_ptr);
	if (**line_ptr == '|' || **line_ptr == '&')
		*type = categorize_ctrl_op(**line_ptr, is_double);
	else
		*type = categorize_redirection(**line_ptr, is_double);
	if (is_double)
		len++;
	lexeme = ft_calloc(len + 1, sizeof(char));
	if (!lexeme)
		return (NULL);
	lexeme[0] = **line_ptr;
	(*line_ptr)++;
	if (is_double)
	{
		lexeme[1] = **line_ptr;
		(*line_ptr)++;
	}
	return (lexeme);
}

char	*process_quotes(char **line_ptr, t_token_type *type)
{
	char	*lexeme;
	char	quote;
	char	*start;
	char	*end;

	quote = **line_ptr;
	*type = T_WORD_DQUOTE;
	if (quote == '\'')
		*type = T_WORD_SQUOTE;
	start = ++(*line_ptr);
	while (**line_ptr && **line_ptr != quote)
		(*line_ptr)++;
	if (**line_ptr != quote)
		return (NULL);
	end = *line_ptr;
	lexeme = ft_calloc((end - start) + 1, sizeof(char));
	if (!lexeme)
		return (NULL);
	ft_strlcpy(lexeme, start, (end - start) + 1);
	(*line_ptr)++;
	return (lexeme);
}

char	*process_grouping(char **line_ptr, t_token_type *type)
{
	char	*lexeme;

	if (**line_ptr == '\'' || **line_ptr == '\"')
		return (process_quotes(line_ptr, type));
	if (**line_ptr == '(')
		*type = T_LPAREN;
	else if (**line_ptr == ')')
		*type = T_RPAREN;
	lexeme = ft_calloc(2, sizeof(char));
	if (!lexeme)
		return (NULL);
	lexeme[0] = **line_ptr;
	(*line_ptr)++;
	return (lexeme);
}

char	*process_parameter(char **line_ptr, t_token_type *type)
{
	char	*lexeme;
	char	*start;
	char	*end;

	*type = T_PARAMETER;
	start = *line_ptr;
	(*line_ptr)++;
	if (ft_strchr(SPECIAL_VARIABLES, **line_ptr) || ft_isdigit(**line_ptr))
		(*line_ptr)++;
	else
	{
		while (**line_ptr && (ft_isalnum(**line_ptr) || **line_ptr == '_'))
			(*line_ptr)++;
	}
	end = *line_ptr;
	lexeme = ft_calloc((end - start) + 1, sizeof(char));
	if (!lexeme)
		return (NULL);
	ft_strlcpy(lexeme, start, (end - start) + 1);
	return (lexeme);
}

char	*process_word(char **line_ptr, t_token_type *type)
{
	char	*lexeme;
	char	*start;
	char	*end;

	start = *line_ptr;
	if (ft_isspace(**line_ptr))
	{
		*type = T_WHITESPACE;
		while (**line_ptr && ft_isspace(**line_ptr))
			(*line_ptr)++;
		return (ft_strdup(""));
	}
	else
	{
		*type = T_WORD;
		while (**line_ptr && !ft_isspace(**line_ptr) && !ft_strchr(METACHARS,
				**line_ptr))
			(*line_ptr)++;
	}
	end = *line_ptr;
	lexeme = ft_calloc((end - start) + 1, sizeof(char));
	if (!lexeme)
		return (NULL);
	ft_strlcpy(lexeme, start, (end - start) + 1);
	return (lexeme);
}
