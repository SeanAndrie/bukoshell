/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 22:36:08 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/27 00:51:03 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <boolean.h>
#include <debug.h>
#include <environ.h>
#include <execute/exec.h>
#include <fcntl.h>
#include <libft.h>
#include <parsing/parsing.h>
#include <stdio.h>

t_bool	redirect_heredoc(t_redirect *redir)
{
	int	fds[2];

	if (pipe(fds) != 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: ", ERR_BASE);
		perror("pipe");
		return (FALSE);
	}
	if (!redir->heredoc)
	{
		close(fds[0]);
		close(fds[1]);
		return (FALSE);
	}
	ft_putstr_fd(redir->heredoc, fds[1]);
	close(fds[1]);
	if (dup2(fds[0], STDIN_FILENO) < 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: ", ERR_BASE);
		perror("dup2");
		close(fds[0]);
		return (FALSE);
	}
	close(fds[0]);
	return (TRUE);
}

void	restore_fds(int in, int out)
{
	if (dup2(in, STDIN_FILENO) < 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: ", ERR_BASE);
		perror("dup2");
	}
	close(in);
	if (dup2(out, STDOUT_FILENO) < 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: ", ERR_BASE);
		perror("dup2");
	}
	close(out);
}

static t_bool	open_file(t_redirect *redir)
{
	if (ft_strcmp(redir->fname, "*") == 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: %s: ambiguous redirection\n", ERR_BASE,
			redir->fname);
		return (FALSE);
	}
	if (is_token_type(redir->type, T_REDIR_OUT))
		redir->fd = open(redir->fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (is_token_type(redir->type, T_REDIR_APPEND))
		redir->fd = open(redir->fname, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (is_token_type(redir->type, T_REDIR_IN))
		redir->fd = open(redir->fname, O_RDONLY);
	else
		return (FALSE);
	if (redir->fd < 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: ", ERR_BASE);
		perror(redir->fname);
		return (FALSE);
	}
	return (TRUE);
}

static void	redirect_fds(t_redirect *redir)
{
	if (is_token_type(redir->type, T_REDIR_IN))
	{
		if (dup2(redir->fd, STDIN_FILENO) < 0)
		{
			ft_dprintf(STDERR_FILENO, "%s: ", ERR_BASE);
			perror("dup2");
		}
	}
	else
	{
		if (dup2(redir->fd, STDOUT_FILENO) < 0)
		{
			ft_dprintf(STDERR_FILENO, "%s: ", ERR_BASE);
			perror("dup2");
		}
	}
}

t_bool	handle_redirections(t_redirect *head)
{
	t_redirect	*curr;

	curr = head;
	while (curr)
	{
		if (is_token_type(curr->type, T_HEREDOC))
		{
			if (!redirect_heredoc(curr))
				return (FALSE);
		}
		else
		{
			if (!open_file(curr))
				return (FALSE);
			redirect_fds(curr);
			close(curr->fd);
		}
		curr = curr->next;
	}
	return (TRUE);
}
