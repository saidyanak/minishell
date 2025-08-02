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
PARSERDIR = parser
SIGNALDIR = signal
ENVDIR = create_env

# Libft
LIBFT = $(LIBFTDIR)/libft.a
LIBFT_FLAGS = -L$(LIBFTDIR) -lft

# Leak Tester
LEAK_TESTER = leak_tester
LEAK_SRC = leak_tester.c

# Source files
SRCS = main.c \
       cleanup.c \
	   expand.c	\
	   word_splitting.c \
	   $(ENVDIR)/create_linked_list.c \
	   $(ENVDIR)/create_linked_list_utils.c \
	   $(SIGNALDIR)/singal_handler_utils.c \
	   $(SIGNALDIR)/signal_handler.c \
	   $(PARSERDIR)/tokenizer.c \
	   $(PARSERDIR)/word_parser.c \
	   $(PARSERDIR)/quote_parser.c \
	   $(PARSERDIR)/quote_utils.c \
	   $(PARSERDIR)/operator_handler.c \
	   $(PARSERDIR)/token_utils.c \
	   $(EXECUTEDIR)/heredoc_handler.c \
	   $(EXECUTEDIR)/syntax_checker.c \
	   $(EXECUTEDIR)/simple_heredoc.c \
       $(EXECUTEDIR)/execute.c \
       $(EXECUTEDIR)/execute_external.c \
       $(EXECUTEDIR)/execute_envp.c \
       $(EXECUTEDIR)/execute_argv_envp.c \
	   $(EXECUTEDIR)/execute_multiple.c \
	   $(EXECUTEDIR)/execute_multiple_utils.c \
	   $(EXECUTEDIR)/execute_multiple_utils2.c \
	   $(EXECUTEDIR)/execute_utils.c \
	   $(EXECUTEDIR)/handle_redirect.c \
       $(BUILTINDIR)/build_in.c \
       $(BUILTINDIR)/ft_echo.c \
	   $(BUILTINDIR)/ft_cd.c \
	   $(BUILTINDIR)/ft_pwd.c \
	   $(BUILTINDIR)/ft_env.c \
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
CYAN = \033[0;36m
MAGENTA = \033[0;35m
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

# Leak Tester compilation
$(LEAK_TESTER): $(LEAK_SRC)
	@echo "$(CYAN)Compiling leak tester...$(NC)"
	@$(CC) -g $(LEAK_SRC) -o $(LEAK_TESTER)
	@echo "$(GREEN)✅ Leak tester compiled successfully!$(NC)"

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

# Additional rules for development
debug: CFLAGS += -DDEBUG -fsanitize=address
debug: re

valgrind: $(NAME)
	@echo "$(BLUE)Running valgrind with readline suppressions...$(NC)"
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --suppressions=readline.supp --track-origins=yes ./$(NAME)

valgrind-pipe: $(NAME)
	@echo "$(BLUE)Testing pipe command with valgrind...$(NC)"
	echo "pwd | echo hello" | valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --suppressions=readline.supp --track-origins=yes ./$(NAME)

test: $(NAME)
	@echo "$(BLUE)Running basic tests...$(NC)"
	@echo "echo hello" | ./$(NAME)
	@echo "ls | grep .c" | ./$(NAME)
	@echo "echo 'test with quotes'" | ./$(NAME)

# ========== LEAK TESTING SECTION ==========

# Comprehensive leak testing
test-leaks: $(NAME) $(LEAK_TESTER)
	@echo "$(CYAN)🔍 Running comprehensive leak tests...$(NC)"
	@./$(LEAK_TESTER)
	@echo "$(GREEN)✅ Leak testing completed!$(NC)"

# Valgrind-based leak testing
test-valgrind: $(NAME) $(LEAK_TESTER)
	@echo "$(MAGENTA)🔍 Running valgrind-based leak tests...$(NC)"
	@./$(LEAK_TESTER) --valgrind
	@echo "$(GREEN)✅ Valgrind leak testing completed!$(NC)"

# Interactive custom testing
test-custom: $(NAME) $(LEAK_TESTER)
	@echo "$(CYAN)🔍 Starting interactive leak testing...$(NC)"
	@echo "$(YELLOW)You can now enter commands to test. Type 'quit' to exit.$(NC)"
	@./$(LEAK_TESTER) --custom

# Stress testing (100 iterations)
test-stress: $(NAME) $(LEAK_TESTER)
	@echo "$(YELLOW)🔄 Running stress tests (100 iterations)...$(NC)"
	@for i in $$(seq 1 100); do \
		echo "$(CYAN)Test iteration $$i/100$(NC)"; \
		timeout 5s ./$(LEAK_TESTER) > /dev/null 2>&1 || true; \
	done
	@echo "$(GREEN)✅ Stress testing completed!$(NC)"

# Memory check with detailed report
memcheck: $(NAME)
	@echo "$(BLUE)🔍 Running comprehensive memory check...$(NC)"
	@echo "ls | cat | grep minishell" | valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --verbose --log-file=valgrind_report.txt ./$(NAME) 2>/dev/null || true
	@echo "$(GREEN)Memory check report saved to valgrind_report.txt$(NC)"
	@if [ -f valgrind_report.txt ]; then \
		echo "$(YELLOW)📊 Memory Report Summary:$(NC)"; \
		grep -E "(ERROR SUMMARY|definitely lost|indirectly lost|possibly lost)" valgrind_report.txt || true; \
	fi

# Quick functionality test
quick-test: $(NAME)
	@echo "$(BLUE)⚡ Running quick functionality tests...$(NC)"
	@echo "$(CYAN)Testing basic commands...$(NC)"
	@echo 'echo "Hello World"' | ./$(NAME)
	@echo "$(CYAN)Testing pipes...$(NC)"
	@echo 'ls | head -3' | ./$(NAME)
	@echo "$(CYAN)Testing redirections...$(NC)"
	@echo 'echo "test" > /tmp/minishell_test.txt' | ./$(NAME)
	@echo 'cat < /tmp/minishell_test.txt' | ./$(NAME)
	@rm -f /tmp/minishell_test.txt
	@echo "$(CYAN)Testing builtin commands...$(NC)"
	@echo 'pwd' | ./$(NAME)
	@echo 'export TEST_VAR=hello' | ./$(NAME)
	@echo 'echo $$TEST_VAR' | ./$(NAME)
	@echo "$(GREEN)✅ Quick tests completed!$(NC)"

# Advanced memory analysis
memcheck-advanced: $(NAME)
	@echo "$(MAGENTA)🔬 Running advanced memory analysis...$(NC)"
	@echo "$(YELLOW)Testing basic commands...$(NC)"
	@echo 'echo hello' | valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./$(NAME)
	@echo "$(YELLOW)Testing pipes...$(NC)"
	@echo 'ls | cat' | valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./$(NAME)
	@echo "$(YELLOW)Testing redirections...$(NC)"
	@echo 'echo test > /tmp/test.txt' | valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./$(NAME)
	@rm -f /tmp/test.txt
	@echo "$(GREEN)✅ Advanced memory analysis completed!$(NC)"

# Performance test
perf-test: $(NAME)
	@echo "$(YELLOW)⚡ Running performance tests...$(NC)"
	@time (for i in $$(seq 1 50); do echo 'echo test' | ./$(NAME) >/dev/null 2>&1; done)
	@echo "$(GREEN)✅ Performance test completed!$(NC)"

# Generate test report
test-report: $(NAME) $(LEAK_TESTER)
	@echo "$(BLUE)📋 Generating comprehensive test report...$(NC)"
	@echo "=== MINISHELL TEST REPORT ===" > test_report.txt
	@echo "Date: $$(date)" >> test_report.txt
	@echo "" >> test_report.txt
	@echo "=== BASIC FUNCTIONALITY ===" >> test_report.txt
	@echo 'echo hello' | ./$(NAME) >> test_report.txt 2>&1 || true
	@echo "" >> test_report.txt
	@echo "=== LEAK TEST RESULTS ===" >> test_report.txt
	@./$(LEAK_TESTER) >> test_report.txt 2>&1 || true
	@echo "" >> test_report.txt
	@echo "$(GREEN)📄 Test report saved to test_report.txt$(NC)"

# Cleanup test files
clean-tests:
	@echo "$(RED)🧹 Cleaning test files...$(NC)"
	@rm -f $(LEAK_TESTER)
	@rm -f test_output.txt test_list.txt valgrind_report.txt test_report.txt
	@rm -f /tmp/minishell_test.txt /tmp/test.txt
	@echo "$(GREEN)✅ Test files cleaned!$(NC)"

# ========== END LEAK TESTING SECTION ==========

norm:
	@echo "$(BLUE)Checking norminette...$(NC)"
	@norminette $(SRCS) minishell.h

# Help
help:
	@echo "$(YELLOW)Available targets:$(NC)"
	@echo ""
	@echo "$(CYAN)📦 BUILD TARGETS:$(NC)"
	@echo "  $(GREEN)all$(NC)           - Build the project"
	@echo "  $(GREEN)clean$(NC)         - Remove object files"
	@echo "  $(GREEN)fclean$(NC)        - Remove object files and executable"
	@echo "  $(GREEN)re$(NC)            - Rebuild the project"
	@echo "  $(GREEN)debug$(NC)         - Build with debug flags and address sanitizer"
	@echo ""
	@echo "$(CYAN)🧪 BASIC TESTING:$(NC)"
	@echo "  $(GREEN)test$(NC)          - Run basic functionality tests"
	@echo "  $(GREEN)quick-test$(NC)    - Run quick functionality tests"
	@echo "  $(GREEN)valgrind$(NC)      - Run with valgrind and readline suppressions"
	@echo "  $(GREEN)valgrind-pipe$(NC) - Test pipe command with valgrind"
	@echo ""
	@echo "$(CYAN)🔍 LEAK TESTING:$(NC)"
	@echo "  $(GREEN)test-leaks$(NC)    - Run comprehensive leak tests"
	@echo "  $(GREEN)test-valgrind$(NC) - Run valgrind-based leak tests"
	@echo "  $(GREEN)test-custom$(NC)   - Interactive custom leak testing"
	@echo "  $(GREEN)test-stress$(NC)   - Run stress tests (100 iterations)"
	@echo ""
	@echo "$(CYAN)🔬 MEMORY ANALYSIS:$(NC)"
	@echo "  $(GREEN)memcheck$(NC)      - Comprehensive memory check with report"
	@echo "  $(GREEN)memcheck-advanced$(NC) - Advanced memory analysis"
	@echo "  $(GREEN)perf-test$(NC)     - Performance testing"
	@echo ""
	@echo "$(CYAN)📋 REPORTING & CLEANUP:$(NC)"
	@echo "  $(GREEN)test-report$(NC)   - Generate comprehensive test report"
	@echo "  $(GREEN)clean-tests$(NC)   - Clean test files"
	@echo "  $(GREEN)norm$(NC)          - Check norminette"
	@echo "  $(GREEN)help$(NC)          - Show this help"
	@echo ""
	@echo "$(YELLOW)💡 TIP: For leak detection, first run 'test-leaks' then 'test-valgrind'$(NC)"

.PHONY: all clean fclean re debug valgrind valgrind-pipe test norm help \
        test-leaks test-valgrind test-custom test-stress memcheck quick-test \
        memcheck-advanced perf-test test-report clean-tests