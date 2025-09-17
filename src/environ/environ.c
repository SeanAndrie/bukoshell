/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 09:47:34 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/17 17:55:23 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>
#include <parsing/clean.h>

static void	free_entries(t_environ **entry)
{
	t_environ	*next;

	while (*entry)
	{
		next = (*entry)->next;
		free((*entry)->key);
		free((*entry)->value);
		free(*entry);
		*entry = next;
	}
}

void	free_map(t_map *map)
{
	size_t	i;

	if (!map)
		return ;
	if (map->entries)
	{
		i = 0;
		while (i < map->capacity)
		{
			free_entries(&map->entries[i]);
			i++;
		}
		free(map->entries);
	}
	free_entries(&map->order);
	free(map->order);
	free(map);
}

void	init_environ(t_map *map, char **envp)
{
	char	*value;
	char	**pair;

	if (!envp || !map)
		return ;	
	while (*envp)
	{
		pair = ft_split(*envp, '=');
		if (!pair)
			break ;
		value = pair[1];
		if (!insert_entry(map, pair[0], value))
		{
			free_str_arr(pair, -1);
			break ;
		}
		free_str_arr(pair, -1);
		envp++;
	}
}
