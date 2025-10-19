/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 23:35:53 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/19 22:57:21 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

typedef enum e_bool t_bool;
typedef struct s_map t_map;
typedef struct s_node t_node;
typedef struct s_token t_token;
typedef struct s_redirect t_redirect;

int     exec_node(t_node *node, t_map *map, char **envp);
int     exec_builtin(t_node *node, t_map *map);
void    exec_external(t_node *node, t_map *map, char **envp);

void    exec_cmd_error(char *arg);
void	exec_dir_error(char *arg);
void    restore_fds(int in, int out);
t_bool  handle_redirections(t_redirect *head);

#endif
