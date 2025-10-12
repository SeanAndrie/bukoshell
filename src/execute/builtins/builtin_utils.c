/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 15:15:08 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/12 20:30:26 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <execute/builtins.h>

t_bool  is_builtin(t_node *node)
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

