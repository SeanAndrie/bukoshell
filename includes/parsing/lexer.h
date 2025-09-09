/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:20:02 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/10 02:10:33 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <tokens.h>

char			*process_word(char **line_ptr, t_token_type *type);
char			*process_quotes(char **line_ptr, t_token_type *type);
char			*process_grouping(char **line_ptr, t_token_type *type);
char			*process_operator(char **line_ptr, t_token_type *type);
char			*process_parameter(char **line_ptr, t_token_type *type);

bool			handle_concatenation(t_token **head);
t_token_type	categorize_ctrl_op(char c, bool is_double);
t_token_type	categorize_redirection(char c, bool is_double);

#endif
