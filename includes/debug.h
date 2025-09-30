/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:50:32 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/27 01:40:04 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include <environ.h>
# include <libft.h>
# include <tokens.h>
# include <tree.h>

# define ERR_BASE "bukoshell"

void	print_env(t_environ *head);
void	print_entry_info(t_environ *entry);

void	print_syntax_tree(t_node *node);
void	print_tokens(t_token *head, t_bool show_whitespace);

#endif
