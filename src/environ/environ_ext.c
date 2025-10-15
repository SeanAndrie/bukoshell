/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_ext.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 00:03:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 00:21:39 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>
#include <parsing/clean.h>
#include <parsing/expand.h>

void	set_order(t_environ **order, t_environ *entry)
{
    t_environ *curr;

    if (!order || !*order || !entry)
        return ;
    curr = *order;
    while (curr)
    {
        if (ft_strcmp(curr->key, entry->key) == 0)
        {
            if (curr->value)
                free(curr->value);
            curr->value = ft_strdup(entry->value);
            curr->readonly = entry->readonly;
            return ;
        }
        curr = curr->next;
    }
	return ;
}

char **map_to_envp(t_map *map)
{
	size_t		i;
	t_environ	*curr;
	char		**envp;

	if (!map || !map->order)
		return (NULL);
	envp = ft_calloc(map->size + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	curr = map->order;
	while (curr)
	{
		envp[i] = ft_vstrjoin(2, "=", curr->key, curr->value);
		if (!envp[i])
		{
			free_str_arr(envp, i);
			return (NULL);
		}
		i++;
		curr = curr->next;
	}
	return (envp);
}

t_map	*realloc_map(t_map *map, char **envp)
{
	t_map	*copy;

	if (!envp)
		return (NULL);
	copy = create_map(map->capacity);
	if (!copy)
		return (NULL);
	init_environ(copy, envp);
	free_map(map);
	if (!copy)
		return (NULL);
	return (copy);
}

char	**copy_envp(char **envp)
{
	size_t	i;
	char	**copy;

	if (!envp)
		return (NULL);
	copy = malloc(sizeof(char *) * (environ_size(envp) + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			free_str_arr(copy, i);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

