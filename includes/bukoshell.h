/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bukoshell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:51:09 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/01 00:52:00 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUKOSHELL_H
# define BUKOSHELL_H

# include <libft.h>
# include <stdio.h>
# include <signals.h>
# include <environ.h>
# include <parsing/parsing.h>
# include <readline/history.h>
# include <readline/readline.h>

# if defined(__linux__)
#  define PS1 "\e[1mbukoshell\e[m 🞂"
#  define PATH_MAX 4096
# elif defined(__APPLE__)
#  define PS1 "\e[1mbukoshell\e[m %"
# endif

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
int					start_shell(t_shell *shell);

char				**copy_envp(char **envp);
char				*create_identifier(t_map *map);
t_map				*realloc_map(t_map *map, char **envp);
char				*set_prompt(t_shell *shell, char *identifier);

void				free_shell(t_shell *shell, t_bool full_free);

#endif
