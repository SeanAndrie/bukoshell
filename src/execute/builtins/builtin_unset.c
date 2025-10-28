/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:55:33 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/26 22:55:05 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <debug.h>
#include <environ.h>
#include <execute/builtins.h>
#include <libft.h>

static int	unset_variable(t_map *map, char *key)
{
	t_environ	*entry;

	if (!is_valid_identifier(key))
	{
		log_error(ERROR_NONE, ERR_BASE,
			"export: '%s': not a valid identifier\n", key);
		return (1);
	}
	delete_entry(map, key);
	entry = search_entry(map, key);
	if (entry && !entry->readonly)
		return (1);
	return (0);
}

int	builtin_unset(char **argv, t_map *map)
{
	size_t	i;
	int		status;

	if (!argv[1] || !map)
		return (0);
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (unset_variable(map, argv[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}
