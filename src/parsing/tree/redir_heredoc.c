/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 00:05:09 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/09 16:46:55 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <signals.h>
#include <parsing/parsing.h>
#include <readline/readline.h>

int heredoc_event_hook(void)
{
    if (g_signal != 0)
    {
        g_signal = 0;
        rl_done = 0;
    }
    return (0);
}

void heredoc_expansion(char **join, t_map *map, t_token_type delim_type)
{
    t_token *tokens;
    t_token *concat;

    if (is_token_type(delim_type, TOKEN_QUOTE))
        return  ;
    tokens = create_tokens(*join, TRUE, TRUE);
    if (!tokens)
        return ;
    apply_expansions(&tokens, map, TRUE);
    concat = concat_tokens(tokens, TOKEN_PARAMETER);
    free_tokens(&tokens);
    if (!concat)
        return ;
    free(*join);
    *join = concat->lexeme;
    free(concat);
}

static char	*handle_heredoc(t_token *delim, t_map *map)
{
	char	*line;
	char	*accum;
	char	*joined;

	accum = NULL;
	set_signals_heredoc();
	while (g_signal == 0)
	{
		line = readline(PS2);
		if (g_signal != 0)
			return (heredoc_interrupt(line, accum));
		if (!line)
			return (heredoc_eof(accum, delim->lexeme));
		if (ft_strcmp(line, delim->lexeme) == 0)
			return (heredoc_success(line, accum, map, delim->type));
		joined = ft_vstrjoin(2, "\n", accum, line);
		free(line);
		line = NULL;
		if (!joined)
			return (heredoc_interrupt(line, accum));
		free(accum);
		accum = joined;
	}
	return (NULL);
}

void	collect_heredocs(t_node *node, t_map *map)
{
	t_redirect	*head;

	if (!node || !map)
		return ;
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
