/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 13:23:11 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/22 11:30:33 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <boolean.h>
#include <execute/builtins.h>
#include <execute/exec.h>
#include <fcntl.h>
#include <libft.h>
#include <parsing/clean.h>
#include <parsing/tree.h>
#include <parsing/valid.h>
#include <stdio.h>
#include <sys/wait.h>

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

int	exec_subshell(t_node *node, t_map *map, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
        if (node->redirect && !handle_redirections(node->redirect))
        {
            free_redirects(&node->redirect, TRUE);
            exit(1);
        }
		if (node->inner && check_arithmetic(node->inner))
			exit(1);
		exit(exec_node(node->left, map, envp));
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
	return (127);
}

int	exec_node(t_node *node, t_map *map, char **envp)
{
	if (node->type == N_COMMAND)
		return (exec_command(node, map, envp));
	else if (node->type == N_SUBSHELL)
		return (exec_subshell(node, map, envp));
	else
		return (exec_operator(node, map, envp));
	return (0);
}
