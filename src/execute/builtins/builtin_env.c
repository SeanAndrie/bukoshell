/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 21:03:27 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 23:29:10 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <boolean.h>
#include <execute/builtins.h>

int builtin_env(t_map *map)
{
    print_env(map, FALSE);
    return (0);
}

