/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 11:46:41 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/13 12:49:40 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <boolean.h>
#include <execute/builtins.h>

static t_bool	is_n_flag(const char *arg)
{
	size_t	i;

	if (!arg || arg[0] != '-')
		return (FALSE);
	i = 1;
	if (arg[i] == '\0')
		return (FALSE);
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

int	builtin_echo(char **argv)
{
	size_t	i;
	t_bool	no_nl;

	i = 1;
	no_nl = FALSE;
	while (argv[i] && is_n_flag(argv[i]))
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
	if (!no_nl)
		ft_printf("\n");
	return (0);
}
