/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 11:51:15 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/27 01:29:07 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <boolean.h>
#include <debug.h>
#include <errno.h>
#include <execute/exec.h>
#include <libft.h>
#include <parsing/tree.h>
#include <signals.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>

t_bool	is_directory(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (FALSE);
	return (S_ISDIR(path_stat.st_mode));
}

void	exec_cmd_error(char *arg, t_bool is_path)
{
	int	exit_code;

	if (is_path && errno == ENOENT)
	{
		log_error(ERROR_NONE, ERR_BASE, "%s: No such file or directory\n", arg);
		exit_code = 127;
	}
	else if (errno == EISDIR)
	{
		log_error(ERROR_NONE, ERR_BASE, "%s: Is a directory\n", arg);
		exit_code = 126;
	}
	else
	{
		log_error(ERROR_NONE, ERR_BASE, "%s: command not found\n", arg);
		exit_code = 127;
	}
	exit(exit_code);
}

void	exec_dir_error(char *arg)
{
	if (is_directory(arg))
	{
		errno = EISDIR;
		exec_cmd_error(arg, TRUE);
	}
}

void	handle_signal(pid_t pid, int *status, char **argv)
{
	int	sig;

	if (waitpid(pid, status, 0) == -1)
	{
		perror("waitpid");
		*status = 1;
		return ;
	}
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
	{
		sig = WTERMSIG(*status);
		if (sig == SIGINT)
			ft_printf("\n");
		else if (sig == SIGQUIT)
		{
			ft_printf("Quit\t\t\t\t(core dumped) ");
			print_argv(argv, FALSE);
		}
		g_signal = 128 + sig;
		*status = 128 + sig;
	}
}

void	handle_missing_command(t_node *node)
{
	int	save_in;
	int	save_out;

	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	if (node->redirect)
		handle_redirections(node->redirect);
	restore_fds(save_in, save_out);
}
