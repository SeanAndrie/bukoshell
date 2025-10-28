/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 00:21:11 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/28 12:17:33 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <boolean.h>
#include <debug.h>
#include <libft.h>
#include <parsing/tokens.h>
#include <parsing/tree.h>

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
		track_depth(start, &depth, TOKEN_GROUP);
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

static t_node	*create_node(t_token *head, t_node_type type)
{
	t_node	*node;
	t_token	*end;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = type;
	if (head && node->type == N_OPERATOR)
		node->operand = head->type;
	else if (node->type == N_COMMAND)
	{
		end = head;
		while (end && !is_operator_or_group_token(end->type))
			end = end->next;
		node->redirect = create_redirections(head, end);
		node->argv = tokens_to_argv(head, end);
		if (!node->argv)
		{
			free(node);
			return (NULL);
		}
	}
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static t_node	*create_subshell(t_token *start, t_token *end)
{
	t_node		*root;
	t_token		*curr;
	t_token		*last;
	t_redirect	*redir;

	curr = start;
	redir = NULL;
	skip_grouping(&curr);
	if (curr && is_token_type(curr->type, TOKEN_REDIR_OP))
		redir = create_redirections(curr, end);
	root = create_node(NULL, N_SUBSHELL);
	if (!root)
		return (NULL);
	if (redir)
		root->redirect = redir;
	last = start;
	while (last && last->next != curr && last != end)
		last = last->next;
	root->inner = copy_tokens(start->next, last);
	root->left = create_syntax_tree(start->next, last);
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
		return (create_subshell(start, end));
	return (create_node(start, N_COMMAND));
}
