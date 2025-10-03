/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:29:15 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/25 00:42:19 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

char	*create_identifier(t_map *map)
{
	t_environ	*user;

	if (!map)
		return (NULL);
	user = search_entry(map, "USER");
	if (!user)
	{
		user = search_entry(map, "LOGNAME");
		if (!user)
			return (NULL);
	}
	return (ft_strdup(user->value));
}

static char	*construct_prompt(char *base_name, char *base_indicator, char *cwd,
		char *identifier)
{
	char	*temp;
	char	*prompt;

	temp = ft_vstrjoin(7, NULL, base_name, " [", identifier, " ", cwd, "] ",
			base_indicator);
	if (!temp)
		return (NULL);
	prompt = ft_strjoin(temp, " ");
	free(temp);
	if (!prompt)
		return (NULL);
	return (prompt);
}

char	*set_prompt(t_shell *shell, char *identifier)
{
	size_t	i;
	char	*prompt;
	char	**cwd_split;
	char	**base_split;

	if (!getcwd(shell->cwd, sizeof(shell->cwd)))
		return (NULL);
	cwd_split = ft_split(shell->cwd, '/');
	if (!cwd_split)
		return (NULL);
	i = 0;
	while (cwd_split[i])
		i++;
	base_split = ft_split(PS1, ' ');
	if (!base_split)
    {
        free_str_arr(cwd_split, -1);
        return (NULL);
    }
	prompt = construct_prompt(base_split[0], base_split[1], cwd_split[i - 1],
			identifier);
	free_str_arr(cwd_split, i);
	free_str_arr(base_split, 2);
	return (prompt);
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
