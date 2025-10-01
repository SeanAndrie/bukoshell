/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:46:38 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/02 03:09:33 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <signals.h>
#include <parsing/parsing.h>
#include <readline/readline.h>

static void	restore_and_free(char *s)
{
	if (s)
		free(s);
	rl_event_hook = NULL;
	set_signals_prompt();
}

static void heredoc_interrupt(char *line, char *new)
{
   if (line)
        free(line);
    restore_and_free(new);
}

static void	heredoc_expansion(char **join, t_map *map, t_token_type delim_type)
{
	t_token	*tokens;
	t_token	*concat;

	if (is_token_type(delim_type, T_WORD_SQUOTE))
		return ;
	tokens = create_tokens(*join);
	if (!tokens)
		return ;
	parameter_expansion(map, tokens);
	concat = concat_tokens(tokens, TOKEN_NONE);
	free_tokens(&tokens);
	if (!concat)
		return ;
	free(*join);
	*join = concat->lexeme;
	free(concat);
}

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

char	*handle_heredoc(t_token *delim, t_map *map)
{
	char	*new;
	char	*line;
	char	*joined;

	set_signals_heredoc();
	new = ft_strdup("");
	if (!new)
		return (NULL);
	while (TRUE)
	{
		line = readline(PS2);
		if (g_signal > 128)
			return (heredoc_interrupt(line, new), NULL);
		if (!line)
			return (restore_and_free(new), log_error(ERROR_WARNING, "bukoshell",
					EOF_MSG, delim->lexeme), NULL);
		if (ft_strcmp(line, delim->lexeme) == 0)
			return (restore_and_free(line), new);
		joined = heredoc_lexeme(line, new);
		if (!joined)
			return (restore_and_free(new), NULL);
		heredoc_expansion(&joined, map, delim->type);
		new = joined;
	}
	return (NULL);
}

