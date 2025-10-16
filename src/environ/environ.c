/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 09:47:34 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 22:40:13 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <environ.h>
#include <parsing/clean.h>

void	free_entries(t_environ **entry)
{
	t_environ	*next;

	while (*entry)
	{
		next = (*entry)->next;
		free((*entry)->key);
		if ((*entry)->value)
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

char	**get_pair(char *env)
{
	char	**pair;
	char	*equal;

	pair = malloc(sizeof(char *) * 3);
	if (!pair)
		return (NULL);
	equal = ft_strchr(env, '=');
	if (equal)
	{
		pair[0] = ft_substr(env, 0, equal - env);
		pair[1] = ft_strdup(equal + 1);
	}
	else
	{
		pair[0] = ft_strdup(env);
		pair[1] = ft_strdup("");
	}
	if (!pair[0] && !pair[1])
	{
		free_str_arr(pair, 2);
		return (NULL);
	}
	pair[2] = NULL;
	return (pair);
}

void	init_shell_variables(t_map *map)
{
	t_environ	*shlvl;
	char		*level;
	t_environ	*status;

	set_entry(map, "?", "0");
	status = search_entry(map, "?");
	if (status)
		status->readonly = TRUE;
	set_order(&map->order, status);
	set_entry(map, "OLDPWD", "");
	shlvl = search_entry(map, "SHLVL");
	if (!shlvl)
	{
		set_entry(map, "SHLVL", "0");
		shlvl = search_entry(map, "SHLVL");
	}
	level = ft_itoa(ft_atoi(shlvl->value) + 1);
	set_entry(map, "SHLVL", level);
	free(level);
}

void	init_environ(t_map *map, char **envp)
{
	char	**pair;

	while (*envp)
	{
		if (**envp)
		{
			pair = get_pair(*envp);
			if (pair)
			{
				if (!insert_entry(map, pair[0], pair[1]))
				{
					free_str_arr(pair, -1);
					return ;
				}
				free_str_arr(pair, -1);
			}
		}
		envp++;
	}
}
