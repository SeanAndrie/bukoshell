/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccastro <ccastro@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/04 08:24:08 by ccastro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

int	exit_status = 0;

int	main(void)
{
	struct termios	term;
	char			*line;
	t_token			*head;

	disable_echoctl(&term);
	while (true)
	{
		exit_status = 0;
		handle_signals();
		line = handle_prompt(PS1);
		if (!line)
			return (ft_printf(EXIT), 0);
		if (ft_strncmp(line, "eof", 3) == 0)
		{
			free(line);
			break ;
		}
		add_history(line);
		head = create_tokens(line);
		free(line);
		print_tokens(head, true);
		if (head)
			free_tokens(&head);
	}
	return (0);
}
