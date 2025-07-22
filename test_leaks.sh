#!/bin/bash

# Memory Leak Test Suite for Minishell
echo "🔍 Memory Leak Test Suite Starting..."

# Test input files
echo "echo hello world" > test_simple.txt
echo "ls -la" > test_ls.txt
echo "export TEST=hello && echo \$TEST" > test_export.txt
echo "cd /tmp && pwd" > test_cd.txt
echo "echo \$USER \$HOME" > test_env.txt
echo "ls | wc -l" > test_pipe.txt

# Test 1: Simple command
echo "📝 Test 1: Simple echo command"
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --quiet ./minishell < test_simple.txt 2>&1 | grep -E "(LEAK SUMMARY|ERROR SUMMARY|definitely lost|indirectly lost)"

# Test 2: External command
echo "📝 Test 2: External command (ls)"
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --quiet ./minishell < test_ls.txt 2>&1 | grep -E "(LEAK SUMMARY|ERROR SUMMARY|definitely lost|indirectly lost)"

# Test 3: Export and environment
echo "📝 Test 3: Export and environment variables"
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --quiet ./minishell < test_export.txt 2>&1 | grep -E "(LEAK SUMMARY|ERROR SUMMARY|definitely lost|indirectly lost)"

# Test 4: Built-in commands
echo "📝 Test 4: Built-in commands (cd, pwd)"
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --quiet ./minishell < test_cd.txt 2>&1 | grep -E "(LEAK SUMMARY|ERROR SUMMARY|definitely lost|indirectly lost)"

# Test 5: Variable expansion
echo "📝 Test 5: Variable expansion"
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --quiet ./minishell < test_env.txt 2>&1 | grep -E "(LEAK SUMMARY|ERROR SUMMARY|definitely lost|indirectly lost)"

# Test 6: Pipe (if implemented)
echo "📝 Test 6: Pipe command"
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --quiet ./minishell < test_pipe.txt 2>&1 | grep -E "(LEAK SUMMARY|ERROR SUMMARY|definitely lost|indirectly lost)"

# Cleanup
rm -f test_*.txt

echo "✅ Memory leak tests completed!"
