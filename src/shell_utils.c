/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:35:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/05 03:16:22 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->line)
		free(shell->line);
	if (shell->root)
	{
		free_syntax_tree(&shell->root);
		shell->root = NULL;
	}
	free(shell);
}

t_shell	*init_shell(void)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	handle_signals();
	shell->line = NULL;
	shell->root = NULL;
	shell->status = 0;
	return (shell);
}

t_node	*process_prompt(char *line)
{
	t_token	*head;
	t_node	*root;

	head = create_tokens(line);
	if (!head)
		return (NULL);
	if (DEBUG_MODE)
		print_tokens(head, true);
	root = create_syntax_tree(head);
	free_tokens(&head);
	if (!root)
		return (NULL);
	if (DEBUG_MODE && root)
		print_syntax_tree(root, 0);
	return (root);
}

