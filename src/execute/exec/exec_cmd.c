/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:05:03 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/27 02:33:55 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdio.h>
#include <debug.h>
#include <fcntl.h>
#include <environ.h>
#include <boolean.h>
#include <sys/stat.h>
#include <execute/exec.h>
#include <parsing/parsing.h>
#include <execute/builtins.h>

static char *find_cmd_path(char *cmd, t_environ *path_var)
{
    size_t      i;
    char        **paths;
    char        *cmd_path;

    if (!cmd || !path_var || !path_var->value)
        return (NULL);
    paths = ft_split(path_var->value, ':');
    if (!paths)
        return (NULL);
    i = -1;
    while (paths[++i])
    {
        cmd_path = ft_vstrjoin(2, "/", paths[i], cmd);
        if (!cmd_path)
            break;
        if (access(cmd_path, X_OK) == 0)
        {
            free_str_arr(paths, i);
            return (cmd_path);
        }
        free(cmd_path);
    }
    free_str_arr(paths, i);
    return (NULL);
}

static int map_builtin(char **argv, t_map *map)
{
    if (ft_strcmp(argv[0], "echo") == 0)
        return (builtin_echo(argv));
    else if (ft_strcmp(argv[0], "exit") == 0)
        return (builtin_exit(argv));
    else if (ft_strcmp(argv[0], "pwd") == 0)
        return (builtin_pwd(map));
    else if (ft_strcmp(argv[0], "cd") == 0)
        return (builtin_cd(argv, map));
    else if (ft_strcmp(argv[0], "export") == 0)
        return (builtin_export(argv, map));
    else if (ft_strcmp(argv[0], "unset") == 0)
        return (builtin_unset(argv, map));
    else if (ft_strcmp(argv[0], "env") == 0)
        return (builtin_env(map));
    return (0);
}

int exec_builtin(t_node *node, t_map *map)
{
    t_bool  saved;
    int     status;
    int     save_in;
    int     save_out;

    saved = FALSE;
    if (!node->in_pipeline && node->redirect)
    {
        save_in = dup(STDIN_FILENO);
        save_out = dup(STDOUT_FILENO);
        if (!handle_redirections(node->redirect))
        {
            restore_fds(save_in, save_out);
            return (1);
        }
        saved = TRUE;
    }
    status = map_builtin(node->argv, map);
    if (saved)
        restore_fds(save_in, save_out);
    return (status);
}

void exec_external(t_node *node, t_map *map, char **envp)
{
    t_bool      is_path;
    t_environ   *path_var;
    char        *cmd_path;
    char        buffer[PATH_MAX];

    if (node->redirect && !handle_redirections(node->redirect))
        exit(1);
    is_path = ft_strchr(node->argv[0], '/') != NULL;
    if (is_path)
    {
        exec_dir_error(node->argv[0]);
        execve(node->argv[0], node->argv, envp);
        exec_cmd_error(node->argv[0], is_path);
    }
    path_var = search_entry(map, "PATH");
    if (!path_var)
        exec_cmd_error(node->argv[0], is_path);
    cmd_path = find_cmd_path(node->argv[0], path_var);
    if (!cmd_path)
        exec_cmd_error(node->argv[0], is_path);
    ft_strlcpy(buffer, cmd_path, sizeof(buffer));
    free(cmd_path);
    execve(buffer, node->argv, envp);
    exec_cmd_error(node->argv[0], is_path);
}
