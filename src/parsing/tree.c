/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 02:08:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/04 12:04:19 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static t_node	*create_node(t_token *head, bool is_operator)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->is_operator = is_operator;
	if (node->is_operator)
	{
		node->operator_type = head->type;
		node->argv = NULL;
		node->redirect = NULL;
	}
	else
	{
		node->operator_type = TOKEN_NONE;
		node->argv = tokens_to_argv(head);
		if (!node->argv)
			return (free(node), NULL);
		node->redirect = create_redirections(head);
	}
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static t_token	*find_lowest_precedence(t_token *head)
{
	t_token	*first_pipe;
	t_token	*first_and_or;

	first_pipe = NULL;
	first_and_or = NULL;
	while (head)
	{
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

t_node	*create_syntax_tree(t_token *head)
{
	t_node	*root;
	t_token	**curr;
	t_token	*operator;

	operator= find_lowest_precedence(head);
	if (!operator)
		return (create_node(head, false));
	root = create_node(operator, true);
	if (!root)
		return (NULL);
	curr = &head;
	while (*curr && *curr != operator)
		curr = &(*curr)->next;
	if (curr && *curr == operator)
		*curr = NULL;
	root->left = create_syntax_tree(head);
	root->right = create_syntax_tree(operator->next);
	return (root);
}
