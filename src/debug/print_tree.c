/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 19:40:31 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/10 02:22:18 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>

void	print_argv(char **argv)
{
	size_t	i;

	if (!argv || !argv[0])
	{
		ft_printf("Command: [empty]\n");
		return ;
	}
	ft_printf("Command: ");
	i = 0;
	while (argv[i])
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	ft_printf("\n");
}

void	print_redirects(t_redirect *head, int level)
{
	int			i;
	t_redirect	*curr;

	i = 0;
	curr = head;
	while (i < level)
	{
		ft_printf("  ");
		i++;
	}
	ft_printf("  ↳ Redirections: ");
	while (curr)
	{
		ft_printf("(");
		if (curr->fd == 0)
			ft_printf("STDIN");
		else
			ft_printf("STDOUT");
		ft_printf(" -> %s) ", curr->fname);
		curr = curr->next;
	}
	ft_printf("\n");
}

void	print_operand(t_node *node)
{
	ft_printf("Operator: ");
	if (is_token_type(node->operand, T_AND))
		ft_printf("&&\n");
	else if (is_token_type(node->operand, T_OR))
		ft_printf("||\n");
	else if (is_token_type(node->operand, T_PIPE))
		ft_printf("|\n");
	else
		ft_printf("UNKNOWN (%d)\n", node->operand);
}

void	recursive_print_tree(t_node *node, int level)
{
	int	i;

	if (!node)
		return ;
	i = -1;
	while (++i < level)
		ft_printf("  ");
	ft_printf("└── ");
	if (node->type == N_OPERATOR)
		print_operand(node);
	else if (node->type == N_SUBSHELL)
	{
		ft_printf("Subshell\n");
		recursive_print_tree(node->left, level + 1);
		return ;
	}
	else
	{
		print_argv(node->argv);
		if (node->redirect)
			print_redirects(node->redirect, level + 1);
	}
	recursive_print_tree(node->left, level + 1);
	recursive_print_tree(node->right, level + 1);
}

void	print_syntax_tree(t_node *node)
{
	ft_printf("Syntax Tree:\n");
	recursive_print_tree(node, 0);
	ft_printf("\n");
}
