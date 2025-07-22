# Program name
NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I. -I./libft
READLINE_FLAGS = -lreadline

# Directories
SRCDIR = src
OBJDIR = obj
LIBFTDIR = libft
BUILTINDIR = build-in
EXECUTEDIR = execute

# Libft
LIBFT = $(LIBFTDIR)/libft.a
LIBFT_FLAGS = -L$(LIBFTDIR) -lft

# Source files
SRCS = main.c \
       tokenize.c \
       parse_quote.c \
       create_linked_list.c \
       cleanup.c \
       debug.c \
	   expand.c	\
	   word_splitting.c \
	   export_handler.c \
	   $(EXECUTEDIR)/heredoc_handler.c \
	   $(EXECUTEDIR)/syntax_checker.c \
	   $(EXECUTEDIR)/simple_heredoc.c \
       $(EXECUTEDIR)/execute.c \
       $(EXECUTEDIR)/execute_external.c \
       $(EXECUTEDIR)/execute_envp.c \
       $(EXECUTEDIR)/execute_argv_envp.c \
	   $(EXECUTEDIR)/execute_multiple.c \
	   $(EXECUTEDIR)/execute_utils.c \
       $(BUILTINDIR)/build_in.c \
       $(BUILTINDIR)/ft_echo.c \
	   $(BUILTINDIR)/ft_cd.c \
	   $(BUILTINDIR)/ft_pwd.c \
	   $(BUILTINDIR)/ft_env.c \
	   $(BUILTINDIR)/ft_export.c \
	   $(BUILTINDIR)/export/export_parsing.c \
	   $(BUILTINDIR)/export/export_env_utils.c \
	   $(BUILTINDIR)/export/export_operations.c \
	   $(BUILTINDIR)/export/export_display.c \
	   $(BUILTINDIR)/export/export_main.c \
	   $(BUILTINDIR)/ft_unset.c \
	   $(BUILTINDIR)/ft_exit.c

# Object files
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
YELLOW = \033[1;33m
NC = \033[0m # No Color

# Rules
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(BLUE)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_FLAGS) $(READLINE_FLAGS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) compiled successfully!$(NC)"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@echo "$(BLUE)Building libft...$(NC)"
	@make -C $(LIBFTDIR)
	@echo "$(GREEN)✅ libft compiled successfully!$(NC)"

clean:
	@echo "$(RED)Cleaning object files...$(NC)"
	@rm -rf $(OBJDIR)
	@make clean -C $(LIBFTDIR)
	@echo "$(GREEN)✅ Object files cleaned!$(NC)"

fclean: clean
	@echo "$(RED)Cleaning $(NAME)...$(NC)"
	@rm -f $(NAME)
	@make fclean -C $(LIBFTDIR)
	@echo "$(GREEN)✅ $(NAME) cleaned!$(NC)"

re: fclean all

# Additional rules for development
debug: CFLAGS += -DDEBUG -fsanitize=address
debug: re

valgrind: $(NAME)
	@echo "$(BLUE)Running valgrind...$(NC)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

test: $(NAME)
	@echo "$(BLUE)Running basic tests...$(NC)"
	@echo "echo hello" | ./$(NAME)
	@echo "ls | grep .c" | ./$(NAME)
	@echo "echo 'test with quotes'" | ./$(NAME)

norm:
	@echo "$(BLUE)Checking norminette...$(NC)"
	@norminette $(SRCS) minishell.h

# Help
help:
	@echo "$(YELLOW)Available targets:$(NC)"
	@echo "  $(GREEN)all$(NC)      - Build the project"
	@echo "  $(GREEN)clean$(NC)    - Remove object files"
	@echo "  $(GREEN)fclean$(NC)   - Remove object files and executable"
	@echo "  $(GREEN)re$(NC)       - Rebuild the project"
	@echo "  $(GREEN)debug$(NC)    - Build with debug flags and address sanitizer"
	@echo "  $(GREEN)valgrind$(NC) - Run with valgrind"
	@echo "  $(GREEN)test$(NC)     - Run basic tests"
	@echo "  $(GREEN)norm$(NC)     - Check norminette"
	@echo "  $(GREEN)help$(NC)     - Show this help"

.PHONY: all clean fclean re debug valgrind test norm help
