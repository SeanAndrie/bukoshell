/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 19:40:31 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/07 16:02:55 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

static void print_argv(char **argv)
{
    size_t i;

    if (!argv || !argv[0])
    {
        ft_printf("Command: [empty]\n");
        return;
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

static void print_redirects(t_redirect *head, int level)
{
    int i;
    t_redirect *curr = head;

	i = 0;
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

static void print_operator(t_node *node)
{
    ft_printf("Operator: ");
    if (is_token_type(node->operator, T_AND))
        ft_printf("&&\n");
    else if (is_token_type(node->operator, T_OR))
        ft_printf("||\n");
    else if (is_token_type(node->operator, T_PIPE))
        ft_printf("|\n");
    else
        ft_printf("UNKNOWN (%d)\n", node->operator);
}

void print_syntax_tree(t_node *node, int level)
{
    int i;

    while (node)
    {
        i = -1;
        while (++i < level)
            ft_printf("  ");
        ft_printf("└── ");
        if (node->type == N_OPERATOR)
            print_operator(node);
        else if (node->type == N_SUBSHELL)
        {
            ft_printf("Subshell\n");
            if (node->left)
                print_syntax_tree(node->left, level + 1);
        }
        else 
        {
            print_argv(node->argv);
            if (node->redirect)
                print_redirects(node->redirect, level + 1);
        }
        node = node->right;
    }
}
