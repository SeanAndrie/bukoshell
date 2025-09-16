
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:15:58 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/10 02:05:37 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include <tree.h>
# include <libft.h>
# include <tokens.h>

typedef enum e_error_type
{
	ERROR_SYNTAX,
	ERROR_RUNTIME
}		t_error_type;

void	print_syntax_tree(t_node *node);
void	print_tokens(t_token *head, bool show_whitespace);
void	print_error(t_error_type type, const char *format, ...);
void	print_error_conditional(t_error_type type, const char *format, bool *condition, ...);

#endif
