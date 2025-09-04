/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/04 18:55:35 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

int	exit_status = 0;

int	main(void)
{
	char			*line;
	t_token			*head;
	t_node			*root;

	while (true)
	{
		exit_status = 0;
		handle_signals();
		line = readline(PS1);
		if (!line)
			return (ft_printf(EXIT), 0);
		if (ft_strncmp(line, "exit", 4) == 0)
		{
			free(line);
			break ;
		}
		add_history(line);
		head = create_tokens(line);
		free(line);
		print_tokens(head, true);
		root = create_syntax_tree(head);
		print_syntax_tree(root, 0);
		if (head)
			free_tokens(&head);
	}
	return (0);
}
