/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:51:09 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/17 18:39:23 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUKOSHELL_H
# define BUKOSHELL_H

# include <libft.h>
# include <environ.h>
# include <signals.h>
# include <parsing.h>

# define PS1 "\e[1mbukoshell\e[m 🞂"
# define PS2 "> "

# define PATH_MAX 4096
# define DEBUG_MODE true 

typedef struct s_shell
{
	struct s_map			*map;
	char					*line;
	struct s_token			*head;
	struct s_node			*root;
	int						status;
	char					**envp;
	char					cwd[PATH_MAX];
}							t_shell;

t_shell				*init_shell(char **envp);
int					start_shell(t_shell *shell);

char    			**copy_envp(char **envp);
char				*create_identifier(t_map *map);
char				*set_cwd_prompt(t_shell *shell, char *identifier);
t_map				*realloc_map(t_map *map, char **envp);

void				free_shell(t_shell *shell, bool full_free);

#endif