/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 20:11:25 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 22:48:43 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <libft.h>
#include <environ.h>
#include <parsing/prompts.h>
#include <execute/builtins.h>

int builtin_pwd(t_map *map)
{
    char    buffer[PATH_MAX];

    if (!getcwd_safe(buffer, sizeof(buffer), map))
    {
        perror("pwd");
        return (1);
    }
    ft_printf("%s\n", buffer);
    return (0);
}

