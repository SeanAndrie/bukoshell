/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:51:09 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/07 19:04:50 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUKOSHELL_H
# define BUKOSHELL_H

# include <libft.h>
# include <parsing.h>
# include <signals.h>

# define ROOT_LEVEL 0

typedef struct s_shell
{
	char			*line;
	struct s_token	*head;
	struct s_node	*root;
	int				status;
	unsigned int	token_mask;
}					t_shell;

void				free_shell(t_shell *shell);

t_shell				*init_shell(void);
int					start_shell(t_shell *shell);

#endif