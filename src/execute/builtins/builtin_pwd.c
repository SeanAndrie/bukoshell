/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 20:11:25 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/24 11:35:26 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>
#include <execute/builtins.h>
#include <libft.h>
#include <parsing/prompts.h>
#include <stdio.h>

int	builtin_pwd(t_map *map)
{
	char	buffer[PATH_MAX];

	if (!getcwd_safe(buffer, sizeof(buffer), map))
	{
		ft_dprintf(STDERR_FILENO, "bukoshell: ");
		perror("pwd");
		return (1);
	}
	ft_printf("%s\n", buffer);
	return (0);
}
