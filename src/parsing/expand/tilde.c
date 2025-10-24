/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 20:32:27 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/24 13:37:50 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>
#include <libft.h>
#include <parsing/clean.h>
#include <parsing/expand.h>
#include <parsing/tokens.h>

static char	*expand_tilde_path(char **paths, char *home_path)
{
	size_t	i;
	char	*temp;
	char	*join;

	join = ft_strdup(home_path);
	if (!join)
		return (NULL);
	i = 1;
	while (paths[i])
	{
		temp = ft_vstrjoin(2, "/", join, paths[i]);
		if (!temp)
		{
			free(join);
			return (NULL);
		}
		free(join);
		join = temp;
		i++;
	}
	return (join);
}

static char	*get_tilde_base_path(char *lexeme, t_map *map)
{
	t_environ	*entry;

	if (!lexeme || lexeme[0] != '~')
		return (NULL);
	if (lexeme[1] == '\0' || lexeme[1] == '/')
		entry = search_entry(map, "HOME");
	else if (lexeme[1] == '+')
		entry = search_entry(map, "PWD");
	else if (lexeme[1] == '-')
		entry = search_entry(map, "OLDPWD");
	else
		return (NULL);
	if (!entry || !entry->value)
		return (NULL);
	return (entry->value);
}

void	apply_tilde_expansion(t_token *token, t_map *map)
{
	char	*base_path;
	char	**paths;
	char	*temp;

	if (!token->expandable || !token->lexeme || token->lexeme[0] != '~')
		return ;
	base_path = get_tilde_base_path(token->lexeme, map);
	if (!base_path)
		return ;
	paths = ft_split(token->lexeme, '/');
	if (!paths)
		return ;
	temp = ft_strdup(token->lexeme);
	if (!temp)
	{
		free_str_arr(paths, -1);
		return ;
	}
	free(token->lexeme);
	token->lexeme = expand_tilde_path(paths, base_path);
	if (!token->lexeme)
		token->lexeme = temp;
	else
		free(temp);
	free_str_arr(paths, -1);
}
