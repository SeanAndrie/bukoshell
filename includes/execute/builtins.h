/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 11:49:40 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/13 21:06:04 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <parsing/tree.h>

int     builtin_pwd(void);
int     builtin_env(t_map *map);
int     builtin_echo(char **argv);
int     builtin_exit(char **argv);
int     builtin_cd(char **argv, t_map *map);
int     builtin_export(char **argv, t_map *map);
int     builtin_unset(char **argv, t_map *map);

t_bool  is_builtin(t_node *node);
t_bool  is_valid_identifier(char *key);

#endif

