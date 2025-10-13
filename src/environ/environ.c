/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 09:47:34 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/23 19:47:00by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>
#include <execute/builtins.h>
#include <parsing/clean.h>

static void	free_entries(t_environ **entry)
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

void update_order(t_environ **order, t_environ *entry)
{
    t_environ *curr;

    if (!order || !entry)
        return ;
    curr = *order;
    while (curr)
    {
        if (ft_strcmp(curr->key, entry->key) == 0)
        {
            if (curr->value)
                free(curr->value);
            curr->value = ft_strdup(entry->value);
            return ;
        }
        curr = curr->next;
    }
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

void	init_environ(t_map *map, char **envp)
{
	char	    **pair;

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
