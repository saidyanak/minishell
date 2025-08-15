NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I. -I./libft
READLINE_FLAGS = -lreadline

SRCDIR = src
OBJDIR = obj
LIBFTDIR = libft
BUILTINDIR = build-in
EXECUTEDIR = execute
HEREDOC = execute/heredoc
EXPANDIR = expand
PARSERDIR = parser
SIGNALDIR = signal
ENVDIR = create_env
CLEANDIR = cleanup

LIBFT = $(LIBFTDIR)/libft.a
LIBFT_FLAGS = -L$(LIBFTDIR) -lft

LEAK_TESTER = leak_tester
LEAK_SRC = leak_tester.c

SRCS = main.c \
	   $(EXPANDIR)/expand.c	\
	   $(EXPANDIR)/word_splitting.c \
	   $(EXPANDIR)/expand_quotes.c \
	   $(EXPANDIR)/expand_utils_1.c \
	   $(EXPANDIR)/expand_utils_2.c \
	   $(EXPANDIR)/expand_utils_3.c \
	   $(EXPANDIR)/word_splitting_utils_1.c \
	   $(EXPANDIR)/word_splitting_utils_2.c \
       $(CLEANDIR)/cleanup.c \
       $(CLEANDIR)/cleanup_utils.c \
	   $(ENVDIR)/create_linked_list.c \
	   $(ENVDIR)/create_linked_list_utils.c \
	   $(SIGNALDIR)/singal_handler_utils.c \
	   $(SIGNALDIR)/signal_handler.c \
	   $(SIGNALDIR)/signal_utils.c \
	   $(PARSERDIR)/tokenizer.c \
	   $(PARSERDIR)/word_parser.c \
	   $(PARSERDIR)/quote_parser.c \
	   $(PARSERDIR)/quote_utils.c \
	   $(PARSERDIR)/operator_handler.c \
	   $(PARSERDIR)/token_utils.c \
	   $(EXECUTEDIR)/syntax_checker_main.c \
	   $(EXECUTEDIR)/syntax_checker_pipe.c \
	   $(EXECUTEDIR)/syntax_checker_redir.c \
       $(EXECUTEDIR)/execute.c \
	   $(HEREDOC)/heredoc_expand.c \
	   $(HEREDOC)/check_heredoc.c \
	   $(HEREDOC)/heredoc_cleanup.c\
	   $(HEREDOC)/heredoc_process.c\
	   $(HEREDOC)/heredoc_reader.c\
	   $(HEREDOC)/heredoc_handler.c\
	   $(EXECUTEDIR)/execute_command_utils.c \
       $(EXECUTEDIR)/execute_path_finder.c \
       $(EXECUTEDIR)/execute_external.c \
       $(EXECUTEDIR)/execute_envp.c \
       $(EXECUTEDIR)/execute_argv_envp.c \
	   $(EXECUTEDIR)/execute_multiple.c \
	   $(EXECUTEDIR)/execute_multiple_helpers.c \
	   $(EXECUTEDIR)/execute_multiple_utils.c \
	   $(EXECUTEDIR)/execute_multiple_utils2.c \
	   $(EXECUTEDIR)/execute_multiple_utils3.c \
	   $(EXECUTEDIR)/execute_multiple_utils4.c \
	   $(EXECUTEDIR)/execute_utils.c \
	   $(EXECUTEDIR)/handle_redirect.c \
	   $(EXECUTEDIR)/handle_redirect_utils.c \
	   $(EXECUTEDIR)/redirect_fd.c \
       $(BUILTINDIR)/build_in.c \
       $(BUILTINDIR)/echo/ft_echo.c \
	   $(BUILTINDIR)/cd/ft_cd.c \
	   $(BUILTINDIR)/cd/cd_utils.c \
	   $(BUILTINDIR)/pwd/ft_pwd.c \
	   $(BUILTINDIR)/env/ft_env.c \
	   $(BUILTINDIR)/export/export_parsing.c \
	   $(BUILTINDIR)/export/export_env_utils.c \
	   $(BUILTINDIR)/export/export_operations.c \
	   $(BUILTINDIR)/export/export_display.c \
	   $(BUILTINDIR)/export/export_main.c \
	   $(BUILTINDIR)/unset/ft_unset.c \
	   $(BUILTINDIR)/unset/unset_env.c \
	   $(BUILTINDIR)/exit/ft_exit.c \
	   $(BUILTINDIR)/exit/ft_exit_utils.c

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
YELLOW = \033[1;33m
CYAN = \033[0;36m
MAGENTA = \033[0;35m
NC = \033[0m

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
	@rm -f $(LEAK_TESTER)
	@rm -f test_output.txt test_list.txt valgrind_report.txt
	@make fclean -C $(LIBFTDIR)
	@echo "$(GREEN)✅ $(NAME) cleaned!$(NC)"

re: fclean all

valgrind: $(NAME)
	@echo "$(BLUE)Running valgrind with readline suppressions...$(NC)"
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --track-fds=yes  ./$(NAME)

norm:
	@echo "$(BLUE)Checking norminette...$(NC)"
	@norminette $(SRCS) minishell.h

.PHONY: all clean fclean re debug valgrind valgrind-pipe test norm help \
        test-leaks test-valgrind test-custom test-stress memcheck quick-test \
        memcheck-advanced perf-test test-report clean-tests