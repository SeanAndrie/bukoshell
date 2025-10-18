/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:20:02 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/18 09:00:04 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <token_types.h>

typedef enum e_bool t_bool;
typedef struct s_token t_token;

char			*process_word(char **line_ptr, t_token_type *type);
char			*process_operator(char **line_ptr, t_token_type *type);
char			*process_parameter(char **line_ptr, t_token_type *type);
char            *process_quotes_heredoc(char **line_ptr, t_token_type *type);
char			*process_grouping(char **line_ptr, t_token_type *type, t_bool suppress, t_bool heredoc);

void			mark_group_tokens(t_token **head);
t_token_type	categorize_ctrl_op(char c, t_bool is_double);
t_token_type	categorize_redirection(char c, t_bool is_double);
t_bool			handle_concatenation(t_token **head, t_token_type concat_type);

#endif
