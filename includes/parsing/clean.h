/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:23:27 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 00:20:20 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEAN_H
# define CLEAN_H

# include <parsing/tree.h>
# include <parsing/tokens.h>

void	free_tokens(t_token **head);
void	free_syntax_tree(t_node **root);
void	free_str_arr(char **str_arr, int n);
void	free_redirects(t_redirect **head, t_bool close_fds);

#endif
