/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 23:35:53 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/27 13:53:33 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

typedef enum e_bool t_bool;
typedef struct s_map t_map;
typedef struct s_node t_node;
typedef struct s_token t_token;
typedef struct s_redirect t_redirect;

typedef struct s_shell t_shell;

typedef struct s_shell_ctx 
{
    struct s_shell  *shell;
}                   t_shell_ctx;

typedef struct s_pipeline
{
    size_t          n_cmds;
    pid_t           *pids;
    int             **pipes;
    struct s_node   **commands;
}                   t_pipeline;

int         exec_node(t_node *node, t_shell_ctx *ctx);
int         exec_builtin(t_node *node, t_shell_ctx *ctx);
void        exec_external(t_node *node, t_shell_ctx *ctx);
int	        exec_subshell(t_node *node, t_shell_ctx *ctx);
void        exec_pipeline(t_pipeline *pipeline, t_shell_ctx *ctx);

void        restore_fds(int in, int out);
t_bool      handle_redirections(t_redirect *head);

t_bool      is_directory(char *path);
void	    handle_missing_command(t_node *node);
void	    exec_dir_error(char *arg);
void        exec_cmd_error(char *arg, t_bool is_path);
void        handle_signal(pid_t pid, int *status, char **argv);

t_pipeline  *create_pipeline(t_node *root, size_t n_cmds);

size_t      count_commands(t_node *node);
void        mark_pipeline_nodes(t_node *node);
size_t      flatten_pipeline(t_node *node, t_node **arr, size_t index);

void        close_pipes(int **pipes, size_t size);
void        free_pipeline(t_pipeline *pipeline, t_bool full_free);

#endif
