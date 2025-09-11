/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:20:02 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/11 17:42:10 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <tokens.h>

typedef struct s_quote_state
{
    bool                    in_single;
    bool                    in_double;
}                           t_quote_state;

typedef struct s_lexer_ctx
{
    bool                    dangling_op;
    int                     subsh_depth;
    struct s_quote_state    quote_state;
}                           t_lexer_ctx;


char			*process_word(char **line_ptr, t_token_type *type);
char			*process_parameter(char **line_ptr, t_token_type *type);
char			*process_grouping(char **line_ptr, t_token_type *type);
char			*process_operator(char **line_ptr, t_token_type *type);

bool			handle_concatenation(t_token **head);
t_token_type	categorize_ctrl_op(char c, bool is_double);
t_token_type	categorize_redirection(char c, bool is_double);

#endif
