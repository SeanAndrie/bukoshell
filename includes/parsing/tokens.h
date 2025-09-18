/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 14:16:27 by sgadinga          #+#    #+#             */
/*   Updated: 2025/09/18 12:23:22 by sgadinga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

#include <stdbool.h>
#include <token_types.h>

/*
** Represents a single token in the lexical analysis phase.
**
** Each token corresponds to a meaningful unit in the command line input.
** Tokens are produced by the lexer and later consumed by the parser to
** build the syntax tree.
**
** @field lexeme  The actual string value of the token (e.g., "echo", "&&").
** @field type    The token type (word, operator, redirection, etc.).
** @field next    Pointer to the next token in the linked list.
*/
typedef struct s_token
{
	enum e_token_type	type;
	struct s_token		*next;
	char				*lexeme;
}						t_token;

/*
** Tokenizes a given command line string into a linked list of tokens.
** Handles variables, operators, grouping symbols, and words while managing
** memory and removing unnecessary whitespace tokens.
**
** @param line   Input command line string to tokenize.
** @return      A pointer to the head of the token list,
**              or NULL if memory allocation or token creation fails.
*/
t_token					*create_tokens(char *line);

/*
** Concatenates the lexemes of a linked list of tokens into a single string
** and creates a new token of the specified type from the result.
**
** @param head         Pointer to the head of the token list to concatenate.
** @param concat_type  The token type to assign to the newly created token.
** @return             A new token containing the concatenated string,
**                     or NULL if memory allocation or token creation fails.
*/
t_token					*concat_tokens(t_token *head, t_token_type concat_type);

/*
** Removes and returns a sequence of consecutive tokens from the list
** that match the specified type, starting from the head. If the head
** does not match, NULL is returned.
**
** @param head            Pointer to the head pointer of the token list.
** @param type_to_extract The token type(s) to extract (bitmask).
** @return                A pointer to the first token of the extracted
**                        sublist, or NULL if no matching token is found.
*/
t_token					*pop_token_type(t_token **head, t_token_type type);

/*
** Creates a new token from the given lexeme and type, then appends it
** to the end of the linked list of tokens. If the list is empty, the
** new token becomes the head.
**
** @param head     Pointer to the head of the token list.
** @param lexeme   The string value of the new token.
** @param type     The type of the token being added.
** @return         true if the token was successfully created and appended,
**                 false if memory allocation for the token fails.
*/
bool					append_token(t_token **head, char *lexeme,
							t_token_type type);

/*
** Removes all tokens of a specified type from a linked list and frees
** their associated memory. Updates the head pointer if the removed
** tokens include the first element(s).
**
** @param head            Pointer to the head of the token list.
** @param type_to_remove  The token type to search for and remove.
** @return                void
*/
void					remove_tokens(t_token **head,
							t_token_type type_to_remove);


/*
** Checks if a given token type matches one or more categories based
** on the provided bitmask. Returns true if all bits in the mask are
** set within the token's type.
**
** @param type            The token's type to check.
** @param category_mask   The bitmask representing one or more token categories.
** @return                true if the token type matches the category mask,
**                        false otherwise.
*/
bool					is_token_type(t_token_type type,
							unsigned int category_mask);

/*
** Generates a combined bitmask representing all token types
** present in a linked list of tokens.
**
** Iterates through the token list and ORs together the type
** of each token to produce a single mask. Useful for quickly
** checking which categories of tokens exist within the list.
**
** @param head  Pointer to the head of the token list.
** @return      A bitmask representing all token types in the list,
**              or 0 if the list is empty.
*/
unsigned int			create_token_mask(t_token *head);

#endif
