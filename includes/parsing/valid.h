/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:24:24 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/10 02:21:48 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALID_H
# define VALID_H

# include <debug.h>
# include <tokens.h>
# include <stdbool.h>

bool	are_valid_tokens(t_token *head);
bool	is_valid_operator(t_token *curr);
bool	is_valid_redirect(t_token *curr);
bool	is_valid_parameter(t_token *curr);
bool	is_valid_grouping(t_token *prev, t_token *curr, int *depth);

#endif
