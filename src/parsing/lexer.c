/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:52:14 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/29 16:11:24 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static char	*process_operator(char **line_ptr, t_token_type *type)
{
	size_t	len;
	char	*lexeme;
	bool	is_double;

	len = 1;
	is_double = (*(*line_ptr + 1) && *(*line_ptr + 1) == **line_ptr);
	if (**line_ptr == '|' || **line_ptr == '&')
		*type = categorize_ctrl_op(line_ptr, is_double);
	else
		*type = categorize_redirection(line_ptr, is_double);
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

static char	*process_quotes(char **line_ptr, t_token_type *type)
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
	while (**line_ptr && *(*line_ptr + 1) != quote)
		(*line_ptr)++;
	if (*(*line_ptr + 1) != quote)
		return (NULL);
	end = ++(*line_ptr);
	(*line_ptr)++;
	lexeme = ft_calloc((end - start) + 1, sizeof(char));
	if (!lexeme)
		return (NULL);	
	ft_strlcpy(lexeme, start, (end - start) + 1);
	return (lexeme);	
}

static char	*process_grouping(char **line_ptr, t_token_type *type)
{
	char	*lexeme;

	if (**line_ptr == '\'' || **line_ptr == '\"')
		return (process_quotes(line_ptr, type));
	if (**line_ptr == '(')
		*type = T_LPAREN;
	else if (**line_ptr == ')')
		*type = T_RPAREN;
	else if (**line_ptr == '{')
		*type = T_LBRACE;
	else if (**line_ptr == '}')
		*type = T_RBRACE;
	lexeme = ft_calloc(2, sizeof(char));
	if (!lexeme)
		return (NULL);
	lexeme[0] = **line_ptr;
	(*line_ptr)++;
	return (lexeme);
}

static char	*process_word(char **line_ptr, t_token_type *type)
{
	char	*lexeme;
	char	*start;
	char	*end;

	*type = T_WORD;
	start = *line_ptr;
	while (**line_ptr && !ft_isspace(**line_ptr) && !ft_strchr("\'\"", **line_ptr))
		(*line_ptr)++;
	end = *line_ptr; 
	lexeme = ft_calloc((end - start) + 1, sizeof(char));
	if (!lexeme)
		return (NULL);
	ft_strlcpy(lexeme, start, (end - start) + 1);
	return (lexeme);
}

t_token	*create_tokens(char *line)
{
	t_token_type	type;
	t_token			*head;
	char			*lexeme;

	head = NULL;
	while (*line)
	{
		if (ft_isspace(*line))
		{
			line++;
			continue ;
		}
		else if (ft_strchr(OPERATOR_TOKENS, *line))
			lexeme = process_operator(&line, &type);
		else if (ft_strchr(GROUP_TOKENS, *line))
			lexeme = process_grouping(&line, &type);
		else
			lexeme = process_word(&line, &type);
		if (!lexeme)
			return (free_tokens(&head), NULL);
		append_token(&head, lexeme, type);
		free(lexeme);
	}
	handle_concatenation(&head);
	return (head);
}
