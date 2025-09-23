/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:50:32 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/23 17:56:53 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include <environ.h>
# include <tokens.h>
# include <libft.h>
# include <tree.h>

typedef enum e_error_type
{
	ERROR_SYNTAX,
	ERROR_RUNTIME
}		t_error_type;

void	print_env(t_environ *head);
void	print_entry_info(t_environ *entry);

void	print_syntax_tree(t_node *node);
void	print_tokens(t_token *head, bool show_whitespace);

void	print_error(t_error_type type, const char *format, ...);
void	print_error_conditional(t_error_type type, const char *format,
			bool *condition, ...);

#endif
