/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 14:01:14 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/29 18:03:53 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bukoshell.h>

static void	print_token_operator(t_token_type type)
{
	if (type == T_PIPE)
		ft_printf("PIPE");
	else if (type == T_OR)
		ft_printf("OR");
	else if (type == T_BACKGROUND)
		ft_printf("BACKGROUND");
	else if (type == T_AND)
		ft_printf("AND");
}

static void	print_token_redirection(t_token_type type)
{
	if (type == T_REDIR_IN)
		ft_printf("REDIR_IN");
	else if (type == T_REDIR_OUT)
		ft_printf("REDIR_OUT");
	else if (type == T_REDIR_APPEND)
		ft_printf("REDIR_APPEND");
	else if (type == T_HEREDOC)
		ft_printf("HEREDOC");
}

static void	print_token_group(t_token_type type)
{
	if (type == T_LPAREN)
		ft_printf("LPAREN");
	else if (type == T_RPAREN)
		ft_printf("RPAREN");
	else if (type == T_LBRACE)
		ft_printf("LBRACE");
	else if (type == T_RBRACE)
		ft_printf("RBRACE");
}

static void	print_token_word(t_token_type type)
{
	if (type == T_WORD_SQUOTE)
		ft_printf("WORD_SQUOTE");
	else if (type == T_WORD_DQUOTE)
		ft_printf("WORD_DQUOTE");
	else if (type == T_WORD)
		ft_printf("WORD");
	else
		ft_printf("[WHITESPACE");
}

void	print_tokens(t_token *head)
{
	if (!head)
		return ;
	while (head)
	{
		if (!is_token_type(head->type, TOKEN_WHITESPACE))
			ft_printf("[%s : ", head->lexeme);
		if (is_token_type(head->type, TOKEN_CTRL_OP))
			print_token_operator(head->type);
		else if (is_token_type(head->type, TOKEN_GROUP))
			print_token_group(head->type);
		else if (is_token_type(head->type, TOKEN_REDIR_OP))
			print_token_redirection(head->type);
		else
			print_token_word(head->type);
		ft_printf("]");
		head = head->next;
		if (head)
			ft_printf(" -> ");
	}
	ft_printf("\n");
}
