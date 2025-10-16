/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 00:12:35 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 17:52:09 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

static t_bool	is_home_directory(t_map *map, char *cwd)
{
	t_environ	*home;

	if (!map || !cwd)
		return (FALSE);
	home = search_entry(map, "HOME");
	if (!home)
		return (FALSE);
	return ((ft_strcmp(home->value, cwd) == 0));
}

static char	*create_cwd(t_shell *shell)
{
	size_t	i;
	char	*cwd;
	char	**split;

	if (!getcwd(shell->cwd, sizeof(shell->cwd)))
		return (NULL);
	if (is_home_directory(shell->map, shell->cwd))
		return (ft_strdup("~"));
	split = ft_split(shell->cwd, '/');
	if (!split)
		return (NULL);
	if (!split[0])
	{
		free_str_arr(split, -1);
		return (ft_strdup("/"));
	}
	i = 0;
	while (split[i + 1])
		i++;
	cwd = ft_strdup(split[i]);
	free_str_arr(split, -1);
	if (!cwd)
		return (NULL);
	return (cwd);
}

char	*set_prompt(t_shell *shell, char *user)
{
	char	*cwd;
	char	*join;
	char	*prompt;
	char	**base_split;

	cwd = create_cwd(shell);
	if (!cwd)
		return (ft_strdup(PS1));
	join = ft_vstrjoin(2, " ", user, cwd);
	base_split = ft_split(PS1, ' ');
	if (!base_split)
		return (NULL);
	prompt = ft_vstrjoin(6, NULL, base_split[0], " [", join, "] ",
			base_split[1], " ");
	free_str_arr(base_split, 2);
	free(join);
	free(cwd);
	if (!prompt)
		return (ft_strdup(PS1));
	return (prompt);
}

char	*create_user(t_map *map)
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

t_bool	check_arithmetic(t_token *head, unsigned int mask)
{
	if ((mask & TOKEN_ARITH) && is_arithmetic(head))
	{
		log_error(ERROR_SYNTAX, ERR_BASE,
			"arithmetic expressions are not supported\n");
		return (FALSE);
	}
	return (TRUE);
}
