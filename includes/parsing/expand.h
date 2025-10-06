/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 01:48:04 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/05 22:04:10 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include <environ.h>
# include <parsing/tokens.h>

typedef struct s_glob
{
    char    *s;
    char    *p;
    char    *star;
    char    *s_back;
}               t_glob;

void    apply_param_expansion(t_token *token, t_map *map);
void    apply_wildcard_expansion(t_token **head, t_token *token);

#endif
