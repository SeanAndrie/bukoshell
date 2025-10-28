/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 20:33:14 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/28 18:35:18 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <environ.h>
#include <execute/builtins.h>
#include <libft.h>
#include <parsing/prompts.h>
#include <sys/stat.h>

static t_bool	is_valid_path(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		log_error(ERROR_NONE, ERR_BASE, "%s: no such file or directory\n",
			path);
		return (FALSE);
	}
	else if (!S_ISDIR(path_stat.st_mode))
	{
		log_error(ERROR_NONE, ERR_BASE, "%s: not a directory\n", path);
		return (FALSE);
	}
	else if (access(path, X_OK) != 0)
	{
		log_error(ERROR_NONE, ERR_BASE, "cd: %s: permission denied\n", path);
		return (FALSE);
	}
	return (TRUE);
}

static char	*resolve_arg(char **argv, t_map *map)
{
	t_environ	*home;
	t_environ	*oldpwd;

	if (!argv[1])
	{
		home = search_entry(map, "HOME");
		if (!home)
		{
			log_error(ERROR_NONE, ERR_BASE, "cd: HOME not set\n");
			return (NULL);
		}
		return (home->value);
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		oldpwd = search_entry(map, "OLDPWD");
		if (!oldpwd || !*oldpwd->value)
		{
			log_error(ERROR_NONE, ERR_BASE, "cd: OLDPWD not set\n");
			return (NULL);
		}
		return (oldpwd->value);
	}
	return (argv[1]);
}

int	builtin_cd(char **argv, t_map *map)
{
	int			argc;
	char		*path;
	t_environ	*pwd;
	char		cwd[PATH_MAX];

	argc = 0;
	while (argv[argc])
		argc++;
	if (argc > 2)
	{
		log_error(ERROR_NONE, ERR_BASE, "cd: too many arguments\n");
		return (0);
	}
	path = resolve_arg(argv, map);
	if (!path)
		return (1);
	if (!is_valid_path(path) || chdir(path) < 0)
		return (1);
	pwd = search_entry(map, "PWD");
	if (pwd)
		set_entry(map, "OLDPWD", pwd->value);
	if (getcwd(cwd, sizeof(cwd)))
		set_entry(map, "PWD", cwd);
	return (0);
}
