/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 11:49:40 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 23:28:31 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

typedef enum e_bool t_bool;
typedef struct s_map t_map;
typedef struct s_node t_node;

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

