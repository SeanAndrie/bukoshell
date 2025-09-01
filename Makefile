# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ccastro <ccastro@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/19 17:25:04 by sgadinga          #+#    #+#              #
#    Updated: 2025/09/01 18:12:22 by ccastro          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = bukoshell
CC = cc
CFLAGS = -Wall -Werror -Wextra -Iincludes -Ilibft/includes

SRCS_DIR = src
OBJS_DIR = obj

DEBUG_DIR = debug
PARSING_DIR = parsing
SIGNAL_DIR = signals

DEBUG_FUNCS = $(addprefix $(DEBUG_DIR)/, print_tokens.c)
PARSING_FUNCS = $(addprefix $(PARSING_DIR)/, lexer.c lexer_utils.c token_utils.c cleanup.c prompt.c)
SIGNALS_FUNCS = $(addprefix $(SIGNAL_DIR)/, signals.c)

SRCS = $(addprefix $(SRCS_DIR)/, bukoshell.c $(PARSING_FUNCS) $(DEBUG_FUNCS) $(SIGNALS_FUNCS))
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

all: libft $(NAME)

libft:
	@make -C libft

$(NAME): $(OBJS) libft/libft.a
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