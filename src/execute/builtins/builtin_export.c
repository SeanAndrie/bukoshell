/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 21:03:59 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/13 12:30:45 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <libft.h>
#include <environ.h>
#include <parsing/clean.h>
#include <execute/builtins.h>

static t_bool is_valid_key(char *key)
{
    size_t  i;

    if (!key || !*key)
        return (FALSE);
    if (!(ft_isalpha(*key) || *key == '_'))
    {
        log_error(ERROR_NONE, ERR_BASE, "export: `%s`: not a valid identifier\n", key);
        return (FALSE);
    }
    i = 1;
    while (key[i])
    {
        if (!(ft_isalnum(key[i]) || key[i] == '_'))
        {
            log_error(ERROR_NONE, ERR_BASE, "export: `%s`: not a valid identifier\n", key);
            return (FALSE);
        }
        i++;
    }
    return (TRUE);
}

static t_bool export_variable(char *arg, t_map *map)
{
    char        **pair;

    pair = get_pair(arg);
    if (!pair)
        return (FALSE);
    if (!is_valid_key(pair[0]))
    {
        free_str_arr(pair, -1);
        return (FALSE);
    }
    if (!set_entry(map, pair[0], pair[1]))
    {
        free_str_arr(pair, -1);
        return (FALSE);
    }
    free_str_arr(pair, -1);
    return (TRUE);
}

int builtin_export(char **argv, t_map *map)
{
    size_t  i;

    if (!argv[1])
    {
        print_env(map->order, TRUE);
        return (0);
    }
    i = 1;
    while (argv[i])
    {
        export_variable(argv[i], map);
        i++;
    }
    return (0);
}

