/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 00:15:26 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/04 12:10:32 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static char	**alloc_argv(t_token *head, size_t n)
{
	size_t	i;
	char	**argv;

	argv = ft_calloc(n + 1, sizeof(char *));
	if (!argv)
		return (NULL);
	i = 0;
	while (head)
	{
		if (is_token_type(head->type, TOKEN_REDIR_OP)
			|| is_token_type(head->type, TOKEN_CTRL_OP))
			break ;
		argv[i] = ft_strdup(head->lexeme);
		if (!argv[i])
			return (free_str_arr(argv, i), NULL);
		i++;
		head = head->next;
	}
	return (argv);
}

char	**tokens_to_argv(t_token *head)
{
	size_t	n;
	t_token	*curr;

	n = 0;
	curr = head;
	while (curr)
	{
		if (is_token_type(curr->type, TOKEN_REDIR_OP)
			|| is_token_type(curr->type, TOKEN_CTRL_OP))
			break ;
		n++;
		curr = curr->next;
	}
	return (alloc_argv(head, n));
}

static void	append_redirect(t_redirect **head, t_redirect *node)
{
	t_redirect	*last;

	if (!node)
		return ;
	if (!*head)
	{
		*head = node;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = node;
}

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
			append_redirect(&redir_head, redir_node);
			token_curr = token_next->next;
			continue ;
		}
		token_curr = token_curr->next;
	}
	return (redir_head);
}
