/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 14:54:08 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/12 21:14:49 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <parsing/tree.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>
#include <execute/execute.h>

static int map_builtin(char **argv, t_map *map)
{
    if (ft_strcmp(argv[0], "echo") == 0)
        return (builtin_echo(argv));
    else if (ft_strcmp(argv[0], "exit") == 0)
        return (builtin_exit(argv));
    else if (ft_strcmp(argv[0], "pwd") == 0)
        return (builtin_pwd());
    else if (ft_strcmp(argv[0], "export") == 0)
        return (builtin_export(argv, map));
    else if (ft_strcmp(argv[0], "cd") == 0)
        return (builtin_cd(argv, map));
    return (0);
}

int exec_builtin(t_node *node, t_map *map)
{
    int status;
    int save_in;
    int save_out;

    if (node->redirect)
    {
        save_in = dup(STDIN_FILENO);
        save_out = dup(STDOUT_FILENO);
    }
    if (node->redirect && !handle_redirections(node->redirect))
    {
        restore_fds(save_in, save_out);
        return (1);
    }
    status = map_builtin(node->argv, map);
    if (node->redirect)
        restore_fds(save_in, save_out);
    return (status);
}

void exec_external(t_node *node, t_map *map, char **envp)
{
    t_environ   *path_var;
    char        *cmd_path;

    if (node->redirect && !handle_redirections(node->redirect))
        exit(1);
    if (ft_strchr(node->argv[0], '/'))
        execve(node->argv[0], node->argv, envp);
    path_var = search_entry(map, "PATH");
    if (!path_var)
    {
        ft_dprintf(STDERR_FILENO, "%s: command not found\n", node->argv[0]);
        exit(127);
    }
    cmd_path = find_cmd_path(node->argv[0], path_var);
    if (!cmd_path)
    {
        ft_dprintf(STDERR_FILENO, "%s: command not found\n", node->argv[0]);
        exit(127);
    }
    execve(cmd_path, node->argv, envp);
    perror(node->argv[0]);
    free(cmd_path);
    exit(127);
}

