#ifndef LEAK_TESTER_H
#define LEAK_TESTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Renk kodları
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define RESET   "\033[0m"

// Memory allocation tracking
typedef struct s_mem_block {
    void *ptr;
    size_t size;
    char *file;
    int line;
    char *func;
    struct s_mem_block *next;
} t_mem_block;

static t_mem_block *g_allocated = NULL;
static size_t g_total_allocated = 0;
static size_t g_total_freed = 0;
static int g_allocation_count = 0;
static int g_free_count = 0;

// Makrolar
#define malloc(size) debug_malloc(size, __FILE__, __LINE__, __func__)
#define calloc(num, size) debug_calloc(num, size, __FILE__, __LINE__, __func__)
#define realloc(ptr, size) debug_realloc(ptr, size, __FILE__, __LINE__, __func__)
#define free(ptr) debug_free(ptr, __FILE__, __LINE__, __func__)
#define strdup(str) debug_strdup(str, __FILE__, __LINE__, __func__)

// Debug fonksiyonları
void *debug_malloc(size_t size, const char *file, int line, const char *func);
void *debug_calloc(size_t num, size_t size, const char *file, int line, const char *func);
void *debug_realloc(void *ptr, size_t size, const char *file, int line, const char *func);
void debug_free(void *ptr, const char *file, int line, const char *func);
char *debug_strdup(const char *str, const char *file, int line, const char *func);

// Utility fonksiyonları
void add_memory_block(void *ptr, size_t size, const char *file, int line, const char *func);
t_mem_block *find_memory_block(void *ptr);
void remove_memory_block(void *ptr);
void print_memory_stats(void);
void print_leaks(void);
void cleanup_leak_tester(void);

// Test fonksiyonları
void test_basic_commands(void);
void test_pipes(void);
void test_redirections(void);
void test_environment_variables(void);
void test_builtin_commands(void);
void test_signal_handling(void);
void test_edge_cases(void);
void run_all_tests(void);

// Implementation

void *debug_malloc(size_t size, const char *file, int line, const char *func) {
    void *ptr = malloc(size);
    if (ptr) {
        add_memory_block(ptr, size, file, line, func);
        g_total_allocated += size;
        g_allocation_count++;
    }
    return ptr;
}

void *debug_calloc(size_t num, size_t size, const char *file, int line, const char *func) {
    void *ptr = calloc(num, size);
    if (ptr) {
        add_memory_block(ptr, num * size, file, line, func);
        g_total_allocated += (num * size);
        g_allocation_count++;
    }
    return ptr;
}

void *debug_realloc(void *old_ptr, size_t size, const char *file, int line, const char *func) {
    if (old_ptr) {
        t_mem_block *block = find_memory_block(old_ptr);
        if (block) {
            g_total_freed += block->size;
            remove_memory_block(old_ptr);
        }
    }
    
    void *ptr = realloc(old_ptr, size);
    if (ptr && size > 0) {
        add_memory_block(ptr, size, file, line, func);
        g_total_allocated += size;
        g_allocation_count++;
    }
    return ptr;
}

void debug_free(void *ptr, const char *file, int line, const char *func) {
    if (!ptr) return;
    
    t_mem_block *block = find_memory_block(ptr);
    if (block) {
        g_total_freed += block->size;
        g_free_count++;
        remove_memory_block(ptr);
        free(ptr);
    } else {
        printf(RED "WARNING: Attempting to free untracked pointer at %s:%d in %s()\n" RESET, 
               file, line, func);
    }
}

char *debug_strdup(const char *str, const char *file, int line, const char *func) {
    if (!str) return NULL;
    
    size_t len = strlen(str) + 1;
    char *dup = malloc(len);
    if (dup) {
        strcpy(dup, str);
        add_memory_block(dup, len, file, line, func);
        g_total_allocated += len;
        g_allocation_count++;
    }
    return dup;
}

void add_memory_block(void *ptr, size_t size, const char *file, int line, const char *func) {
    t_mem_block *block = malloc(sizeof(t_mem_block));
    if (!block) return;
    
    block->ptr = ptr;
    block->size = size;
    block->file = strdup(file);
    block->line = line;
    block->func = strdup(func);
    block->next = g_allocated;
    g_allocated = block;
}

t_mem_block *find_memory_block(void *ptr) {
    t_mem_block *current = g_allocated;
    while (current) {
        if (current->ptr == ptr) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void remove_memory_block(void *ptr) {
    t_mem_block **current = &g_allocated;
    while (*current) {
        if ((*current)->ptr == ptr) {
            t_mem_block *to_remove = *current;
            *current = (*current)->next;
            free(to_remove->file);
            free(to_remove->func);
            free(to_remove);
            return;
        }
        current = &(*current)->next;
    }
}

void print_memory_stats(void) {
    printf(CYAN "\n=== MEMORY STATISTICS ===\n" RESET);
    printf("Total Allocations: %d\n", g_allocation_count);
    printf("Total Frees: %d\n", g_free_count);
    printf("Total Allocated: %zu bytes\n", g_total_allocated);
    printf("Total Freed: %zu bytes\n", g_total_freed);
    
    size_t leaked = g_total_allocated - g_total_freed;
    if (leaked > 0) {
        printf(RED "LEAKED MEMORY: %zu bytes\n" RESET, leaked);
    } else {
        printf(GREEN "NO MEMORY LEAKS DETECTED!\n" RESET);
    }
    printf(CYAN "========================\n\n" RESET);
}

void print_leaks(void) {
    if (!g_allocated) {
        printf(GREEN "No memory leaks detected!\n" RESET);
        return;
    }
    
    printf(RED "\n=== MEMORY LEAKS DETECTED ===\n" RESET);
    t_mem_block *current = g_allocated;
    int leak_count = 0;
    
    while (current) {
        leak_count++;
        printf(RED "LEAK #%d:\n" RESET, leak_count);
        printf("  Address: %p\n", current->ptr);
        printf("  Size: %zu bytes\n", current->size);
        printf("  File: %s\n", current->file);
        printf("  Line: %d\n", current->line);
        printf("  Function: %s()\n", current->func);
        printf("  ---\n");
        current = current->next;
    }
    printf(RED "Total leaks: %d\n" RESET, leak_count);
    printf(RED "=============================\n\n" RESET);
}

void cleanup_leak_tester(void) {
    while (g_allocated) {
        t_mem_block *to_remove = g_allocated;
        g_allocated = g_allocated->next;
        free(to_remove->file);
        free(to_remove->func);
        free(to_remove);
    }
}

// Test fonksiyonları
void test_basic_commands(void) {
    printf(YELLOW "Testing basic commands...\n" RESET);
    
    // Test echo
    system("echo 'ls -la' | ./minishell");
    system("echo 'pwd' | ./minishell");
    system("echo 'echo hello world' | ./minishell");
    
    // Test with different arguments
    system("echo 'ls -la /tmp' | ./minishell");
    system("echo 'cat /etc/passwd' | ./minishell");
    
    printf(GREEN "Basic commands test completed.\n\n" RESET);
}

void test_pipes(void) {
    printf(YELLOW "Testing pipes...\n" RESET);
    
    system("echo 'ls | grep minishell' | ./minishell");
    system("echo 'cat /etc/passwd | head -5' | ./minishell");
    system("echo 'ls -la | wc -l' | ./minishell");
    system("echo 'echo hello | cat | cat' | ./minishell");
    
    // Multiple pipes
    system("echo 'ls | head -10 | tail -5' | ./minishell");
    system("echo 'cat /etc/passwd | grep root | wc -l' | ./minishell");
    
    printf(GREEN "Pipes test completed.\n\n" RESET);
}

void test_redirections(void) {
    printf(YELLOW "Testing redirections...\n" RESET);
    
    // Output redirection
    system("echo 'echo hello > test_output.txt' | ./minishell");
    system("echo 'ls -la > test_list.txt' | ./minishell");
    
    // Input redirection
    system("echo 'cat < test_output.txt' | ./minishell");
    system("echo 'wc -l < test_list.txt' | ./minishell");
    
    // Append redirection
    system("echo 'echo world >> test_output.txt' | ./minishell");
    
    // Here document
    system("echo 'cat << EOF\nhello\nworld\nEOF' | ./minishell");
    
    // Cleanup
    system("rm -f test_output.txt test_list.txt");
    
    printf(GREEN "Redirections test completed.\n\n" RESET);
}

void test_environment_variables(void) {
    printf(YELLOW "Testing environment variables...\n" RESET);
    
    system("echo 'echo $HOME' | ./minishell");
    system("echo 'echo $PATH' | ./minishell");
    system("echo 'echo $USER' | ./minishell");
    system("echo 'export TEST_VAR=hello' | ./minishell");
    system("echo 'echo $TEST_VAR' | ./minishell");
    system("echo 'unset TEST_VAR' | ./minishell");
    
    printf(GREEN "Environment variables test completed.\n\n" RESET);
}

void test_builtin_commands(void) {
    printf(YELLOW "Testing builtin commands...\n" RESET);
    
    system("echo 'pwd' | ./minishell");
    system("echo 'cd /tmp' | ./minishell");
    system("echo 'pwd' | ./minishell");
    system("echo 'cd -' | ./minishell");
    system("echo 'echo hello world' | ./minishell");
    system("echo 'env | grep USER' | ./minishell");
    system("echo 'export NEW_VAR=test' | ./minishell");
    system("echo 'unset NEW_VAR' | ./minishell");
    system("echo 'exit 0' | ./minishell");
    
    printf(GREEN "Builtin commands test completed.\n\n" RESET);
}

void test_signal_handling(void) {
    printf(YELLOW "Testing signal handling...\n" RESET);
    
    // Test Ctrl+C handling
    printf("Testing Ctrl+C handling (automatic)...\n");
    system("timeout 2s bash -c 'echo \"sleep 10\" | ./minishell' || true");
    
    printf(GREEN "Signal handling test completed.\n\n" RESET);
}

void test_edge_cases(void) {
    printf(YELLOW "Testing edge cases...\n" RESET);
    
    // Empty input
    system("echo '' | ./minishell");
    
    // Only spaces
    system("echo '   ' | ./minishell");
    
    // Invalid commands
    system("echo 'nonexistentcommand' | ./minishell");
    
    // Special characters
    system("echo 'echo \"hello world\"' | ./minishell");
    system("echo \"echo 'hello world'\" | ./minishell");
    
    // Long commands
    system("echo 'echo this is a very long command with many arguments to test memory allocation' | ./minishell");
    
    printf(GREEN "Edge cases test completed.\n\n" RESET);
}

void run_all_tests(void) {
    printf(BLUE "Starting comprehensive minishell leak testing...\n\n" RESET);
    
    test_basic_commands();
    test_pipes();
    test_redirections();
    test_environment_variables();
    test_builtin_commands();
    test_signal_handling();
    test_edge_cases();
    
    printf(BLUE "All tests completed!\n\n" RESET);
    print_memory_stats();
    print_leaks();
}

// Valgrind wrapper fonksiyonu
void run_valgrind_test(const char *command) {
    char valgrind_cmd[1024];
    snprintf(valgrind_cmd, sizeof(valgrind_cmd), 
             "echo '%s' | valgrind --leak-check=full --show-leak-kinds=all "
             "--track-origins=yes --verbose ./minishell 2>&1", command);
    
    printf(CYAN "Running: %s\n" RESET, command);
    system(valgrind_cmd);
    printf("\n");
}

void run_valgrind_tests(void) {
    printf(MAGENTA "=== VALGRIND LEAK TESTING ===\n\n" RESET);
    
    run_valgrind_test("ls -la");
    run_valgrind_test("echo hello | cat");
    run_valgrind_test("ls | grep minishell");
    run_valgrind_test("echo hello > /tmp/test.txt");
    run_valgrind_test("cat < /tmp/test.txt");
    run_valgrind_test("export TEST=value");
    run_valgrind_test("echo $TEST");
    run_valgrind_test("pwd");
    run_valgrind_test("cd /tmp");
    run_valgrind_test("exit 0");
    
    system("rm -f /tmp/test.txt");
    
    printf(MAGENTA "=== VALGRIND TESTING COMPLETED ===\n\n" RESET);
}

// Main function
int main(int argc, char **argv) {
    printf(GREEN "Minishell Comprehensive Leak Tester\n" RESET);
    printf(GREEN "===================================\n\n" RESET);
    
    if (argc > 1 && strcmp(argv[1], "--valgrind") == 0) {
        run_valgrind_tests();
    } else if (argc > 1 && strcmp(argv[1], "--custom") == 0) {
        // Custom test mode
        printf("Custom test mode - enter commands to test:\n");
        char command[1024];
        while (1) {
            printf("Enter command (or 'quit' to exit): ");
            if (!fgets(command, sizeof(command), stdin)) break;
            
            // Remove newline
            command[strcspn(command, "\n")] = 0;
            
            if (strcmp(command, "quit") == 0) break;
            
            char test_cmd[1100];
            snprintf(test_cmd, sizeof(test_cmd), "echo '%s' | ./minishell", command);
            system(test_cmd);
        }
    } else {
        run_all_tests();
    }
    
    cleanup_leak_tester();
    
    printf(GREEN "\nLeak testing completed!\n" RESET);
    printf("Usage:\n");
    printf("  %s              - Run all tests\n", argv[0]);
    printf("  %s --valgrind   - Run valgrind tests\n", argv[0]);
    printf("  %s --custom     - Interactive custom testing\n", argv[0]);
    
    return 0;
}

#endif