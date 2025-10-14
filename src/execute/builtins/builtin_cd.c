/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 20:33:14 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/12 21:09:07 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <debug.h>
#include <libft.h>
#include <environ.h>
#include <sys/stat.h>
#include <parsing/prompts.h>
#include <execute/builtins.h>

static t_bool is_valid_path(char *path)
{
    struct stat st;

    if (stat(path, &st) != 0)
    {
        log_error(ERROR_NONE, ERR_BASE, "cd: no such file or directory\n", path);
        return (FALSE);
    }
    else if (!S_ISDIR(st.st_mode))
    {
        log_error(ERROR_NONE, ERR_BASE, "cd: %s: not a directory\n", path);
        return (FALSE);
    }
    else if (access(path, X_OK) != 0)
    {
        log_error(ERROR_NONE, ERR_BASE, "cd: %s: permission denied", path);
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
            return (1);
        path = home->value;
    }
    if (!is_valid_path(path))
        return (1);
    if (chdir(path) < 0)
        return (1);
    pwd = search_entry(map, "PWD");
    if (pwd)
        set_entry(map, "OLDPWD", pwd->value, FALSE);
    if (getcwd(cwd, sizeof(cwd)))
        set_entry(map, "PWD", cwd, FALSE);
    return (0);
}

