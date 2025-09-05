/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 20:29:44 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/05 03:18:45 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

void	free_str_arr(char **str_arr, int n)
{
	if (n >= 0)
	{
		while (--n >= 0)
			free(str_arr[n]);
	}
	else if (n == -1)
	{
		while (str_arr[++n])
			free(str_arr[n]);
	}
	free(str_arr);
}

void	free_redirects(t_redirect **head)
{
	t_redirect	*next;

	while (*head)
	{
		next = (*head)->next;
		free((*head)->fname);
		free(*head);
		*head = next;
	}
	*head = NULL;
}

void	free_tokens(t_token **head)
{
	t_token	*next;

	while (*head)
	{
		next = (*head)->next;
		free((*head)->lexeme);
		free(*head);
		*head = next;
	}
	*head = NULL;
}

void	free_syntax_tree(t_node **root)
{
	if (!*root)
		return ;
	free_syntax_tree(&(*root)->left);
	free_syntax_tree(&(*root)->right);
	if ((*root)->argv)
		free_str_arr((*root)->argv, -1);
	if ((*root)->redirect)
		free_redirects(&(*root)->redirect);
	free(*(root));
	*root = NULL;
}
