/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:49:50 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 22:48:08 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <libft.h>
#include <debug.h>
#include <execute/builtins.h>

int builtin_exit(char **argv)
{
    long    value;
    char    *endptr;

    ft_printf("exit\n");
    if (!argv[1])
        exit(EXIT_SUCCESS);
    errno = 0;
    value = ft_strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || errno == EINVAL || errno == ERANGE)
    {
        log_error(ERROR_NONE, ERR_BASE, "exit: %s: numeric argument required\n", argv[1]);
        exit(255);
    }
    if (argv[2])
    {
        log_error(ERROR_NONE, ERR_BASE, "exit: too many arguments\n");
        return (1);
    }
    exit((unsigned char)value);
}

