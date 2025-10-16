/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 02:07:20 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 23:16:28 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <boolean.h>
#include <parsing/tree.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>

static void	*free_helper(t_redirect *redir)
{
	free(redir);
	return (NULL);
}

static t_redirect	*create_redirect(t_token *token)
{
	t_redirect	*redir;

	redir = ft_calloc(1, sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = token->type;
    redir->fd = -1;
	if (token->next && is_token_type(token->type, T_HEREDOC))
	{
		redir->delim = token->next;
        redir->delim->type |= TOKEN_AFTER_REDIR;
		if (!redir->delim)
            return (free_helper(redir));
		redir->heredoc = NULL;
		return (redir);
	}
	redir->fname = ft_strdup(token->next->lexeme);
	if (!redir->fname)
        return (free_helper(redir));
    token->next->type |= TOKEN_AFTER_REDIR;
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

static t_bool	is_valid_redir(t_token *curr, t_token *next)
{
	return (next && is_token_type(curr->type, TOKEN_REDIR_OP)
		&& is_token_type(next->type, TOKEN_WORD));
}

t_redirect	*create_redirections(t_token *head)
{
	t_token		*curr;
	t_token		*next;
	t_redirect	*redir_head;

	curr = head;
	redir_head = NULL;
	while (curr)
	{
		next = curr->next;
		if (is_token_type(curr->type, TOKEN_REDIR_OP))
		{
			if (next && is_valid_redir(curr, next))
			{
				if (!append_redirect(&redir_head, curr))
				{
					free_redirects(&redir_head, FALSE);
					return (NULL);
				}
				curr = next->next;
				continue ;
			}
		}
		curr = curr->next;
	}
	return (redir_head);
}
