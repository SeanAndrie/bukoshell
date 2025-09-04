/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 19:40:31 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/04 18:59:43 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

static void	print_argv(char **argv)
{
	size_t	i;

	i = 0;
	ft_printf("Command: ");
	while (argv && argv[i])
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	ft_printf("\n");
}

static void	print_redirects(t_redirect *head, int level)
{
	int			i;
	t_redirect	*curr;

	i = -1;
	while (++i < level)
		ft_printf("  |  ");
	curr = head;
	ft_printf("Redirections: ");
	while (curr)
	{
		ft_printf("(");
		if (curr->fd == 0)
			ft_printf("STDIN");
		else
			ft_printf("STDOUT");
		ft_printf(" -> %s", curr->fname);
		ft_printf(") ");
		curr = curr->next;
	}
	ft_printf("\n");
}

static void	print_operator(t_node *node)
{
	ft_printf("Operator: ");
	if (is_token_type(node->operator_type, T_AND))
		ft_printf("&&\n");
	else if (is_token_type(node->operator_type, T_OR))
		ft_printf("||\n");
	else if (is_token_type(node->operator_type, T_PIPE))
		ft_printf("|\n");
	else
		ft_printf("UNKNOWN (%d)\n", node->operator_type);
}

void	print_syntax_tree(t_node *node, int level)
{
	int	i;

	if (!node)
		return ;
	i = -1;
	while (++i < level)
		ft_printf("  ");
	if (node->is_operator)
		print_operator(node);
	else
	{
		ft_printf("└── ");
		if (node->argv)
			print_argv(node->argv);
		else
			ft_printf("[Empty Command]\n");
		if (node->redirect)
			print_redirects(node->redirect, level);
	}
	print_syntax_tree(node->left, level + 1);
	print_syntax_tree(node->right, level + 1);
}
