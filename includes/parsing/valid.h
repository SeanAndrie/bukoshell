/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:24:24 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/17 12:50:48 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALID_H
# define VALID_H

typedef struct s_token t_token;

void	consume(t_token **curr);
t_bool	check_arithmetic(t_token *head);
t_bool	validate_tokens(t_token *head);
t_bool 	is_separated_command(t_token *token);
t_bool	parse_command_list(t_token **curr, int *depth);

#endif
