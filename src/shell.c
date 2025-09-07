/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/07 16:02:56 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

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
	free_shell(shell);	
	return (status);
}
