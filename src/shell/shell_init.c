/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:20:12 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/10 02:19:41 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>
#include <debug.h>

t_shell	*init_shell(void)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	handle_signals();
	shell->status = 0;
	shell->line = NULL;
	shell->head = NULL;
	shell->root = NULL;
	shell->prompt_mask = 0;
	return (shell);
}

bool	parse_prompt(t_shell *shell)
{
	char	*line;

	if (has_unbalanced_quotes(shell->line))
	{
		line = handle_unclosed_prompt(shell->line);
		free(shell->line);
		if (!line)
			return (false);
		shell->line = line;
	}
	shell->head = create_tokens(shell->line);
	if (!shell->head)
		return (false);
	if (DEBUG_MODE)
		print_tokens(shell->head, true);
	if (!are_valid_tokens(shell->head))
		return (false);
	shell->root = create_syntax_tree(shell->head, NULL);
	if (!shell->root)
		return (false);
	if (DEBUG_MODE)
		print_syntax_tree(shell->root);
	return (true);
}

int	start_shell(t_shell *shell)
{
	if (!parse_prompt(shell))
	{
		free_shell(shell, false);
		return (2);
	}
	free_shell(shell, false);
	return (shell->status);
}
