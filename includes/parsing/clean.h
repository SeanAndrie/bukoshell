/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:23:27 by sgadinga          #+#    #+#             */
/*   Updated: 2025/10/16 23:16:07 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEAN_H
# define CLEAN_H

typedef enum e_bool			t_bool;
typedef struct s_node		t_node;
typedef struct s_token		t_token;
typedef struct s_redirect	t_redirect;

void						free_tokens(t_token **head);
void						free_syntax_tree(t_node **root);
void						free_str_arr(char **str_arr, int n);
void						free_redirects(t_redirect **head, t_bool close_fds);

#endif
