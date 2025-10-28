/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 16:49:50 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/29 02:23:19 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>
#include <debug.h>
#include <errno.h>
#include <execute/builtins.h>
#include <execute/exec.h>
#include <libft.h>

static void	exit_with_last_status(t_shell_ctx *ctx)
{
	int	status;

	status = ctx->shell->status;
	if (status < 0)
		status = 0;
	free_shell(ctx->shell, TRUE);
	exit(status);
}

int	builtin_exit(char **argv, t_shell_ctx *ctx)
{
	size_t	i;
	long	value;
	char	*endptr;

	ft_printf("exit\n");
	if (!argv[1])
		return (exit_with_last_status(ctx), 0);
	errno = 0;
	i = 0;
	while (argv[++i])
	{
		value = ft_strtol(argv[1], &endptr, 10);
		if (*endptr != '\0' || errno == ERANGE || errno == EINVAL)
		{
			ft_dprintf(STDERR_FILENO, NUMERIC_ARG_ERR, ERR_BASE, argv[i]);
			free_shell(ctx->shell, TRUE);
			exit(255);
		}
		if (i == 2)
			return (log_error(ERROR_NONE, ERR_BASE,
					"exit: too many arguments\n"), 1);
	}
	free_shell(ctx->shell, TRUE);
	exit((unsigned char)value);
}
