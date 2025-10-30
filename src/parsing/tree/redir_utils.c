/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 02:51:56 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/29 12:02:44 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <environ.h>
#include <libft.h>
#include <signals.h>
#include <parsing/tokens.h>
#include <parsing/tree.h>

void	*heredoc_interrupt(char *line, char *accum)
{
	if (line)
		free(line);
	free(accum);
	set_signals_interactive();
	return (NULL);
}

void	*heredoc_eof(char *accum)
{
	char	*res;

	if (!accum)
		return (NULL);
	res = ft_strjoin(accum, "\n");
	free(accum);
	set_signals_interactive();
	return (res);
}

char	*heredoc_success(char *line, char *accum, t_map *map,
		t_token_type delim_type)
{
	char	*join;

	free(line);
	set_signals_interactive();
	if (!accum)
		return (NULL);
	join = ft_strjoin(accum, "\n");
	free(accum);
	if (join && ft_strchr(join, '$'))
		heredoc_expansion(&join, map, delim_type);
	return (join);
}
