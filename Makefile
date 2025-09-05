# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgadinga <sgadinga@student.42abudhabi.ae>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/19 17:25:04 by sgadinga          #+#    #+#              #
#    Updated: 2025/09/05 02:42:50 by sgadinga         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = bukoshell
CC = cc
CFLAGS = -Wall -Werror -Wextra -Iincludes -Ilibft/includes -g3

SRCS_DIR = src
OBJS_DIR = obj

SIGNALS_DIR = signals
PARSING_DIR = parsing

PARSING_DEBUG = $(addprefix debug/, print_tokens.c print_tree.c)
PARSING_UTILS = $(addprefix utils/, lexer_utils.c token_utils.c tree_utils.c)
PARSING_FUNCS = $(addprefix $(PARSING_DIR)/, lexer.c tree.c cleanup.c $(PARSING_UTILS) $(PARSING_DEBUG))

SIGNALS_FUNCS = $(addprefix $(SIGNALS_DIR)/, signals.c)

SRCS = $(addprefix $(SRCS_DIR)/, shell.c shell_utils.c $(SIGNALS_FUNCS) $(PARSING_FUNCS))
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

all: libft $(NAME)

libft:
	@make -C libft

$(NAME): $(OBJS) libft
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -Llibft -lft -lreadline -lncurses

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)
	@make -C libft clean

fclean: clean
	@rm -f $(NAME)
	@make -C libft fclean

re: fclean all

.PHONY: re fclean clean libft all
