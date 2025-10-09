/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:23:27 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/09 17:33:18 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEAN_H
# define CLEAN_H

# include <stdlib.h>
# include <parsing/tokens.h>
# include <parsing/tree.h>

void	free_tokens(t_token **head);
void	free_syntax_tree(t_node **root);
void	free_redirects(t_redirect **head);
void	free_str_arr(char **str_arr, int n);

#endif
