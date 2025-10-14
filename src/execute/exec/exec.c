/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 13:23:11 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/13 12:56:29 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <environ.h>
#include <parsing/tree.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>
#include <execute/execute.h>

int exec_node(t_node *node, t_map *map, char **envp);

static void handle_missing_command(t_node *node)
{
    int save_in;
    int save_out;

    if (node->redirect)
        return ;
    save_in = dup(STDIN_FILENO);
    save_out = dup(STDOUT_FILENO);
    handle_redirections(node->redirect);
    restore_fds(save_in, save_out);
}

int exec_command(t_node *node, t_map *map, char **envp)
{
    pid_t   pid;
    int     status;

    status = 0;
    if (!node->argv || !node->argv[0])
    {
        handle_missing_command(node);
        return (0);
    }
    if (is_builtin(node))
        return (exec_builtin(node, map));
    pid = fork();
    if (pid == 0)
    {
        exec_external(node, map, envp);
        if (node->argv)
            perror(node->argv[0]);
        exit(127);
    }
    else if (pid > 0)
        waitpid(pid, &status, 0);
    else
        perror("fork");
    return (WEXITSTATUS(status));
}

int exec_subshell(t_node *node, t_map *map, char **envp)
{
    pid_t   pid;
    int     ret;
    int     status;
    
    pid = fork();
    if (pid == 0)
    {
        ret = exec_node(node->left, map, envp);
        exit(ret);
    }
    else if (pid > 0)
        waitpid(pid, &status, 0);
    else
        perror("fork");
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return (status);
}

int exec_node(t_node *node, t_map *map, char **envp)
{
    if (node->type == N_COMMAND)
        return (exec_command(node, map, envp));
    else if (node->type == N_SUBSHELL)
        return(exec_subshell(node, map, envp));
    return (0);
}

