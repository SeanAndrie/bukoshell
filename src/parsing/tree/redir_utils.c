/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:46:38 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/23 17:41:08 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <parsing/tree.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

static char	*heredoc_lexeme(char *line, char *new)
{
	char	*temp;
	char	*joined;

	temp = ft_strjoin(line, "\n");
	free(line);
	if (!temp)
		return (free(new), NULL);
	joined = ft_strjoin(new, temp);
	free(temp);
	free(new);
	if (!joined)
		return (NULL);
	return (joined);
}

char	*handle_heredoc(char *delim)
{
	char	*new;
	char	*line;
	char	*joined;

	new = ft_strdup("");
	if (!new)
		return (false);
	while (true)
	{
		line = readline("> ");
		if (!line)
			return (free(new), NULL);
		if (ft_strncmp(line, delim, ft_strlen(delim)) == 0)
		{
			free(line);
			return (new);
		}
		joined = heredoc_lexeme(line, new);
		if (!joined)
			return (NULL);
		new = joined;
	}
	return (NULL);
}

void	collect_heredocs(t_redirect *head)
{
	while (head)
	{
		if (is_token_type(head->type, T_HEREDOC))
			head->heredoc = handle_heredoc(head->delim);
		head = head->next;
	}
}
