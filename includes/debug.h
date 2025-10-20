/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:50:32 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/17 00:08:17 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# define ERR_BASE "bukoshell"

typedef enum e_bool t_bool;
typedef struct s_map t_map;
typedef struct s_node t_node;
typedef struct s_token t_token;
typedef struct s_redirect t_redirect;
typedef struct s_environ t_environ;

void    print_dir_list(void);
void	print_syntax_tree(t_node *node);
void	print_argv(char **argv);
void	print_redirects(t_redirect *head, int level);
void	print_entry_info(t_environ *entry);
void	print_env(t_map *map, t_bool formatted);
void	print_tokens(t_token *head, t_bool show_whitespace);

#endif
