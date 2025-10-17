/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:51:09 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/17 13:58:57 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUKOSHELL_H
# define BUKOSHELL_H

# include <libft.h>
# include <stdio.h>
# include <debug.h>
# include <signals.h>
# include <environ.h>
# include <parsing/parsing.h>
# include <execute/execute.h>
# include <readline/history.h>
# include <readline/readline.h>

# define DEBUG_MODE TRUE

typedef struct s_shell
{
	struct s_map	*map;
	char			*line;
	struct s_token	*head;
	struct s_node	*root;
	int				status;
	char			**envp;
	unsigned int	token_mask;
	char			cwd[PATH_MAX];
}					t_shell;

t_shell				*init_shell(char **envp);
void                start_shell(t_shell *shell);

char	            *create_user(t_map *map);
char				*set_prompt(t_shell *shell, char *user);

void				free_shell(t_shell *shell, t_bool full_free);

#endif
