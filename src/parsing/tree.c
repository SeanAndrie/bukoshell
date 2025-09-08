/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 02:08:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/07 19:04:48 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static t_node	*create_node(t_token *head, t_node_type type)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->operator = T_NONE;
	if (head && node->type == N_OPERATOR)
		node->operator = head->type;
	else if (node->type == N_COMMAND)
	{
		node->operator = T_NONE;
		node->argv = tokens_to_argv(head);
		if (!node->argv)
			return (NULL);
		node->redirect = create_redirections(head);
	}
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_node	*create_syntax_tree(t_token *head)
{
	t_node	*root;
	t_token	**curr;
	t_token	*operator;

	if (!head)
		return (NULL);
	operator= find_lowest_precedence(head);
	if (!operator)
		return (create_node(head, N_COMMAND));
	root = create_node(operator, N_OPERATOR);
	if (!root)
		return (NULL);
	curr = &head;
	while (*curr && *curr != operator)
		curr = &(*curr)->next;
	if (curr && *curr == operator)
		*curr = NULL;
	print_tokens(head, false);
	print_tokens(operator->next, false);
	root->left = create_syntax_tree(head);
	root->right = create_syntax_tree(operator->next);
	free_tokens(&operator);
	return (root);
}
