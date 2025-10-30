/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:51:09 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/29 15:13:15 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUKOSHELL_H
# define BUKOSHELL_H

# include <debug.h>
# include <libft.h>
# include <stdio.h>
# include <environ.h>
# include <signals.h>
# include <execute/execute.h>
# include <parsing/parsing.h>
# include <readline/history.h>
# include <readline/readline.h>

# define DEBUG_MODE FALSE 

typedef struct s_shell
{
	struct s_map	*map;
	char			*line;
	struct s_token	*head;
	struct s_node	*root;
	int				status;
	char			**envp;
	char			cwd[PATH_MAX];
}					t_shell;

void				start_shell(t_shell *shell);
char				*create_identifier(t_map *map);
char				*set_prompt(t_shell *shell, char *identifier);

void				free_shell(t_shell *shell, t_bool full_free);

#endif
