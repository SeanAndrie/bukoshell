/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 21:55:59 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/13 15:04:09 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>

size_t	environ_size(char **envp)
{
	size_t	size;

	size = 0;
	while (*envp)
	{
		size++;
		envp++;
	}
	return (size);
}

size_t	hash_djb2(char *key)
{
	size_t	hash;

	hash = 5381;
	while ((unsigned char)*key)
	{
		hash = ((hash << 5) + hash) + *key;
		key++;
	}
	return (hash);
}

t_environ	*create_entry(char *key, char *value)
{
	t_environ	*entry;

	entry = ft_calloc(1, sizeof(t_environ));
	if (!entry)
		return (NULL);
	entry->key = ft_strdup(key);
	if (!entry->key)
	{
		free(entry);
		return (NULL);
	}
	if (value)
	{
		entry->value = ft_strdup(value);
		if (!entry->value)
		{
			free(entry->key);
			free(entry);
			return (NULL);
		}
	}
	return (entry);
}

void	append_entry(t_environ **head, t_environ *node)
{
	t_environ	*last;

	if (!node)
		return ;
	if (!*head)
	{
		*head = node;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = node;
}

t_map	*create_map(size_t size)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	map->capacity = size * 2;
	map->entries = ft_calloc(map->capacity, sizeof(t_environ *));
	if (!map->entries)
	{
		free(map);
		return (NULL);
	}
	map->order = NULL;
	map->size = 0;
	map->load_factor = 0.0;
	map->modified = FALSE;
	return (map);
}

