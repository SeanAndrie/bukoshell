/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/15 14:45:16 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

void	free_shell(t_shell *shell, t_bool full_free)
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
		if (shell->envp)
			free_str_arr(shell->envp, -1);
		if (shell->map)
			free_map(shell->map);
		free(shell);
		clear_history();
	}
}

static int	shell_loop_interactive(t_shell *shell)
{
	char	*prompt;
	char	*identifier;

	while (TRUE)
	{
		identifier = create_identifier(shell->map);
		prompt = set_prompt(shell, identifier);
		free(identifier);
		shell->line = readline(prompt);
		free(prompt);
		if (!shell->line)
		{
			ft_printf("exit\n");
			break ;
		}
		add_history(shell->line);
		start_shell(shell);
		update_status(shell);
	}
	return (shell->status);
}

static int	shell_loop_noninteractive(t_shell *shell)
{
	size_t	len;
	char	*line;

	len = 0;
	while (TRUE)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		len = ft_strlen(line);
		if (len && line[len - 1] == '\n')
			line[len - 1] = '\0';
		shell->line = line;
		start_shell(shell);
		update_status(shell);
	}
	return (shell->status);
}

static int	shell_mode(t_shell *shell)
{
	if (isatty(STDIN_FILENO))
	{
		set_signals_interactive();
		return (shell_loop_interactive(shell));
	}
	set_signals_noninteractive();
	return (shell_loop_noninteractive(shell));
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
	if (shell->map && shell->envp)
	{
		init_environ(shell->map, shell->envp);
		init_shell_variables(shell->map);
	}
	status = shell_mode(shell);
	if (DEBUG_MODE)
		ft_printf("Status: %d\n", status);
	free_shell(shell, TRUE);
	return (status);
}
