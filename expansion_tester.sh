#!/bin/bash

# =========================================================================
# MINISHELL EXPANSION TESTER - FINAL FIX
# =========================================================================

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Minishell path
MINISHELL_PATH=${1:-"./minishell"}

# Check if minishell exists
if [ ! -f "$MINISHELL_PATH" ]; then
    echo -e "${RED}Error: Minishell not found at $MINISHELL_PATH${NC}"
    exit 1
fi

# Test environment setup
export TEST_VAR="hello"
export HOME="/home/test"
export USER="testuser" 
export EMPTY_VAR=""
export H="H"
export HO=""
export TEST_NUM="42"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  MINISHELL EXPANSION TESTER v3.0${NC}"
echo -e "${BLUE}  FIXED OUTPUT PARSING${NC}"
echo -e "${BLUE}========================================${NC}"
echo

# Function to extract actual output from minishell
extract_minishell_output() {
    local full_output="$1"
    local command="$2"
    
    # Method 1: Get line after the command
    local output_line=$(echo "$full_output" | grep -A1 "echo $command" | tail -n1)
    
    # Method 2: If method 1 fails, try getting the line that's not a prompt
    if [[ "$output_line" == *"minishell$"* ]] || [[ "$output_line" == *"exit"* ]]; then
        output_line=$(echo "$full_output" | grep -v "minishell\$" | grep -v "exit" | grep -v "echo $command" | head -n1)
    fi
    
    # Method 3: If still not good, try between command and next prompt
    if [[ "$output_line" == *"minishell$"* ]] || [[ "$output_line" == *"exit"* ]] || [[ -z "$output_line" ]]; then
        # Split by lines and find the one after command
        local lines=($(echo "$full_output" | tr '\n' '|'))
        local found_command=false
        for line in "${lines[@]}"; do
            line=$(echo "$line" | tr '|' '\n')
            if [[ "$line" == *"echo $command"* ]]; then
                found_command=true
                continue
            fi
            if $found_command && [[ ! "$line" == *"minishell$"* ]] && [[ ! "$line" == *"exit"* ]] && [[ -n "$line" ]]; then
                output_line="$line"
                break
            fi
        done
    fi
    
    echo "$output_line"
}

# Function to run test - COMPLETELY FIXED
run_test() {
    local test_name="$1"
    local command="$2"
    local description="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}Test $TOTAL_TESTS: $test_name${NC}"
    echo -e "${BLUE}Command: echo $command${NC}"
    echo -e "${BLUE}Description: $description${NC}"
    
    # Get bash output
    bash_output=$(bash -c "echo $command" 2>/dev/null)
    
    # Get minishell output
    temp_file=$(mktemp)
    echo "echo $command" > "$temp_file"
    echo "exit" >> "$temp_file"
    
    minishell_full_output=$(timeout 10 "$MINISHELL_PATH" < "$temp_file" 2>/dev/null)
    
    # Extract the actual echo output
    minishell_output=$(extract_minishell_output "$minishell_full_output" "$command")
    
    # Clean up
    rm -f "$temp_file"
    
    # Debug output (optional)
    echo -e "${BLUE}   Full minishell output:${NC}"
    echo "$minishell_full_output" | sed 's/^/      /'
    echo -e "${BLUE}   Extracted output: '$minishell_output'${NC}"
    echo -e "${BLUE}   Expected output:  '$bash_output'${NC}"
    
    # Compare outputs
    if [ "$bash_output" = "$minishell_output" ]; then
        echo -e "${GREEN}✅ PASS${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ FAIL${NC}"
        echo -e "   Expected: '$bash_output'"
        echo -e "   Got:      '$minishell_output'"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    echo
}

# Simplified extraction for cleaner output
run_simple_test() {
    local test_name="$1"
    local command="$2"
    local description="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}Test $TOTAL_TESTS: $test_name${NC}"
    echo -e "${BLUE}Command: echo $command${NC}"
    
    # Get bash output
    bash_output=$(bash -c "echo $command" 2>/dev/null)
    
    # Get minishell output - simpler method
    temp_file=$(mktemp)
    echo "echo $command" > "$temp_file"
    echo "exit" >> "$temp_file"
    
    # Get only the echo result line
    minishell_output=$(timeout 10 "$MINISHELL_PATH" < "$temp_file" 2>/dev/null | grep -A1 "echo $command" | tail -1)
    
    # If that didn't work, try alternative
    if [[ "$minishell_output" == *"minishell$"* ]] || [[ "$minishell_output" == *"exit"* ]]; then
        minishell_output=$(timeout 10 "$MINISHELL_PATH" < "$temp_file" 2>/dev/null | sed -n '/echo '"$command"'/{ n; p; }')
    fi
    
    rm -f "$temp_file"
    
    # Compare outputs
    if [ "$bash_output" = "$minishell_output" ]; then
        echo -e "${GREEN}✅ PASS - Expected: '$bash_output', Got: '$minishell_output'${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ FAIL - Expected: '$bash_output', Got: '$minishell_output'${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    echo
}

# =========================================================================
# TESTS
# =========================================================================
echo -e "${BLUE}=== BASIC VARIABLE EXPANSION ===${NC}"

run_simple_test "Basic Variable" '$TEST_VAR' "Simple variable expansion"
run_simple_test "Undefined Variable" '$UNDEFINED_VAR' "Undefined variable should expand to empty"
run_simple_test "Empty Variable" '$EMPTY_VAR' "Empty variable expansion"
run_simple_test "HOME Variable" '$HOME' "HOME environment variable"

echo -e "${BLUE}=== QUOTE HANDLING TESTS ===${NC}"

run_simple_test "Single Quote Literal" "'$TEST_VAR'" "Single quotes prevent expansion"
run_simple_test "Double Quote Expansion" '"$TEST_VAR"' "Double quotes allow expansion"

echo -e "${BLUE}=== COMPLEX QUOTE COMBINATIONS ===${NC}"

run_simple_test "Dollar Quote HO+ME" '$HO"ME"' "Partial variable + quoted literal"
run_simple_test "Dollar Quote H+O+ME" '$H"O"ME' "Variable + quoted + literal"  
run_simple_test "Quote Dollar" '"$"TEST_VAR' "Quoted dollar + literal"
run_simple_test "Empty Dollar" '"$"' "Empty dollar in quotes"

echo -e "${BLUE}=== ADVANCED TESTS ===${NC}"

run_simple_test "Multiple Variables" '$USER$HOME' "Multiple variable concatenation"
run_simple_test "Variable with Suffix" '$TEST_VAR.txt' "Variable with suffix"

# =========================================================================
# SUMMARY
# =========================================================================
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}           TEST SUMMARY${NC}" 
echo -e "${BLUE}========================================${NC}"
echo -e "Total Tests: $TOTAL_TESTS"
echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
echo -e "${RED}Failed: $FAILED_TESTS${NC}"

if [ $TOTAL_TESTS -gt 0 ]; then
    compatibility=$(( PASSED_TESTS * 100 / TOTAL_TESTS ))
    echo -e "${YELLOW}Compatibility: $compatibility%${NC}"
    
    if [ $compatibility -ge 80 ]; then
        echo -e "${GREEN}🎉 EXCELLENT! Your expansion is working great!${NC}"
    elif [ $compatibility -ge 60 ]; then
        echo -e "${YELLOW}👍 GOOD! Minor issues to fix${NC}"
    else
        echo -e "${RED}❌ Needs work on expansion logic${NC}"
    fi
fi
