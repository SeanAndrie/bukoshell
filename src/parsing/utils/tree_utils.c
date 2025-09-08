/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 00:15:26 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/07 19:04:49 by sgadinga         ###   ########.fr       */
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

static inline void skip_parentheses(t_token **head)
{
	int depth;

	depth = 1;
	*head = (*head)->next;
	while (*head && depth > 0)
	{
		if (is_token_type((*head)->type, T_LPAREN))
			depth++;
		else if (is_token_type((*head)->type, T_RPAREN))
			depth--;
		*head = (*head)->next;
	}
}

t_token	*find_lowest_precedence(t_token *head)
{
	t_token	*first_pipe;
	t_token	*first_and_or;

	first_pipe = NULL;
	first_and_or = NULL;
	while (head)
	{
		if (is_token_type(head->type, T_LPAREN))
		{
			skip_parentheses(&head);
			continue;	
		}
		if (is_token_type(head->type, T_AND) || is_token_type(head->type, T_OR))
		{
			first_and_or = head;
			break ;
		}
		else if (is_token_type(head->type, T_PIPE) && !first_pipe)
			first_pipe = head;
		head = head->next;
	}
	if (first_and_or)
		return (first_and_or);
	return (first_pipe);
}
