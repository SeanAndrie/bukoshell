/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:28:50 by sgadinga          #+#    #+#             */
/*   Updated: 2025/08/20 03:29:49 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

t_token	*create_token(char *lexeme, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->lexeme = ft_strdup(lexeme);
	if (!token->lexeme)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

void	append_token(t_token **head, char *lexeme, t_token_type type)
{
	t_token	*token;
	t_token	*last_token;

	token = create_token(lexeme, type);
	if (!token)
		return ;
	if (!*head)
	{
		*head = token;
		return ;
	}
	last_token = *head;
	while (last_token->next)
		last_token = last_token->next;
	last_token->next = token;
}

void	free_tokens(t_token **head)
{
	t_token	*next;

	while (*head)
	{
		next = (*head)->next;
		free((*head)->lexeme);
		free(*head);
		*head = next;
	}
}

void    print_token_type(t_token_type type)
{
    if (type == T_WORD)
        ft_printf("WORD");
    else if (type == T_WORD_SQUOTE)
        ft_printf("S_QUOTE_WORD");
    else if (type == T_WORD_DQUOTE)
        ft_printf("D_QUOTE_WORD");
    else if (type == T_PIPE)
        ft_printf("PIPE");
    else if (type == T_REDIR_IN)
        ft_printf("REDIR_IN");
    else if (type == T_REDIR_OUT)
        ft_printf("REDIR_OUT");
    else if (type == T_REDIR_APPEND)
        ft_printf("REDIR_APPEND");
    else if (type == T_HEREDOC)
        ft_printf("HEREDOC");
    else if(type == T_AND)
        ft_printf("AND");
    else if (type == T_OR)
        ft_printf("OR");
    else if (type == T_SEMICOLON)
        ft_printf("SEMICOLON");
    else if (type == T_EOF)
        ft_printf("EOF");
    
}

void	print_tokens(t_token *head)
{
	while (head)
	{
		ft_printf("[%s : ", head->lexeme);
        print_token_type(head->type);
        ft_printf("]");
		head = head->next;
		if (head)
			ft_printf(" -> ");
	}
	ft_printf("\n");
}
