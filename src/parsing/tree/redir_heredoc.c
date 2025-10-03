/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 00:05:09 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/03 00:35:19 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <signals.h>
#include <parsing/parsing.h>
#include <readline/readline.h>

void	heredoc_expansion(char **join, t_map *map, t_token_type delim_type)
{
	t_token	*tokens;
	t_token	*concat;

	if (is_token_type(delim_type, T_WORD_SQUOTE))
		return ;
	tokens = create_tokens(*join, TRUE);
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

static char	*heredoc_lexeme(char *line, char *accum)
{
	char	*temp;
	char	*joined;

	temp = ft_strjoin(line, "\n");
	free(line);
	if (!temp)
		return (free(accum), NULL);
	joined = ft_strjoin(accum, temp);
	free(temp);
	free(accum);
	return (joined);
}

static char	*handle_heredoc(t_token *delim, t_map *map)
{
	char	*line;
	char	*accum;
	char	*content;

	accum = ft_strdup("");
	if (!accum)
		return (NULL);
	while (g_signal == 128)
	{
		line = readline(PS2);
		if (g_signal > 128)
			return (heredoc_interrupt(line, accum));
		if (!line)
			return (heredoc_eof(accum, delim->lexeme));
		if (ft_strcmp(line, delim->lexeme) == 0)
			return (heredoc_success(line, accum, map, delim->type));
        content = heredoc_lexeme(line, accum);
		if (!content)
        {
            set_signals_prompt();
            return (NULL);
        }
		accum = content;
	}
	return (NULL);
}

void	collect_heredocs(t_node *node, t_map *map)
{
	t_redirect	*head;

	if (!node || !map)
		return ;
	set_signals_heredoc();
	head = node->redirect;
	while (head)
	{
		if (is_token_type(head->type, T_HEREDOC))
			head->heredoc = handle_heredoc(head->delim, map);
		head = head->next;
	}
	if (node->left)
		collect_heredocs(node->left, map);
	if (node->right)
		collect_heredocs(node->right, map);
}


