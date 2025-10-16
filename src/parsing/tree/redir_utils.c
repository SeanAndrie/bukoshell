/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 02:51:56 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 23:30:27 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <libft.h>
#include <environ.h>
#include <signals.h>
#include <parsing/tree.h>
#include <parsing/tokens.h>

void	*heredoc_interrupt(char *line, char *accum)
{
	if (line)
		free(line);
	free(accum);
    set_signals_interactive();
	return (NULL);
}

void	*heredoc_eof(char *accum, char *delim)
{
	free(accum);
	set_signals_interactive();
	log_error(ERROR_SYNTAX, ERR_BASE, EOF_MSG, delim);
	return (NULL);
}

char	*heredoc_success(char *line, char *accum, t_map *map,
		t_token_type delim_type)
{
    char    *join;

	free(line);
	set_signals_interactive();
    if (!accum)
        return(NULL);
    join = ft_strjoin(accum, "\n");
    free(accum);
	if (join && ft_strchr(join, '$'))
        heredoc_expansion(&join, map, delim_type);
	return (join);
}
