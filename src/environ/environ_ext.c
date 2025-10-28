/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_ext.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 00:03:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/24 00:50:15 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>
#include <libft.h>
#include <parsing/clean.h>
#include <parsing/expand.h>

char	*getcwd_safe(char *buffer, size_t size, t_map *map)
{
	t_environ	*pwd;
	char		*res;

	res = getcwd(buffer, size);
	if (res)
		return (res);
	if (!map)
		return (NULL);
	pwd = search_entry(map, "PWD");
	if (!pwd || !pwd->value)
		return (NULL);
	if (buffer && size)
	{
		if (ft_strlen(pwd->value) >= size)
			return (NULL);
		ft_strlcpy(buffer, pwd->value, size);
	}
	return (ft_strdup(pwd->value));
}

void	set_order(t_environ **order, t_environ *entry)
{
	t_environ	*curr;

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

char	**map_to_envp(t_map *map)
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

t_map	*realloc_map(t_map *map)
{
	t_map		*copy;
	t_environ	*curr;
	t_environ	*entry;

	if (!map)
		return (NULL);
	copy = create_map(map->capacity);
	if (!copy)
		return (NULL);
	curr = map->order;
	while (curr)
	{
		if (!insert_entry(copy, curr->key, curr->value))
		{
			free_map(copy);
			return (NULL);
		}
		entry = search_entry(copy, curr->key);
		if (entry)
			entry->readonly = curr->readonly;
		set_order(&copy->order, curr);
		curr = curr->next;
	}
	free_map(map);
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
