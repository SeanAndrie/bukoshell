/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccastro <ccastro@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:48:06 by ccastro           #+#    #+#             */
/*   Updated: 2025/09/01 18:26:29 by ccastro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

char	*handle_prompt(const char *prompt_display)
{
	char	*line;
	char	quote;

	line = readline(prompt_display);
	if (!line)
		return (NULL);
	while (*line)
	{
		if (ft_strchr("\'", *line))
		{
			quote = *line;
		} 
		line++;
	}
	return (line);
}
