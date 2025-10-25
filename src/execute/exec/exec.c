/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 13:23:11 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/25 07:57:21 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <libft.h>
#include <stdio.h>
#include <signals.h>
#include <boolean.h>
#include <sys/wait.h>
#include <parsing/tree.h>
#include <parsing/clean.h>
#include <parsing/valid.h>
#include <execute/exec.h>
#include <execute/builtins.h>

int			exec_node(t_node *node, t_map *map, char **envp);

static void	handle_missing_command(t_node *node)
{
	int	save_in;
	int	save_out;

	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
    if (node->redirect)
	    handle_redirections(node->redirect);
	restore_fds(save_in, save_out);
}

int	exec_command(t_node *node, t_map *map, char **envp)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (is_builtin(node))
		return (exec_builtin(node, map));
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
        set_signals_default();
		exec_external(node, map, envp);
		if (node->argv)
			perror(node->argv[0]);
		exit(127);
	}
	else if (pid > 0)
        handle_signal(pid, &status);
    else
        status = 1;
    set_signals_interactive();
	return (status);
}

int	exec_subshell(t_node *node, t_map *map, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
        set_signals_default();
        if (node->redirect && !handle_redirections(node->redirect))
            exit(1);
		if (node->inner && check_arithmetic(node->inner))
			exit(1);
		exit(exec_node(node->left, map, envp));
	}
	else if (pid > 0)
        handle_signal(pid, &status);
    else
        status = 1;
    set_signals_interactive();
	return (status);
}

int	exec_operator(t_node *node, t_map *map, char **envp)
{
	int	status;

	if (node->operand == T_AND)
	{
		status = exec_node(node->left, map, envp);
		if (status == 0)
			return (exec_node(node->right, map, envp));
		return (status);
	}
	else if (node->operand == T_OR)
	{
		status = exec_node(node->left, map, envp);
		if (status != 0)
			return (exec_node(node->right, map, envp));
		return (status);
	}
    return (0);
}

int	exec_node(t_node *node, t_map *map, char **envp)
{
	if (node->type == N_COMMAND)
    {
	    if (!node->argv || !node->argv[0])
            return (handle_missing_command(node), 0);
		return (exec_command(node, map, envp));
    }
	else if (node->type == N_SUBSHELL)
    {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
		return (exec_subshell(node, map, envp));
    }
	else
		return (exec_operator(node, map, envp));
	return (0);
}
