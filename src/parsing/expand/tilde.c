/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 20:32:27 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/17 20:33:54 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <environ.h>
#include <parsing/clean.h>
#include <parsing/tokens.h>
#include <parsing/expand.h>

static char *expand_tilde_path(char **paths, char *home_path)
{
    size_t  i;
    char    *temp;
    char    *join;

    join = ft_strdup(home_path);
    if (!join)
        return (NULL);
    i = 1;
    while (paths[i])
    {
        temp = ft_vstrjoin(2, "/", join, paths[i]);
        if (!temp)
        {
            free(join);
            return (NULL);
        }
        join = temp;
        i++;
    }
    return (join);
}

void    apply_tilde_expansion(t_token *token, t_map *map)
{
    t_environ   *home;
    char        *temp;
    char        **paths;

    home = search_entry(map, "HOME");
    if (!home)
        return ;
    temp = ft_strdup(token->lexeme);
    if (!temp)
        return ;
    free(token->lexeme);
    paths = ft_split(temp, '/');
    if (!paths)
        return ;
    if (!paths[1])
    {
        token->lexeme = ft_strdup(home->value);
        if (!token->lexeme)
            token->lexeme = temp;
        return ;
    }
    token->lexeme = expand_tilde_path(paths, home->value);
    if (!token->lexeme)
        token->lexeme = temp;
    free_str_arr(paths, -1);
}

