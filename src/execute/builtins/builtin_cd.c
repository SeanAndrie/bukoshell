/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 20:33:14 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/19 23:07:08 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <debug.h>
#include <environ.h>
#include <sys/stat.h>
#include <parsing/prompts.h>
#include <execute/builtins.h>

static t_bool is_valid_path(char *path)
{
    struct stat path_stat;

    if (stat(path, &path_stat) != 0)
    {
        log_error(ERROR_NONE, ERR_BASE, "%s: no such file or directory\n", path);
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

int builtin_cd(char **argv, t_map *map)
{
    t_environ   *home;
    char        *path;
    t_environ   *pwd;
    char        cwd[PATH_MAX];

    path = argv[1];
    if (!path)
    {
        home = search_entry(map, "HOME");
        if (!home)
        {
            log_error(ERROR_NONE, ERR_BASE, "cd: HOME not set\n");
            return (1);
        }
        path = home->value;
    }
    if (!is_valid_path(path) || chdir(path) < 0)
        return (1);
    pwd = search_entry(map, "PWD");
    if (pwd)
        set_entry(map, "OLDPWD", pwd->value);
    if (getcwd(cwd, sizeof(cwd)))
        set_entry(map, "PWD", cwd);
    return (0);
}
