/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 15:15:08 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/22 11:53:28 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <boolean.h>
#include <debug.h>
#include <execute/builtins.h>
#include <libft.h>
#include <parsing/tree.h>

t_bool	is_builtin(t_node *node)
{
	if (!node || !node->argv)
		return (FALSE);
	if (ft_strcmp(node->argv[0], "cd") == 0)
		return (TRUE);
	else if (ft_strcmp(node->argv[0], "echo") == 0)
		return (TRUE);
	else if (ft_strcmp(node->argv[0], "export") == 0)
		return (TRUE);
	else if (ft_strcmp(node->argv[0], "unset") == 0)
		return (TRUE);
	else if (ft_strcmp(node->argv[0], "env") == 0)
		return (TRUE);
	else if (ft_strcmp(node->argv[0], "exit") == 0)
		return (TRUE);
	else if (ft_strcmp(node->argv[0], "pwd") == 0)
		return (TRUE);
	return (FALSE);
}

t_bool	is_valid_identifier(char *key)
{
	size_t	i;

	if (!key || !*key)
		return (FALSE);
	if (!(ft_isalpha(*key) || *key == '_'))
		return (FALSE);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (FALSE);
		i++;
	}
	return (TRUE);
}
