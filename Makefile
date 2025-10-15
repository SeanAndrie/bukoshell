# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgadinga <sgadinga@student.42.abudhabi.ae> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/19 17:25:04 by sgadinga          #+#    #+#              #
#    Updated: 2025/10/16 00:14:30 by sgadinga         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := bukoshell
CC := cc
CFLAGS := -Wall -Werror -Wextra -Iincludes -Iincludes/parsing -Ilibft/includes -g3

RDL_INC := -I/opt/vagrant/embedded/include/readline
RDL_LIB := -L/opt/vagrant/embedded/lib/

CFLAGS += $(RDL_INC)

LIBS := -Llibft -lft $(RDL_LIB) -lreadline -lhistory -lncurses

SRCS_DIR := src
OBJS_DIR := obj

PARSING_DIR := parsing
EXECUTE_DIR := execute
SIGNALS_DIR := signals
ENVIRON_DIR := environ
DEBUG_DIR := debug
SHELL_DIR := shell

PARSING_MODULES := \
    clean/clean.c \
    lexer/lexer.c lexer/lexer_utils.c \
    tokens/tokens.c tokens/token_init.c tokens/token_utils.c \
    tree/tree.c tree/tree_utils.c tree/redir_init.c tree/redir_heredoc.c tree/redir_utils.c \
    valid/validate.c valid/valid_tokens.c \
	expand/parameter.c expand/wildcard.c expand/wildcard_utils.c

EXECUTE_MODULES := \
	exec/exec.c exec/exec_simple.c exec/exec_utils.c \
	builtins/builtin_echo.c builtins/builtin_exit.c builtins/builtin_utils.c \
	builtins/builtin_pwd.c builtins/builtin_export.c builtins/builtin_cd.c \
	builtins/builtin_unset.c builtins/builtin_env.c

DEBUG_SRCS := $(addprefix $(DEBUG_DIR)/, print_tokens.c print_tree.c print_env.c print_dir_list.c)
PARSING_SRCS := $(addprefix $(PARSING_DIR)/, $(PARSING_MODULES))
EXECUTE_SRCS := $(addprefix $(EXECUTE_DIR)/, $(EXECUTE_MODULES))
SIGNALS_SRCS := $(SIGNALS_DIR)/signals.c
ENVIRON_SRCS := $(addprefix $(ENVIRON_DIR)/, environ.c environ_init.c environ_ext.c environ_utils.c)
MAIN_SRCS := $(addprefix $(SHELL_DIR)/, shell.c shell_init.c shell_utils.c)

SRCS := $(addprefix $(SRCS_DIR)/, $(MAIN_SRCS) $(SIGNALS_SRCS) $(PARSING_SRCS) $(DEBUG_SRCS) $(ENVIRON_SRCS) $(EXECUTE_SRCS))
OBJS := $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

all: libft/libft.a $(NAME)

libft/libft.a:
	@$(MAKE) -C libft

$(NAME): $(OBJS) libft/libft.a
	$(CC) $(OBJS) -o $@ $(LIBS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)
	@$(MAKE) -C libft clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C libft fclean

re: fclean all

.PHONY: all re clean fclean 
