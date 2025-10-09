/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 02:51:56 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/07 02:51:58 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <libft.h>
#include <environ.h>
#include <signals.h>
#include <parsing/tokens.h>
#include <parsing/tree.h>

void assign_fds(t_redirect *redir, t_token_type token_type)
{
    if (is_token_type(token_type, T_REDIR_IN) || is_token_type(token_type, T_HEREDOC))
        redir->fd = 0;
    else if (is_token_type(token_type, T_REDIR_OUT || is_token_type(token_type, T_REDIR_APPEND)))
        redir->fd = 1;
}

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

char	*heredoc_success(char *line, char *accum, t_map *map,
		t_token_type delim_type)
{
	free(line);
	set_signals_prompt();
	if (accum && ft_strchr(accum, '$'))
        heredoc_expansion(&accum, map, delim_type);
	return (accum);
}
