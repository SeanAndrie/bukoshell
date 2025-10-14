/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/13 14:46:44 by sgadinga         ###   ########.fr       */
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

static void	init_shell_variables(t_map *map)
{
	t_environ	*shlvl;
	char		*shlvl_value;

    set_entry(map, "?", "0", TRUE);
	set_entry(map, "OLDPWD", "", FALSE);
	shlvl = search_entry(map , "SHLVL");
	if (!shlvl)
	{
		set_entry(map, "SHLVL", "0", FALSE);
		shlvl = search_entry(map, "SHLVL");
	}
	shlvl_value = ft_itoa(ft_atoi(shlvl->value) + 1);
	set_entry(map, "SHLVL", shlvl_value, FALSE);
	free(shlvl_value);
}

static char	*create_identifier(t_map *map)
{
	t_environ	*user;

	if (!map)
		return (NULL);
	user = search_entry(map, "USER");
	if (!user)
	{
		user = search_entry(map, "LOGNAME");
		if (!user)
			return (NULL);
	}
	return (ft_strdup(user->value));
}

static int	shell_loop(t_shell *shell)
{
	char	*prompt;
	char	*identifier;
    char    *status_str;

	identifier = create_identifier(shell->map);
	while (TRUE)
	{
		prompt = set_prompt(shell, identifier);
		shell->line = readline(prompt);
		free(prompt);
		if (!shell->line)
		{
			ft_printf("exit\n");
			break ;
		}
		add_history(shell->line);
		shell->status = start_shell(shell);
        status_str = ft_itoa(shell->status);
        if (!status_str)
            status_str = ft_strdup("0");
        set_entry(shell->map, "?", status_str, TRUE);
        free(status_str);
	}
	free(identifier);
	return (shell->status);
}

int	main(int argc, char **argv, char **envp)
{
	int		status;
	t_shell	*shell;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	set_signals_prompt();
	if (!shell)
		return (EXIT_FAILURE);
    if (shell->map && shell->envp)
	{
	    init_environ(shell->map, shell->envp);
		init_shell_variables(shell->map);
	}
	status = shell_loop(shell);
	free_shell(shell, TRUE);
	return (status);
}
