/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/16 20:22:30 by sgadinga         ###   ########.fr       */
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
		free(shell);
		clear_history();
	}
}

static int	shell_loop(t_shell *shell)
{
	char	*prompt;

	while (true)
	{
		prompt = set_cwd_prompt(shell);
		if (!prompt)
			prompt = ft_strdup(PS1); 
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
	return (shell->status);
}

int	main(void)
{
	int		status;
	t_shell	*shell;

	shell = init_shell();
	if (!shell)
		return (EXIT_FAILURE);
	status = shell_loop(shell);
	free_shell(shell, true);
	return (status);
}
