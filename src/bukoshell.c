/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seang <seang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:50:42 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/25 17:17:27 by seang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

int	main(void)
{
	char	*line;
	t_token	*head;

	while (true)
	{
		line = readline(PROMPT);
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
		print_tokens(head);
		if (head)
			free_tokens(&head);
	}
	return (0);
}
