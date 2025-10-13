/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 21:57:04 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/13 14:01:54 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRON_H
# define ENVIRON_H

# include <boolean.h>
# include <libft.h>

# define LOAD_THRESHOLD 0.75

typedef struct s_environ
{
	char				*key;
	char				*value;
	struct s_environ	*next;
}						t_environ;

typedef struct s_map
{
	struct s_environ	**entries;
	struct s_environ	*order;
	double				load_factor;
	size_t				capacity;
	size_t				size;
}						t_map;

size_t					hash_djb2(char *key);
char	                **get_pair(char *env);
size_t					environ_size(char **envp);
void					init_environ(t_map *map, char **envp);
void                    update_order(t_environ **order, t_environ *entry);

t_map					*create_map(size_t size);
t_environ				*create_entry(char *key, char *value);
void					append_entry(t_environ **head, t_environ *node);

t_bool					delete_entry(t_map *map, char *key);
t_environ				*search_entry(t_map *map, char *key);
t_bool                  set_entry(t_map *map, char *key, char *value);
t_bool					insert_entry(t_map *map, char *key, char *value);

void					free_map(t_map *map);
#endif
