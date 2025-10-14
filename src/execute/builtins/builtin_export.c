/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 21:03:59 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/13 20:48:09 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <debug.h>
#include <environ.h>
#include <parsing/clean.h>
#include <execute/builtins.h>

static int export_variable(char *arg, t_map *map)
{
    char        **pair;

    pair = get_pair(arg);
    if (!pair)
        return (1);
    if (!is_valid_identifier(pair[0]))
    {
        free_str_arr(pair, -1);
        return (1);
    }
    if (!set_entry(map, pair[0], pair[1], FALSE))
    {
        free_str_arr(pair, -1);
        return (1);
    }
    free_str_arr(pair, -1);
    return (0);
}

int builtin_export(char **argv, t_map *map)
{
    size_t  i;
    int     status;

    if (!argv[1])
    {
        print_env(map->order, TRUE);
        return (0);
    }
    i = 1;
    status = 0;
    while (argv[i])
    {
        status = export_variable(argv[i], map);
        i++;
    }
    return (status);
}

