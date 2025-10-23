/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/23 22:21:26 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

static t_shell	*init_shell(char **envp)
{
    t_shell	*shell;

    shell = malloc(sizeof(t_shell));
    if (!shell)
        return (NULL);
    shell->status = 0;
    shell->line = NULL;
    shell->head = NULL;
    shell->root = NULL;
    shell->envp = copy_envp(envp);
    ft_memset(shell->cwd, 0, sizeof(shell->cwd));
    shell->map = create_map(environ_size(shell->envp));
    return (shell);
}

static int	shell_loop_interactive(t_shell *shell)
{
	char	*prompt;
    char    *identifier;

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
	}
	return (shell->status);
}

static int	shell_loop_noninteractive(t_shell *shell)
{
	size_t	len;

	len = 0;
	while (TRUE)
	{
		shell->line = get_next_line(STDIN_FILENO);
		if (!shell->line)
			break ;
		len = ft_strlen(shell->line);
		if (len && shell->line[len - 1] == '\n')
			shell->line[len - 1] = '\0';
		start_shell(shell);
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
	set_signals_default();
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
		init_variables(shell->map);
	}
	status = shell_mode(shell);
	if (DEBUG_MODE)
		ft_printf("Status: %d\n", status);
	free_shell(shell, TRUE);
	return (status);
}
