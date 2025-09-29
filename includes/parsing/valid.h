/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:24:24 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/28 16:31:21 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALID_H
# define VALID_H

# include <debug.h>
# include <tokens.h>

void    consume(t_token **curr);
t_bool	parse_command_list(t_token **curr, t_bool in_group);
t_bool	validate_tokens(t_token *head);
t_bool    validate_arithmetic(t_token *start, t_token *end);

#endif
