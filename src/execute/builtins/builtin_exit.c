/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:49:50 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/27 14:12:13 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <libft.h>
#include <debug.h>
#include <bukoshell.h>
#include <execute/exec.h>
#include <execute/builtins.h>

static void exit_with_last_status(t_shell_ctx *ctx)
{
    int status;
    
    status = ctx->shell->status;
    if (status < 0)
        status = 0;
    free_shell(ctx->shell, TRUE);
    exit(status);
}

int builtin_exit(char **argv, t_shell_ctx *ctx)
{
    long    value;
    char    *endptr;

    ft_printf("exit\n");
    if (!argv[1])
        return (exit_with_last_status(ctx), 0); 
    if (argv[2])
    {
        log_error(ERROR_NONE, ERR_BASE, "exit: too many arguments\n");
        return (1);
    }
    errno = 0;
    value = ft_strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || errno == ERANGE || errno == EINVAL)
    {
        log_error(ERROR_NONE, ERR_BASE,
                  "exit: %s: numeric argument required\n", argv[1]);
        free_shell(ctx->shell, TRUE);
        exit(255);
    }
    free_shell(ctx->shell, TRUE);
    exit((unsigned char)value);
}
