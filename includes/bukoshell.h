/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:51:09 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/16 17:15:56 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUKOSHELL_H
# define BUKOSHELL_H

# include <libft.h>
# include <signals.h>
# include <parsing.h>

# define PS1 "bukoshell $> "
# define PS2 "> "

# define DEBUG_MODE true

typedef struct s_shell
{
	char					*line;
	struct s_token			*head;
	struct s_node			*root;
	int						status;
}							t_shell;

typedef struct s_quote_state
{
	bool					in_single;
	bool					in_double;	
}							t_quote_state;

t_shell				*init_shell(void);
int					start_shell(t_shell *shell);
void				free_shell(t_shell *shell, bool full_free);

#endif