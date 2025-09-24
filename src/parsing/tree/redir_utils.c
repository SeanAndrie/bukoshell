/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:46:38 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/24 20:59:16 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <expand.h>
#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <parsing/clean.h>
#include <parsing/tree.h>
#include <readline/readline.h>

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

static char	*handle_heredoc(t_token *delim, t_map *map)
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
		if (ft_strncmp(line, delim->lexeme, ft_strlen(delim->lexeme)) == 0)
		{
			free(line);
			return (new);
		}
		joined = heredoc_lexeme(line, new);
		if (!joined)
			return (NULL);
		heredoc_expansion(&joined, map, delim->type);
		new = joined;
	}
	return (NULL);
}

void	collect_heredocs(t_node *node, t_map *map)
{
	t_redirect *head;

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
