/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_heredoc_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 00:37:09 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/03 00:38:18 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <debug.h>
#include <signals.h>
#include <environ.h>
#include <parsing/tree.h>
#include <parsing/tokens.h>

void	*heredoc_interrupt(char *line, char *accum)
{
	if (line)
		free(line);
	free(accum);
	set_signals_prompt();
	return (NULL);
}

void	*heredoc_eof(char *accum, char *delim)
{
	free(accum);
	set_signals_prompt();
	log_error(ERROR_SYNTAX, ERR_BASE, EOF_MSG, delim);
	return (NULL);
}

char	*heredoc_success(char *line, char *accum, t_map *map, t_token_type delim_type)
{
	free(line);
	set_signals_prompt();
    heredoc_expansion(&accum, map, delim_type);
	return (accum);
}

