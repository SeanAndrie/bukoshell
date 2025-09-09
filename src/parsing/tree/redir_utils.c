/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:07:20 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/10 01:43:41 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing/tokens.h>
#include <parsing/clean.h>
#include <parsing/tree.h>
#include <stdbool.h>
#include <libft.h>

static t_redirect	*create_redirect(t_token_type token_type, char *filename)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	if (is_token_type(token_type, T_REDIR_IN))
		redir->fd = 0;
	else if (is_token_type(token_type, T_REDIR_OUT) || is_token_type(token_type,
			T_REDIR_APPEND))
		redir->fd = 1;
	redir->fname = ft_strdup(filename);
	if (!redir->fname)
		return (free(redir), NULL);
	redir->next = NULL;
	return (redir);
}

static bool	append_redirect(t_redirect **head, t_redirect *node)
{
	t_redirect	*last;

	if (!node)
		return (false);
	if (!*head)
	{
		*head = node;
		return (true);
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = node;
	return (true);
}

t_redirect	*create_redirections(t_token *head)
{
	t_token		*token_curr;
	t_token		*token_next;
	t_redirect	*redir_node;
	t_redirect	*redir_head;

	token_curr = head;
	redir_head = NULL;
	while (token_curr)
	{
		token_next = token_curr->next;
		if (token_next && is_token_type(token_curr->type, TOKEN_REDIR_OP)
			&& is_token_type(token_next->type, TOKEN_WORD))
		{
			redir_node = create_redirect(token_curr->type, token_next->lexeme);
			if (!redir_node)
				return (free_redirects(&redir_head), NULL);
			if (!append_redirect(&redir_head, redir_node))
				return (free(redir_node), free_redirects(&redir_head), NULL);
			token_curr = token_next->next;
			continue ;
		}
		token_curr = token_curr->next;
	}
	return (redir_head);
}
