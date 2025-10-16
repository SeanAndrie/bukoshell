/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 00:15:26 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 23:14:31 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <boolean.h>
#include <parsing/tree.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>

static t_bool is_operator_or_group_token(t_token_type type)
{
	return  (is_token_type(type, TOKEN_CTRL_OP) || is_token_type(type, TOKEN_GROUP));
}

static char	**alloc_argv(t_token *head, size_t n)
{
	size_t	i;
	char	**argv;

	argv = ft_calloc(n + 1, sizeof(char *));
	if (!argv)
		return (NULL);
	i = -1;
	while (head)
	{
		if (is_operator_or_group_token(head->type))
			break ;
		if (!is_token_type(head->type, TOKEN_REDIR_OP)
			&& !is_token_type(head->type, TOKEN_AFTER_REDIR))
		{
			argv[++i] = ft_strdup(head->lexeme);
			if (!argv[i])
			{
				free_str_arr(argv, i);
				return (NULL);
			}
			head->type &= ~TOKEN_AFTER_REDIR;
		}
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
		if (is_operator_or_group_token(curr->type))
			break ;
		if (!is_token_type(curr->type, TOKEN_REDIR_OP)
			&& !is_token_type(curr->type, TOKEN_AFTER_REDIR))
			n++;
		curr = curr->next;
	}
	return (alloc_argv(head, n));
}

void	track_depth(t_token *head, int *depth, t_token_type group_category)
{
	if (is_token_type(head->type, group_category) && is_token_type(head->type,
			TOKEN_GROUP_OPEN))
		(*depth)++;
	else if (is_token_type(head->type, group_category)
		&& is_token_type(head->type, TOKEN_GROUP_CLOSE))
		(*depth)--;
}

void	skip_grouping(t_token **head)
{
	int	depth;

	depth = 1;
	*head = (*head)->next;
	while (*head && depth > 0)
	{
		track_depth(*head, &depth, TOKEN_GROUP);
		*head = (*head)->next;
	}
}
