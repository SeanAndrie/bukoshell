/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 11:46:41 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/12 16:48:21 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <boolean.h>
#include <execute/builtins.h>

static t_bool has_newline(char *arg)
{
    size_t  i;

    i = 1;
    if (arg[i] == '-')
    {
        if (!arg[i + 1])
            return (TRUE);
    }
    while (arg[i] && arg[i] == 'n')
        i++;
    if (!arg[i])
        return (FALSE);
    return (TRUE);
}

int builtin_echo(char **argv)
{
    size_t  i;
    t_bool  no_nl;

    i = 1;
    no_nl = FALSE;
    while (argv[i] && !has_newline(argv[i]))
    {
        no_nl = TRUE;
        i++;
    }
    while (argv[i])
    {
        ft_printf("%s", argv[i]);
        if (argv[i + 1])
            ft_printf(" ");
        i++;
    }
    ft_printf("\n");
    if (!no_nl)
        ft_printf("\n");
    return (0);
}

