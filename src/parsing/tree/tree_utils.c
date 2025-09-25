/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 00:15:26 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/10 01:42:53 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>
#include <parsing/tree.h>

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
		if (is_token_type(head->type, TOKEN_METACHAR))
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
		if (is_token_type(curr->type, TOKEN_REDIR_OP))
			break ;
		n++;
		curr = curr->next;
	}
	return (alloc_argv(head, n));
}

void	track_depth(t_token *head, int *depth)
{
	if (is_token_type(head->type, T_LPAREN))
		(*depth)++;
	else if (is_token_type(head->type, T_RPAREN))
		(*depth)--;
}

void	skip_parentheses(t_token **head)
{
	int	depth;

	depth = 1;
	*head = (*head)->next;
	while (*head && depth > 0)
	{
		track_depth(*head, &depth);
		*head = (*head)->next;
	}
}

t_token	*find_lowest_precedence(t_token *start, t_token *end)
{
	int		depth;
	t_token	*lowest_and_or;
	t_token	*lowest_pipe;

	depth = 0;
	lowest_and_or = NULL;
	lowest_pipe = NULL;
	while (start != end)
	{
		track_depth(start, &depth);
		if (depth == 0)
		{
			if (is_token_type(start->type, T_OR) || is_token_type(start->type,
					T_AND))
				lowest_and_or = start;
			else if (is_token_type(start->type, T_PIPE) && !lowest_pipe)
				lowest_pipe = start;
		}
		start = start->next;
	}
	if (lowest_and_or)
		return (lowest_and_or);
	return (lowest_pipe);
}
