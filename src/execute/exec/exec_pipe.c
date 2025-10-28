/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 19:23:14 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/28 12:21:28 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdio.h>
#include <signals.h>
#include <parsing/tree.h>
#include <execute/builtins.h>
#include <execute/exec.h>
#include <bukoshell.h>

static int	**alloc_pipes(size_t size)
{
	size_t	i;
	int		*block;
	int		**pipes;

	if (size == 0)
		return (NULL);
	pipes = malloc((sizeof(int *) * size) + (sizeof(int) * 2 * size));
	if (!pipes)
		return (NULL);
	block = (int *)(pipes + size);
	i = 0;
	while (i < size)
	{
		pipes[i] = &block[i * 2];
		if (pipe(pipes[i]) == -1)
		{
			close_pipes(pipes, i);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

static t_node	**alloc_commands(t_node *root, size_t size)
{
	t_node	**pipeline;

	if (!root || size == 0)
		return (NULL);
	pipeline = ft_calloc(size + 1, sizeof(t_node *));
	if (!pipeline)
		return (NULL);
	flatten_pipeline(root, pipeline, 0);
	return (pipeline);
}

t_pipeline	*create_pipeline(t_node *root, size_t n_cmds)
{
	t_pipeline	*pl;

	if (!root)
		return (NULL);
	pl = malloc(sizeof(t_pipeline));
	if (!pl)
		return (NULL);
	pl->n_cmds = n_cmds;
	pl->commands = alloc_commands(root, pl->n_cmds);
	if (!pl->commands)
		return (NULL);
	pl->pipes = alloc_pipes(pl->n_cmds - 1);
	if (!pl->pipes)
	{
		free_pipeline(pl, TRUE);
		return (NULL);
	}
	pl->pids = malloc(sizeof(pid_t) * pl->n_cmds);
	if (!pl->pids)
	{
		free_pipeline(pl, TRUE);
		return (NULL);
	}
	return (pl);
}

static void	exec_pipe_cmd(t_node *node, t_shell_ctx *ctx)
{
	if (node->type == N_SUBSHELL)
		exit(exec_subshell(node, ctx));
	if (node->type == N_COMMAND)
	{
		if (is_builtin(node))
			exit(exec_builtin(node, ctx));
		exec_external(node, ctx);
		if (node->argv)
			perror(node->argv[0]);
		exit(127);
	}
}

void	exec_pipeline(t_pipeline *pl, t_shell_ctx *ctx)
{
	size_t	i;

	if (!pl)
		return ;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	i = 0;
	while (i < pl->n_cmds)
	{
		pl->pids[i] = fork();
		if (pl->pids[i] == -1)
			continue ;
		if (pl->pids[i] == 0)
		{
			set_signals_default();
			if (i > 0 && dup2(pl->pipes[i - 1][0], STDIN_FILENO) < 0)
				exit(1);
			if (i < pl->n_cmds - 1 && dup2(pl->pipes[i][1], STDOUT_FILENO) < 0)
				exit(1);
			close_pipes(pl->pipes, pl->n_cmds - 1);
			exec_pipe_cmd(pl->commands[i], ctx);
			exit(127);
		}
		i++;
	}
}
