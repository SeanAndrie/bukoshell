/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 15:15:08 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/19 22:57:42 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <boolean.h>
#include <debug.h>
#include <execute/builtins.h>
#include <libft.h>
#include <parsing/tree.h>
#include <sys/stat.h>

t_bool	is_builtin(t_node *node)
{
	if (!node || !node->argv)
		return (FALSE);
	if (ft_strcmp(node->argv[0], "cd") == 0)
		return (TRUE);
	else if (ft_strcmp(node->argv[0], "echo") == 0)
		return (TRUE);
	else if (ft_strcmp(node->argv[0], "export") == 0)
		return (TRUE);
	else if (ft_strcmp(node->argv[0], "unset") == 0)
		return (TRUE);
	else if (ft_strcmp(node->argv[0], "env") == 0)
		return (TRUE);
	else if (ft_strcmp(node->argv[0], "exit") == 0)
		return (TRUE);
	else if (ft_strcmp(node->argv[0], "pwd") == 0)
		return (TRUE);
	return (FALSE);
}

t_bool	is_valid_identifier(char *key)
{
	size_t	i;

	if (!key || !*key)
		return (FALSE);
	if (!(ft_isalpha(*key) || *key == '_'))
		return (FALSE);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

t_bool	is_directory(char *path)
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

void	exec_dir_error(char *arg)
{
	if (is_directory(arg))
	{
		log_error(ERROR_NONE, ERR_BASE, "%s: Is a directory\n", arg);
		exit(126);
	}
}

void	exec_cmd_error(char *arg)
{
	log_error(ERROR_NONE, ERR_BASE, "%s: command not found\n", arg);
	exit(127);
}
