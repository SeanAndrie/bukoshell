/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 01:48:04 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/23 11:37:44 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

typedef enum e_bool t_bool;
typedef struct s_map t_map;
typedef struct s_token t_token;

typedef struct s_glob
{
    char    *s;
    char    *p;
    char    *star;
    char    *s_back;
}               t_glob;

t_bool  apply_wildcard_expansion(t_token **head, t_token *token);
void	apply_param_expansion(t_token *token, t_map *map, t_bool heredoc);
void    apply_tilde_expansion(t_token *token, t_map *map);

void    quick_sort(char **arr, int low, int high);
t_bool	is_matching_pattern(char *pattern, char *str);

#endif
