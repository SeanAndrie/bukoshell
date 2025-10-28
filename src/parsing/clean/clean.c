/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 20:29:44 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/28 12:12:11 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <boolean.h>
#include <parsing/tokens.h>
#include <parsing/tree.h>

void	free_str_arr(char **str_arr, int n)
{
	if (!str_arr)
		return ;
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

void	free_redirects(t_redirect **head, t_bool close_fds)
{
	t_redirect	*next;

	if (!*head)
		return ;
	while (*head)
	{
		next = (*head)->next;
		if ((*head)->fd >= 0 && close_fds)
		{
			if ((*head)->fd != STDIN_FILENO && (*head)->fd != STDOUT_FILENO
				&& (*head)->fd != STDERR_FILENO)
				close((*head)->fd);
		}
		if ((*head)->fname)
			free((*head)->fname);
		if ((*head)->heredoc)
			free((*head)->heredoc);
		free(*head);
		*head = next;
	}
	*head = NULL;
}

void	free_tokens(t_token **head)
{
	t_token	*next;

	if (!head || !*head)
		return ;
	while (*head)
	{
		next = (*head)->next;
		if ((*head)->lexeme)
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
		free_redirects(&(*root)->redirect, FALSE);
	if ((*root)->inner)
		free_tokens(&(*root)->inner);
	free(*(root));
	*root = NULL;
}
