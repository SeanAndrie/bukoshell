/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:23:27 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/10 02:05:26 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEAN_H
# define CLEAN_H

# include <tree.h>
# include <stdlib.h>
# include <tokens.h>

void	free_tokens(t_token **head);
void	free_syntax_tree(t_node **root);
void	free_redirects(t_redirect **head);
void	free_str_arr(char **str_arr, int n);

#endif
