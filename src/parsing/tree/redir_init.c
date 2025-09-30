/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:07:20 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/27 01:37:04 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>
#include <parsing/tree.h>

static t_redirect	*create_redirect(t_token *token)
{
	t_redirect	*redir;

	redir = ft_calloc(1, sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = token->type;
	if (is_token_type(token->type, T_REDIR_IN))
		redir->fd = 0;
	else if (is_token_type(token->type, T_REDIR_OUT)
		|| is_token_type(token->type, T_REDIR_APPEND))
		redir->fd = 1;
	else if (token->next && is_token_type(token->type, T_HEREDOC))
	{
		redir->fd = 0;
		redir->delim = token->next;
		if (!redir->delim)
			return (free(redir), NULL);
		redir->heredoc = NULL;
		return (redir);
	}
	redir->fname = ft_strdup(token->next->lexeme);
	if (!redir->fname)
		return (free(redir), NULL);
	redir->next = NULL;
	return (redir);
}

static t_bool	append_redirect(t_redirect **head, t_token *token)
{
	t_redirect	*node;
	t_redirect	*last;

	node = create_redirect(token);
	if (!node)
		return (FALSE);
	if (!*head)
	{
		*head = node;
		return (TRUE);
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = node;
	return (TRUE);
}

t_redirect	*create_redirections(t_token *head)
{
	t_token		*token_curr;
	t_token		*token_next;
	t_redirect	*redir_head;

	token_curr = head;
	redir_head = NULL;
	while (token_curr)
	{
		token_next = token_curr->next;
		if (is_token_type(token_curr->type, TOKEN_CTRL_OP))
			break ;
		if (token_next && is_token_type(token_curr->type, TOKEN_REDIR_OP)
			&& is_token_type(token_next->type, TOKEN_WORD))
		{
			if (!append_redirect(&redir_head, token_curr))
				return (free_redirects(&redir_head), NULL);
			token_curr = token_next->next;
			continue ;
		}
		token_curr = token_curr->next;
	}
	return (redir_head);
}
