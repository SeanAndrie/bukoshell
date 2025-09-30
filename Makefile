# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/19 17:25:04 by sgadinga          #+#    #+#              #
#    Updated: 2025/09/30 00:55:11 by sgadinga         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = bukoshell
CC = cc
CFLAGS = -Wall -Werror -Wextra -Iincludes -Iincludes/parsing -Ilibft/includes -g3

UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_S), Darwin)
	ifeq ($(UNAME_M), arm64)
		RDL_LIB := -L/Users/carljosiah/.brew/Cellar/readline/8.3.1/lib
		RDL_INC := -I/Users/carljosiah/.brew/Cellar/readline/8.3.1/include/readline
	else ifeq ($(UNAME_M), x86_64)
		RDL_LIB := -L/opt/vagrant/embedded/lib/
		RDL_INC := -I/opt/vagrant/embedded/include/readline
	else
		$(error Unsupported Darwin Architecture: $(UNAME_M))
endif
else ifeq ($(UNAME_S), Linux)
	RDL_LIB := -L/opt/vagrant/embedded/lib/
	RDL_INC := -I/opt/vagrant/embedded/include/readline
else
	$(error Unsupported OS: $(UNAME_S))
endif

SRCS_DIR := src
OBJS_DIR := obj

PARSING_DIR := parsing
SIGNALS_DIR := signals
ENVIRON_DIR := environ
DEBUG_DIR := debug
SHELL_DIR := shell

PARSING_MODULES := \
	clean/clean.c \
	lexer/lexer.c lexer/lexer_utils.c \
	tokens/tokens.c tokens/token_init.c tokens/token_utils.c \
	tree/tree.c tree/tree_utils.c tree/redir_init.c tree/redir_utils.c \
	valid/validate.c valid/valid_tokens.c \
	expand/expand.c

DEBUG_SRCS := $(addprefix $(DEBUG_DIR)/, print_tokens.c print_tree.c print_env.c)
PARSING_SRCS := $(addprefix $(PARSING_DIR)/,$(PARSING_MODULES))
SIGNALS_SRCS := $(SIGNALS_DIR)/signals.c
ENVIRON_SRCS := $(addprefix $(ENVIRON_DIR)/, environ.c environ_init.c environ_utils.c) 
MAIN_SRCS := $(addprefix $(SHELL_DIR)/,shell.c shell_init.c shell_utils.c)

SRCS := $(addprefix $(SRCS_DIR)/, $(MAIN_SRCS) $(SIGNALS_SRCS) $(PARSING_SRCS) $(DEBUG_SRCS) $(ENVIRON_SRCS))
OBJS := $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

all: libft $(NAME)

libft:
	@make -C libft

$(NAME): $(OBJS) libft
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(RDL_LIB) $(RDL_INC) -Llibft -lft -lreadline -lncurses

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

.PHONY: all re clean fclean libft
