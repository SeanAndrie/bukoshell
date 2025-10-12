/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:50:32 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/12 21:12:12 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include <environ.h>
# include <parsing/tree.h>
# include <parsing/tokens.h>

# define ERR_BASE "bukoshell"

void    print_dir_list(void);

void	print_entry_info(t_environ *entry);
void	print_env(t_environ *head, t_bool formatted);

void	print_syntax_tree(t_node *node);
void	print_tokens(t_token *head, t_bool show_whitespace);

#endif
