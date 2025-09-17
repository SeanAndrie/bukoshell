/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/17 10:46:35 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

void	free_shell(t_shell *shell, bool full_free)
{
	if (!shell)
		return ;
	if (shell->line)
		free(shell->line);
	if (shell->head)
		free_tokens(&shell->head);
	if (shell->root)
		free_syntax_tree(&shell->root);
	if (full_free)
	{
		if (shell->map)
			free_map(shell->map);
		free(shell);
		clear_history();
	}
}

static int	shell_loop(t_shell *shell)
{
	char	*prompt;
	char	*identifier;

	while (true)
	{
		identifier = create_identifier(shell->map);
		prompt = set_cwd_prompt(shell, (identifier) ? identifier : "unknown");
		free(identifier);
		if (!prompt)
			prompt = PS1;
		if (!prompt)
			return (1);
		shell->line = readline(prompt);
		if (!shell->line)
		{
			ft_printf("exit\n");
			break ;
		}
		if (ft_strncmp(shell->line, "exit", 4) == 0)
			break ;
		add_history(shell->line);
		shell->status = start_shell(shell);
	}
	free(prompt);
	return (shell->status);
}

int	main(int argc, char **argv, char **envp)
{
	int		status;
	t_shell	*shell;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
		return (EXIT_FAILURE);
	// if (DEBUG_MODE)
	// 	print_env(shell->map->order);
	status = shell_loop(shell);
	free_shell(shell, true);
	return (status);
}
