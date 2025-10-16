/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:55:33 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 22:49:51 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <debug.h>
#include <environ.h>
#include <execute/builtins.h>

static int unset_variable(char *key, t_map *map)
{
    if (!is_valid_identifier(key))
    {
        log_error(ERROR_NONE, ERR_BASE, "unset: '%s': not a valid identifier\n", key);
        return (1);
    }
    if (!delete_entry(map, key))
        return (1);
    return (0);
}

int builtin_unset(char **argv, t_map *map)
{
    size_t      i;
    int         status;
    
    if (!argv[1] || !map)
        return (0);
    i = 1;
    status = 0;
    while (argv[i])
    {
        status = unset_variable(argv[i], map);
        i++;
    }
    return (status);
}

