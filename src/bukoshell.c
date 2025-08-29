/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/30 01:42:29 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

int	main(void)
{
	char	*line;
	t_token	*head;

	while (true)
	{
		line = readline(PS1);
		if (!line)
			break ;
		if (ft_strncmp(line, "eof", 3) == 0)
		{
			free(line);
			break ;
		}
		add_history(line);
		head = create_tokens(line);
		free(line);
		print_tokens(head, false);
		if (head)
			free_tokens(&head);
	}
	return (0);
}
