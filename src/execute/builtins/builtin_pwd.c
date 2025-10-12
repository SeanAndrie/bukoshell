/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 20:11:25 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/12 21:16:03 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <execute/builtins.h>

int builtin_pwd(void)
{
    char    buffer[PATH_MAX];

    if (!getcwd(buffer, sizeof(buffer)))
    {
        perror("pwd");
        return (1);
    }
    ft_printf("%s\n", buffer);
    return (0);
}

