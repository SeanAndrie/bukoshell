# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/19 17:25:04 by sgadinga          #+#    #+#              #
#    Updated: 2025/09/08 01:18:58 by sgadinga         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = bukoshell
CC = cc
CFLAGS = -Wall -Werror -Wextra -Iincludes -Ilibft/includes -g3
UNAME_S = $(shell uname -s)
UNAME_M = $(shell uname -m)

ifeq ($(UNAME_S), Darwin)
	ifeq ($(UNAME_M), arm64)
		RDL_LIB = -L/Users/carljosiah/.brew/Cellar/readline/8.3.1/lib
		RDL_INC = -I/Users/carljosiah/.brew/Cellar/readline/8.3.1/include/readline
	else ifeq ($(UNAME_M), x86_64)
		RDL_LIB = -L/opt/vagrant/embedded/lib/
		RDL_INC = -I/opt/vagrant/embedded/include/readline
	else
		$(error Unsupported Darwin Architecture: $(UNAME_M))
endif

else ifeq ($(UNAME_S), Linux)
	RDL_LIB = -L/opt/vagrant/embedded/lib/
	RDL_INC = -I/opt/vagrant/embedded/include/readline
else
	$(error Unsupported OS: $(UNAME_S))
endif

SRCS_DIR = src
OBJS_DIR = obj

SIGNALS_DIR = signals
PARSING_DIR = parsing

PARSING_VALID = $(addprefix valid/, valid_tokens.c)
PARSING_DEBUG = $(addprefix debug/, print_tokens.c print_tree.c print_error.c)
PARSING_UTILS = $(addprefix utils/, lexer_utils.c token_utils.c tree_utils.c redir_utils.c)
PARSING_FUNCS = $(addprefix $(PARSING_DIR)/, lexer.c tree.c cleanup.c tokens.c \
					$(PARSING_UTILS) $(PARSING_DEBUG) $(PARSING_VALID))

SIGNALS_FUNCS = $(addprefix $(SIGNALS_DIR)/, signals.c)

SRCS = $(addprefix $(SRCS_DIR)/, shell.c shell_utils.c $(SIGNALS_FUNCS) $(PARSING_FUNCS))
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

all: libft $(NAME)

libft:
	@make -C libft

$(NAME): $(OBJS) libft
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(RDL_LIB) $(RDL_INC) -Llibft -lft -lreadline -lncurses

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