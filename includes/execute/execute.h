/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:41:25 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/13 13:13:21 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include <stdio.h>
# include <fcntl.h>
# include <boolean.h>
# include <sys/wait.h>
# include <execute/builtins.h>

int     exec_node(t_node *node, t_map *map, char **envp);
int     exec_builtin(t_node *node, t_map *map);
void    exec_external(t_node *node, t_map *map, char **envp);

void    restore_fds(int in, int out);
t_bool  handle_redirections(t_redirect *head);

#endif

