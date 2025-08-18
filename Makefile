NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
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
       $(CLEANDIR)/cleanup_base.c \
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
	   $(HEREDOC)/heredoc_temp_file.c \
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
	   $(EXECUTEDIR)/token_filtering.c\
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
	   $(BUILTINDIR)/cd/count_cd_arguments.c \
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

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_FLAGS) $(READLINE_FLAGS) -o $(NAME)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFTDIR)

clean:
	@rm -rf $(OBJDIR)
	@make clean -C $(LIBFTDIR)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFTDIR)

re: fclean all

valgrind: $(NAME)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --track-fds=yes  ./$(NAME)

.PHONY: all clean fclean re valgrind