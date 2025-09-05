/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/05 03:31:20 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

int main(void)
{
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
			free_shell(shell);
			return (EXIT_SUCCESS);
		}
		add_history(shell->line);
		shell->root = process_prompt(shell->line);
		if (shell->root)
		{
			// Execution here ...
			free_syntax_tree(&shell->root);
		}
		free(shell->line);
	}
	return (EXIT_SUCCESS);
}
