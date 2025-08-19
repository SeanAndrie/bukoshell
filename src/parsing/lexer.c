/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:52:14 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/20 03:33:27 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static void categorize_operator(char c, t_token_type *type, bool is_heredoc_or_redir)
{
    if (c == '|')
        *type = T_PIPE;
    else if (c == '<')
    {
        *type = T_REDIR_IN;
        if (is_heredoc_or_redir)
            *type = T_HEREDOC;
    }
    else if (c == '>')
    {
        *type = T_REDIR_OUT;
        if (is_heredoc_or_redir)
            *type = T_REDIR_APPEND;
    }
}

static char	*process_operator(char **line_ptr, t_token_type *type)
{
    size_t  len;
	char	*operator;
	bool	is_heredoc_or_redir;

	is_heredoc_or_redir = (*(*line_ptr + 1) && ft_strchr("<>", *(*line_ptr
					+ 1)));
    categorize_operator(**line_ptr, type, is_heredoc_or_redir);
    if (is_heredoc_or_redir)
        len = 2;
    else
        len = 1;
    operator = ft_calloc(len + 1, sizeof(char));
    if (!operator)
        return (NULL);
    operator[0] = **line_ptr;
    (*line_ptr)++;
    if (is_heredoc_or_redir)
    {
        operator[1] = **line_ptr;
        (*line_ptr)++;
    }
	return (operator);
}

static char	*process_word(char **line_ptr, t_token_type *type)
{
	char	*start;
	char	*end;
	char	*word;

	*type = T_WORD;
	start = *line_ptr;
	while (**line_ptr && !ft_isspace(**line_ptr))
		(*line_ptr)++;
	end = *line_ptr;
	word = ft_calloc((end - start) + 1, sizeof(char));
	if (!word)
		return (NULL);
	ft_strlcpy(word, start, (end - start) + 1);
	return (word);
}

t_token	*create_tokens(char *line)
{
	t_token_type	type;
	t_token			*head;
	char			*lexeme;

	head = NULL;
	if (!line)
		return (NULL);
	while (*line)
	{
		if (ft_isspace(*line))
			line++;
		if (ft_strchr("|<>", *line))
			lexeme = process_operator(&line, &type);
		else
			lexeme = process_word(&line, &type);
		if (!lexeme)
			return (free_tokens(&head), NULL);
		append_token(&head, lexeme, type);
        free(lexeme);
	}
	return (head);
}
