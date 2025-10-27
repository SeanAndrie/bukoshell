/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 13:23:11 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/27 13:50:12 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <fcntl.h>
#include <stdio.h>
#include <signals.h>
#include <boolean.h>
#include <sys/wait.h>
#include <parsing/clean.h>
#include <parsing/tree.h>
#include <parsing/valid.h>
#include <execute/builtins.h>
#include <execute/exec.h>
#include <bukoshell.h>

int	exec_node(t_node *node, t_shell_ctx *ctx);

int	exec_command(t_node *node, t_shell_ctx *ctx)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (is_builtin(node))
		return (exec_builtin(node, ctx));
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		set_signals_default();
		exec_external(node, ctx);
		if (node->argv)
			perror(node->argv[0]);
		exit(127);
	}
	else if (pid > 0)
		handle_signal(pid, &status, node->argv);
	else
		status = 1;
	set_signals_interactive();
	return (status);
}

int	exec_subshell(t_node *node, t_shell_ctx *ctx)
{
	pid_t	pid;
	int		status;

	if (node->in_pipeline)
	{
		if ((node->redirect && !handle_redirections(node->redirect))
			|| (node->inner && check_arithmetic(node->inner)))
			return (1);
		exit(exec_node(node->left, ctx));
	}
	pid = fork();
	if (pid == 0)
	{
		set_signals_default();
		if ((node->redirect && !handle_redirections(node->redirect))
			|| (node->inner && check_arithmetic(node->inner)))
			exit(1);
		exit(exec_node(node->left, ctx));
	}
	else if (pid > 0)
		handle_signal(pid, &status, node->argv);
	else
		status = 1;
	set_signals_interactive();
	return (status);
}

int	exec_pipe(t_node *node, t_shell_ctx *ctx)
{
	size_t		i;
	t_pipeline	*pl;
	size_t		n_cmds;
	int			status;

	n_cmds = count_commands(node);
	pl = create_pipeline(node, n_cmds);
	if (!pl)
		return (0);
	exec_pipeline(pl, ctx);
	if (pl->pipes)
        close_pipes(pl->pipes, pl->n_cmds - 1);
	i = 0;
	while (i < pl->n_cmds)
	{
		waitpid(pl->pids[i], &status, 0);
		i++;
	}
	set_signals_interactive();
	free_pipeline(pl, TRUE);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	exec_operator(t_node *node, t_shell_ctx *ctx)
{
	int	status;

	if (node->operand == T_AND)
	{
		status = exec_node(node->left, ctx);
		if (status == 0)
			return (exec_node(node->right, ctx));
		return (status);
	}
	else if (node->operand == T_OR)
	{
		status = exec_node(node->left, ctx);
		if (status != 0)
			return (exec_node(node->right, ctx));
		return (status);
	}
	else if (node->operand == T_PIPE)
		return (exec_pipe(node, ctx));
	return (0);
}

int	exec_node(t_node *node, t_shell_ctx *ctx)
{
	if (node->type == N_COMMAND)
	{
		if (!node->argv || !node->argv[0])
			return (handle_missing_command(node), 0);
		return (exec_command(node, ctx));
	}
	else if (node->type == N_SUBSHELL)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		return (exec_subshell(node, ctx));
	}
	else
		return (exec_operator(node, ctx));
	return (0);
}
