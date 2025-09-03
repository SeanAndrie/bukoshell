/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccastro <ccastro@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:48:06 by ccastro           #+#    #+#             */
/*   Updated: 2025/09/03 17:54:02 by ccastro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

// char	*handle_prompt(const char *prompt_display)
// {
// 	char	*line;
// 	char	*temp;
// 	char	*handler;

// 	line = readline(prompt_display);
// 	temp = line;
// 	if (!temp)
// 		return (NULL);
// 	while (*temp) // echo 'hi
// 	{
// 		if (ft_strchr("\'", *temp))
// 		{
// 			temp++;
// 			while (*temp && !ft_strchr("\'", *temp))
// 				temp++;
// 			if (!*temp)
// 			{
// 				handler = readline(PS2);
// 				if (!handler)
// 					return (NULL);
// 				while ((!*handler || *handler) && !ft_strchr("\'", *handler))
// 				{
// 					handler = readline(PS2);
// 					if (!handler)
// 						return (NULL);
// 					handler++;
// 				}
// 				// free(handler);
// 			}
// 		}
// 		temp++;
// 	}
// 	return (line);
// }

// char	*handle_prompt(const char *prompt_display)
// {
// 	char	*line;
// 	char	*temp;
// 	bool	in_squote;

// 	line = readline(prompt_display);
// 	if (!line)
// 		return (NULL);
// 	temp = line;
// 	while (*temp)
// 	{
// 		if (ft_strchr("\'", *temp))
// 		{
// 			temp++;
// 			in_squote = true;
// 			ft_printf("in!\n");
// 			while (*temp && in_squote)
// 			{
// 				if (ft_strchr("\'", *temp))
// 				{
// 					in_squote = false;
// 					ft_printf("out!\n");
// 				}
// 				temp++;
// 			}
// 		}
// 		else
// 			temp++;
// 	}
// 	return (line);
// }

// char	*handle_prompt(const char *prompt_display)
// {
// 	char	*line;
// 	char	*handler;
// 	char	*temp;
// 	bool	in_squote;

// 	line = readline(prompt_display);
// 	if (!line)
// 		return (NULL);
// 	temp = line;
// 	while (*temp)
// 	{
// 		if (ft_strchr("\'", *temp))
// 		{
// 			in_squote = true;
// 			temp++;
// 		}
// 		while (*temp && in_squote)
// 		{
// 			if (ft_strchr("\'", *temp))
// 				in_squote = false;
// 			temp++;
// 		}
// 		if (in_squote)
// 		{
// 			handler = readline(PS2);
// 			if (!handler)
// 				return (NULL);
// 			while (in_squote)
// 			{
// 				handler = readline(PS2);
// 				if (!handler)
// 					return (NULL);
// 				if (ft_strchr("\'", *handler))
// 					break ;
// 				handler++;
// 			}
// 		}
// 	}
// 	return (line);
// }

// char	*handle_prompt(const char *prompt_display)
// {
// 	char	*line;
// 	char	*temp;
// 	bool	in_squote;
// 	bool	in_dquote;

// 	line = readline(prompt_display);
// 	if (!line)
// 		return (NULL);
// 	temp = line;
// 	in_squote = false;
// 	in_dquote = false;
// 	while (*temp && (!in_squote || !in_dquote))
// 	{
// 		if (ft_strchr("\'", *temp))
// 			in_squote = handle_unclosed_squote(temp);
// 		else if (ft_strchr("\"", *temp))
// 			in_dquote = handle_unclosed_dquote(temp);
// 		else
// 			temp++;
// 	}
// 	// if (in_squote || in_dquote)
// 	return (line);
// }

char	*handle_prompt(const char *prompt_display)
{
	char	*line;
	char	*temp;
	char	*handler;
	bool	in_squote;
	bool	in_dquote;
	
	line = readline(prompt_display);
	if (!line)
		return (NULL);
	temp = line;
	in_squote = false;
	in_dquote = false;
	while (*temp)
	{
		if (ft_strchr("\'", *temp) && !in_dquote)
		{
			if (!in_squote)
				in_squote = true;
			else
				in_squote = false;
		}
		else if (ft_strchr("\"", *temp) && !in_squote)
		{
			if (!in_dquote)
				in_dquote = true;
			else
				in_dquote = false;
		}
		ft_printf("squote: %d\ndquote: %d\n\n", in_squote, in_dquote);
		temp++;
	}
	if (in_squote || in_dquote)
	{
		handler = readline(PS2);
		if (!handler)
			return (NULL);
		temp = handler;
	}
	return (line);
}
