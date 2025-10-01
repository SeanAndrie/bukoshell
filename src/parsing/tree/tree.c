/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 02:08:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/24 20:54:09 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <parsing/tokens.h>
#include <parsing/tree.h>
#include <signals.h>

static t_node	*create_node(t_token *head, t_node_type type)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->operand = T_NONE;
	if (head && node->type == N_OPERATOR)
		node->operand = head->type;
	else if (node->type == N_COMMAND)
	{
		node->operand = T_NONE;
		node->argv = tokens_to_argv(head);
		if (!node->argv)
			return (free(node), NULL);
		node->redirect = create_redirections(head);
	}
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static t_node	*create_subshell(t_token *start)
{
	t_node	*root;
	t_token	*curr;

	curr = start;
	skip_grouping(&curr);
	root = create_node(NULL, N_SUBSHELL);
	if (!root)
		return (NULL);
	root->left = create_syntax_tree(start->next, curr);
	return (root);
}

t_node	*create_syntax_tree(t_token *start, t_token *end)
{
	t_node	*root;
	t_token	*operand;

	if (!start || start == end)
		return (NULL);
	operand = find_lowest_precedence(start, end);
	if (operand && operand != end)
	{
		root = create_node(operand, N_OPERATOR);
		if (!root)
			return (NULL);
		root->left = create_syntax_tree(start, operand);
		root->right = create_syntax_tree(operand->next, end);
		return (root);
	}
	if (is_token_type(start->type, T_LPAREN))
		return (create_subshell(start));
	return (create_node(start, N_COMMAND));
}

void	collect_heredocs(t_node *node, t_map *map)
{
	t_redirect	*head;

	if (!node || !map)
		return ;
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

