/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 00:20:12 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/25 13:54:17 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <bukoshell.h>

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

static bool	parse_prompt(t_shell *shell)
{
	shell->head = create_tokens(shell->line);
	if (!shell->head)
		return (false);
	if (!handle_concatenation(&shell->head))
		return (false);
	remove_tokens(&shell->head, TOKEN_WHITESPACE);
	if (!validate_tokens(shell->head))
		return (false);
	shell->token_mask = create_token_mask(shell->head);
	if ((shell->token_mask & TOKEN_WORD))
		parameter_expansion(shell->map, shell->head);
	if (DEBUG_MODE)
		print_tokens(shell->head, true);
	shell->root = create_syntax_tree(shell->head, NULL);
	if (!shell->root)
		return (false);
	collect_heredocs(shell->root, shell->map);
	if (DEBUG_MODE)
		print_syntax_tree(shell->root);
	return (true);
}

int	start_shell(t_shell *shell)
{
	if (shell->map->load_factor >= LOAD_THRESHOLD)
		shell->map = realloc_map(shell->map, shell->envp);
	if (!parse_prompt(shell))
	{
		free_shell(shell, false);
		return (2);
	}
	shell->token_mask = 0;
	free_shell(shell, false);
	return (shell->status);
}
