/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/08 01:41:44 by sgadinga         ###   ########.fr       */
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

int main(void)
{
	int		status;
	t_shell *shell;

	shell = init_shell();
	if (!shell)
		return (EXIT_FAILURE);
	while (true)
	{
		shell->line = readline(PS1);
		if (!shell->line)
		{
			ft_printf("exit\n");
			break;
		}
		if (ft_strncmp(shell->line, "exit", 4) == 0)
			break;
		add_history(shell->line);
		shell->status = start_shell(shell);
	}
	status = shell->status;
	free_shell(shell, true);	
	return (status);
}
