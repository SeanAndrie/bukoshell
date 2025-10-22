/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 11:51:15 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/22 11:53:55 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <errno.h>
#include <debug.h>
#include <boolean.h>
#include <sys/stat.h>
#include <execute/exec.h>

static t_bool is_directory(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		log_error(ERROR_NONE, ERR_BASE, "%s: no such file or directory\n",
			path);
		return (FALSE);
	}
	return (S_ISDIR(path_stat.st_mode));
}

void	exec_cmd_error(char *arg)
{
	int	exit_code;

	if (errno == ENOENT)
	{
		log_error(ERROR_NONE, ERR_BASE, "%s: No such file or directory\n", arg);
		exit_code = 127;
	}
	else if (errno == EISDIR)
	{
		log_error(ERROR_NONE, ERR_BASE, "%s: Is a directory\n", arg);
		exit_code = 126;	
	}
	else if (errno == EACCES)
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
		exec_cmd_error(arg);
	}
	if (access(arg, F_OK) == 0 && access(arg, X_OK) != 0)
	{
		errno = EACCES;
		exec_cmd_error(arg);
	}
}
