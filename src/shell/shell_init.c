/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:20:12 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/27 05:36:25 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>
#include <debug.h>

t_shell	*init_shell(char **envp)
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
	shell->token_mask = 0;
	shell->envp = copy_envp(envp);
	ft_memset(shell->cwd, 0, sizeof(shell->cwd));
	shell->map = create_map(environ_size(shell->envp));
	return (shell);
}

static t_bool	parse_prompt(t_shell *shell)
{
	shell->head = create_tokens(shell->line);
	if (!shell->head)
		return (FALSE);
	parameter_expansion(shell->map, shell->head);
	if (!handle_concatenation(&shell->head))
		return (FALSE);
	remove_tokens(&shell->head, TOKEN_WHITESPACE);
	if (!validate_tokens(shell->head))
		return (FALSE);
	shell->token_mask = create_token_mask(shell->head);
	handle_arithmetic(&shell->head);
	if (shell->token_mask & TOKEN_ARITH)
	{
		if (!validate_arithmetic(shell->head, NULL))
			return (FALSE);
	}
	if (DEBUG_MODE)
		print_tokens(shell->head, TRUE);
	shell->root = create_syntax_tree(shell->head, NULL);
	if (!shell->root)
		return (FALSE);
	collect_heredocs(shell->root, shell->map);
	if (DEBUG_MODE)
		print_syntax_tree(shell->root);
	return (TRUE);
}

int	start_shell(t_shell *shell)
{
	if (shell->map->load_factor >= LOAD_THRESHOLD)
		shell->map = realloc_map(shell->map, shell->envp);
	if (!parse_prompt(shell))
	{
		free_shell(shell, FALSE);
		return (2);
	}
	free_shell(shell, FALSE);
	return (shell->status);
}
