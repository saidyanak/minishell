#!/bin/bash

# Minishell Tester Script
# Bu script minishell'inizi test eder ve memory leak kontrolü yapar

# Renkler
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Değişkenler
MINISHELL="./minishell"
LOGDIR="test_logs"
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Test kategorileri - exit komutunu kaldırdık
declare -a BASIC_TESTS=(
    "echo hello world"
    "echo \$USER"
    "echo \$HOME"
    "pwd"
    "env | head -5"
    "export TEST_VAR=42"
    "echo \$TEST_VAR"
    "unset TEST_VAR"
    "cd .."
    "cd -"
)

declare -a PIPE_TESTS=(
    "echo hello | cat"
    "ls | grep minishell"
    "echo test | wc -l"
    "cat /etc/passwd | head -5"
    "echo hello world | cat | cat | cat"
    "ls -la | grep \".c\" | wc -l"
)

declare -a REDIRECT_TESTS=(
    "echo hello > test_output.txt"
    "cat < test_output.txt"
    "echo world >> test_output.txt"
    "cat test_output.txt"
    "ls > /dev/null"
    "echo error >&2"
)

declare -a COMPLEX_TESTS=(
    "echo hello && echo world"
    "false || echo fallback"
    "true && echo success || echo fail"
    "ls nonexistent 2>/dev/null || echo file not found"
    "(echo subshell)"
    "echo \"quoted string\""
    "echo 'single quotes'"
)

# Fonksiyonlar
print_header() {
    echo -e "${BLUE}=====================================${NC}"
    echo -e "${BLUE}    MINISHELL TESTER & LEAK CHECKER${NC}"
    echo -e "${BLUE}=====================================${NC}"
    echo
}

print_section() {
    echo -e "\n${YELLOW}=== $1 ===${NC}"
}

setup() {
    # Log dizinini oluştur
    mkdir -p "$LOGDIR"
    
    # Minishell'in var olup olmadığını kontrol et
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}Error: $MINISHELL bulunamadı!${NC}"
        echo "Önce 'make' komutunu çalıştırın."
        exit 1
    fi
    
    # Valgrind'in yüklü olup olmadığını kontrol et
    if ! command -v valgrind &> /dev/null; then
        echo -e "${YELLOW}Warning: valgrind bulunamadı. Memory leak testleri atlanacak.${NC}"
        VALGRIND_AVAILABLE=false
    else
        VALGRIND_AVAILABLE=true
    fi
}

cleanup() {
    # Test dosyalarını temizle
    rm -f test_output.txt
    rm -f temp_test_*
    # Arka planda kalan processları temizle
    pkill -f "$MINISHELL" 2>/dev/null || true
}

run_test() {
    local test_cmd="$1"
    local test_name="$2"
    local use_valgrind="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "Testing: $test_name... "
    
    # Geçici script dosyası oluştur
    local temp_script="temp_test_$_$(date +%s).sh"
    echo "#!/bin/bash" > "$temp_script"
    echo "echo '$test_cmd' | $MINISHELL" >> "$temp_script"
    echo "echo 'exit' | $MINISHELL" >> "$temp_script"
    chmod +x "$temp_script"
    
    if [ "$use_valgrind" = true ] && [ "$VALGRIND_AVAILABLE" = true ]; then
        # Valgrind ile test çalıştır
        local logfile="$LOGDIR/valgrind_$(echo "$test_name" | tr ' ' '_' | tr '/' '_').log"
        
        # Komutları pipe ile gönder
        timeout 15s bash -c "echo '$test_cmd'; echo 'exit'" | \
        valgrind --leak-check=full \
                 --show-leak-kinds=all \
                 --track-fds=yes \
                 --trace-children=yes \
                 --child-silent-after-fork=yes \
                 --log-file="$logfile" \
                 "$MINISHELL" &>/dev/null
        
        local exit_code=$?
        
        if [ $exit_code -eq 0 ]; then
            # Leak kontrolü yap
            if grep -q "ERROR SUMMARY: 0 errors" "$logfile" && \
               ! grep -q "definitely lost" "$logfile" && \
               ! grep -q "indirectly lost" "$logfile"; then
                echo -e "${GREEN}PASS (No leaks)${NC}"
                PASSED_TESTS=$((PASSED_TESTS + 1))
            else
                echo -e "${RED}FAIL (Memory leaks detected)${NC}"
                FAILED_TESTS=$((FAILED_TESTS + 1))
                echo -e "  ${YELLOW}Check: $logfile${NC}"
            fi
        elif [ $exit_code -eq 124 ]; then
            echo -e "${RED}FAIL (Timeout)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        else
            echo -e "${RED}FAIL (Exit code: $exit_code)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    else
        # Normal test çalıştır - komutları pipe ile gönder
        timeout 10s bash -c "echo '$test_cmd'; echo 'exit'" | "$MINISHELL" &>/dev/null
        local exit_code=$?
        
        if [ $exit_code -eq 0 ]; then
            echo -e "${GREEN}PASS${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        elif [ $exit_code -eq 124 ]; then
            echo -e "${RED}FAIL (Timeout)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        else
            echo -e "${RED}FAIL (Exit code: $exit_code)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    fi
    
    # Geçici dosyayı temizle
    rm -f "$temp_script"
}

run_interactive_test() {
    print_section "Interactive Test with Valgrind"
    
    if [ "$VALGRIND_AVAILABLE" = false ]; then
        echo -e "${YELLOW}Valgrind bulunamadı, interactive test atlanıyor.${NC}"
        return
    fi
    
    echo "Interaktif test başlatılıyor... (çıkmak için 'exit' yazın)"
    echo -e "${YELLOW}Bu test manuel olarak komutları test etmenizi sağlar.${NC}"
    
    local logfile="$LOGDIR/interactive_valgrind.log"
    
    valgrind --leak-check=full \
             --show-leak-kinds=all \
             --track-fds=yes \
             --trace-children=yes \
             --log-file="$logfile" \
             "$MINISHELL"
    
    echo -e "\nInteraktif test tamamlandı."
    echo -e "Valgrind raporu: ${BLUE}$logfile${NC}"
    
    # Hızlı leak özeti göster
    if grep -q "ERROR SUMMARY: 0 errors" "$logfile"; then
        echo -e "${GREEN}No memory errors detected!${NC}"
    else
        echo -e "${RED}Memory errors detected! Check the log file.${NC}"
    fi
}

stress_test() {
    print_section "Stress Test - Child Process Leaks"
    
    if [ "$VALGRIND_AVAILABLE" = false ]; then
        echo -e "${YELLOW}Valgrind bulunamadı, stress test atlanıyor.${NC}"
        return
    fi
    
    echo "Child process leak testi çalıştırılıyor..."
    
    local stress_commands=(
        "ls | grep test | wc -l"
        "echo test | cat | cat | cat | wc -c"
        "find . -name '*.c' | head -5"
        "ps aux | grep minishell | head -1"
        "echo hello && echo world && echo test"
    )
    
    for cmd in "${stress_commands[@]}"; do
        run_test "$cmd" "STRESS: $cmd" true
    done
}

run_all_tests() {
    print_section "Basic Commands"
    for test in "${BASIC_TESTS[@]}"; do
        run_test "$test" "$test" false
    done
    
    print_section "Pipe Tests"
    for test in "${PIPE_TESTS[@]}"; do
        run_test "$test" "$test" true
    done
    
    print_section "Redirection Tests"
    for test in "${REDIRECT_TESTS[@]}"; do
        run_test "$test" "$test" true
    done
    
    print_section "Complex Tests"
    for test in "${COMPLEX_TESTS[@]}"; do
        run_test "$test" "$test" true
    done
}

show_results() {
    echo
    echo -e "${BLUE}=====================================${NC}"
    echo -e "${BLUE}           TEST RESULTS${NC}"
    echo -e "${BLUE}=====================================${NC}"
    echo -e "Total Tests: $TOTAL_TESTS"
    echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
    echo -e "${RED}Failed: $FAILED_TESTS${NC}"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "\n${GREEN}🎉 Tüm testler başarılı!${NC}"
    else
        echo -e "\n${RED}❌ Bazı testler başarısız oldu.${NC}"
        echo -e "Detaylar için $LOGDIR dizinindeki log dosyalarını kontrol edin."
    fi
    
    echo -e "\nLog dosyaları: ${BLUE}$LOGDIR/${NC}"
}

check_memory_summary() {
    print_section "Memory Leak Summary"
    
    if [ "$VALGRIND_AVAILABLE" = false ]; then
        echo -e "${YELLOW}Valgrind bulunamadı, memory leak özeti oluşturulamıyor.${NC}"
        return
    fi
    
    echo "Valgrind loglarını analiz ediliyor..."
    
    local total_leaks=0
    local files_with_leaks=0
    
    for logfile in "$LOGDIR"/valgrind_*.log; do
        if [ -f "$logfile" ]; then
            if ! grep -q "ERROR SUMMARY: 0 errors" "$logfile" || \
               grep -q "definitely lost" "$logfile" || \
               grep -q "indirectly lost" "$logfile"; then
                files_with_leaks=$((files_with_leaks + 1))
                echo -e "${RED}Leak detected in: $(basename "$logfile")${NC}"
                
                # Leak detaylarını göster
                grep "definitely lost\|indirectly lost\|ERROR SUMMARY" "$logfile" | head -3
                echo "---"
            fi
        fi
    done
    
    if [ $files_with_leaks -eq 0 ]; then
        echo -e "${GREEN}✅ Hiçbir memory leak tespit edilmedi!${NC}"
    else
        echo -e "${RED}⚠️  $files_with_leaks dosyada memory leak tespit edildi.${NC}"
    fi
}

# Ana fonksiyon
main() {
    print_header
    
    case "${1:-all}" in
        "basic")
            setup
            print_section "Basic Commands"
            for test in "${BASIC_TESTS[@]}"; do
                run_test "$test" "$test" false
            done
            show_results
            ;;
        "debug")
            setup
            echo -e "${YELLOW}Debug Mode: Testing single command${NC}"
            echo "Minishell path: $MINISHELL"
            echo "Testing: echo hello"
            echo
            echo "Method 1: Pipe input"
            timeout 5s bash -c "echo 'echo hello'; echo 'exit'" | "$MINISHELL"
            echo
            echo "Method 2: File input"
            echo -e "echo hello\nexit" > temp_debug.txt
            timeout 5s "$MINISHELL" < temp_debug.txt
            rm -f temp_debug.txt
            echo
            echo "Method 3: Interactive (you can test manually)"
            "$MINISHELL"
            cleanup
            ;;
        "alternative")
            setup
            echo -e "${YELLOW}Using alternative test method${NC}"
            # run_test yerine run_test_alternative kullan
            alias run_test=run_test_alternative
            run_all_tests
            show_results
            cleanup
            ;;
        "leaks")
            setup
            run_all_tests
            stress_test
            check_memory_summary
            show_results
            cleanup
            ;;
        "interactive")
            setup
            run_interactive_test
            cleanup
            ;;
        "stress")
            setup
            stress_test
            show_results
            cleanup
            ;;
        "all"|*)
            setup
            run_all_tests
            stress_test
            check_memory_summary
            show_results
            cleanup
            ;;
    esac
}

# Script çalıştır
main "$@"