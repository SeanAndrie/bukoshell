/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:03:32 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/27 03:41:04 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <debug.h>
#include <stdio.h>
#include <boolean.h>
#include <signals.h>
#include <sys/wait.h>
#include <parsing/tree.h>
#include <execute/exec.h>

void close_pipes(int **pipes, size_t size)
{
    size_t i;

    if (!pipes || !*pipes)
        return ;
    i = 0;
    while (i < size)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

void    free_pipeline(t_pipeline *pipeline, t_bool full_free)
{
    if (!pipeline)
        return ;
    if (pipeline->pipes)
        free(pipeline->pipes);
    if (full_free)
    {
        if (pipeline->pids)
            free(pipeline->pids);
        if (pipeline->commands)
            free(pipeline->commands);
        free(pipeline);
    }
}

void mark_pipeline_nodes(t_node *node)
{
    if (!node)
        return ;
    node->in_pipeline = TRUE;
    if (node->type == N_SUBSHELL)
        mark_pipeline_nodes(node->left);
    if (node->type == N_OPERATOR)
    {
        mark_pipeline_nodes(node->left);
        mark_pipeline_nodes(node->right);
    }
}

size_t  count_commands(t_node *node)
{
    if (node->type == N_COMMAND || node->type == N_SUBSHELL)
        return (1);
    if (node->operand == T_PIPE)
        return (count_commands(node->left) + count_commands(node->right));
    return (0);
}

size_t  flatten_pipeline(t_node *node, t_node **arr, size_t index)
{
    if (!node)
        return (index);
    if (node->operand == T_PIPE)
    {
        index = flatten_pipeline(node->left, arr, index);
        index = flatten_pipeline(node->right, arr, index);
        return (index);
    }
    mark_pipeline_nodes(node);
    arr[index++] = node;
    return (index);
}
