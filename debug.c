#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

// Mevcut token debug fonksiyonu
void    print_tokens(t_token *token)
{
    t_token *head;
    int i = 0;

    head = token;
    printf("=== TOKEN DEBUG ===\n");
    while (head)
    {
        printf("Token[%d]: content:'%s', type:%d, q_type:%d\n", 
               i++, head->content ? head->content : "(null)", head->type, head->q_type);
        head = head->next;
    }
    printf("===================\n\n");
}

// Environment variables debug
void    print_env(t_env *env)
{
    t_env *head;
    int i = 0;

    head = env;
    printf("=== ENV DEBUG ===\n");
    while (head)
    {
        printf("Env[%d]: key:'%s', value:'%s', exported:%d\n", 
               i++, head->key ? head->key : "(null)", 
               head->value ? head->value : "(null)", head->exported);
        head = head->next;
    }
    printf("==================\n\n");
}

// Execution data debug
void    print_exec_data(t_exec_data *data)
{
    printf("=== EXEC DATA DEBUG ===\n");
    if (!data)
    {
        printf("Exec data is NULL\n");
        printf("========================\n\n");
        return;
    }
    
    printf("Command count: %d\n", data->cmd_count);
    printf("Pipe count: %d\n", data->pipe_count);
    printf("PIDs allocated: %s\n", data->pids ? "Yes" : "No");
    printf("Pipes allocated: %s\n", data->pipes ? "Yes" : "No");
    printf("Commands allocated: %s\n", data->commands ? "Yes" : "No");
    
    // Her komut için token'ları yazdır
    if (data->commands && data->cmd_count > 0)
    {
        printf("\n--- Commands breakdown ---\n");
        for (int i = 0; i < data->cmd_count; i++)
        {
            printf("Command[%d]:\n", i);
            print_tokens(data->commands[i]);
        }
    }
    
    // PIDs yazdır
    if (data->pids && data->cmd_count > 0)
    {
        printf("--- Process IDs ---\n");
        for (int i = 0; i < data->cmd_count; i++)
        {
            printf("PID[%d]: %d\n", i, data->pids[i]);
        }
    }
    
    printf("========================\n\n");
}

// Heredoc info debug
void    print_heredoc_info(t_heredoc_info *heredocs)
{
    t_heredoc_info *head;
    int i = 0;

    head = heredocs;
    printf("=== HEREDOC DEBUG ===\n");
    while (head)
    {
        printf("Heredoc[%d]: temp_filename:'%s', original_delimiter:'%s', heredoc_id:%d\n",
               i++, head->temp_filename ? head->temp_filename : "(null)",
               head->original_delimiter ? head->original_delimiter : "(null)",
               head->heredoc_id);
        head = head->next;
    }
    printf("======================\n\n");
}

// Base struct debug
void    print_base(t_base *base)
{
    printf("=== BASE DEBUG ===\n");
    if (!base)
    {
        printf("Base is NULL\n");
        printf("===================\n\n");
        return;
    }
    
    printf("Exit status: %d\n", base->exit_status);
    printf("Heredoc count: %d\n", base->heredoc_count);
    printf("===================\n\n");
    
    // Alt struct'ları yazdır
    print_tokens(base->token);
    //print_env(base->env);
    print_heredoc_info(base->heredocs);
    
    if (base->data)
        print_exec_data(base->data);
}

// Execution parameters debug
void    print_exec_params(t_exec_params *params)
{
    printf("=== EXEC PARAMS DEBUG ===\n");
    if (!params)
    {
        printf("Exec params is NULL\n");
        printf("==========================\n\n");
        return;
    }
    
    printf("Command path: '%s'\n", params->command_path ? params->command_path : "(null)");
    
    // ARGV yazdır
    printf("--- ARGV ---\n");
    if (params->argv)
    {
        for (int i = 0; params->argv[i]; i++)
            printf("argv[%d]: '%s'\n", i, params->argv[i]);
    }
    else
        printf("ARGV is NULL\n");
    
    // ENVP yazdır (sadece ilk birkaç tanesi)
    printf("--- ENVP (first 10) ---\n");
    if (params->envp)
    {
        for (int i = 0; params->envp[i] && i < 10; i++)
            printf("envp[%d]: '%s'\n", i, params->envp[i]);
        if (params->envp[10])
            printf("... and more\n");
    }
    else
        printf("ENVP is NULL\n");
    
    printf("==========================\n\n");
}

// Shell signal debug
void    print_shell_signal(t_shell_signal *sig)
{
    printf("=== SHELL SIGNAL DEBUG ===\n");
    if (!sig)
    {
        printf("Shell signal is NULL\n");
        printf("===========================\n\n");
        return;
    }
    
    printf("Shell level: %d\n", sig->shell_level);
    printf("In heredoc: %d\n", sig->in_heredoc);
    printf("In child: %d\n", sig->in_child);
    printf("Last exit status: %d\n", sig->last_exit_status);
    printf("===========================\n\n");
}

// Heredoc data debug
void    print_heredoc_data(t_heredoc_data *data)
{
    printf("=== HEREDOC DATA DEBUG ===\n");
    if (!data)
    {
        printf("Heredoc data is NULL\n");
        printf("===========================\n\n");
        return;
    }
    
    printf("Should expand: %d\n", data->should_expand);
    printf("Clean delimiter: '%s'\n", data->clean_delimiter ? data->clean_delimiter : "(null)");
    printf("Line count: %d\n", data->line_count);
    printf("Pipe fd: %d\n", data->pipefd);
    printf("Base pointer: %p\n", (void*)data->base);
    printf("===========================\n\n");
}

// Parse data debug
void    print_parse_data(t_parse_data *data)
{
    printf("=== PARSE DATA DEBUG ===\n");
    if (!data)
    {
        printf("Parse data is NULL\n");
        printf("==========================\n\n");
        return;
    }
    
    printf("Quoted: %d\n", data->quoted);
    printf("Detected quote type: %d (0:DOUBLE, 1:SINGLE, 2:NONE)\n", data->detected);
    printf("==========================\n\n");
}

// Tüm debug bilgilerini yazdıran ana fonksiyon
void    debug_all(t_base *base)
{
    printf("\n##########################################\n");
    printf("#          FULL DEBUG OUTPUT            #\n");
    printf("##########################################\n\n");
    
    print_base(base);
    
    printf("##########################################\n");
    printf("#            DEBUG COMPLETE             #\n");
    printf("##########################################\n\n");
}

// Enum değerlerini string'e çeviren yardımcı fonksiyonlar
const char* token_type_to_string(t_token_type type)
{
    switch(type)
    {
        case TOKEN_WORD: return "WORD";
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_REDIRECT_IN: return "REDIRECT_IN";
        case TOKEN_REDIRECT_OUT: return "REDIRECT_OUT";
        case TOKEN_APPEND: return "APPEND";
        case TOKEN_HEREDOC: return "HEREDOC";
        case TOKEN_QUOTED_WORD: return "QUOTED_WORD";
        case TOKEN_ENV_VAR: return "ENV_VAR";
        default: return "UNKNOWN";
    }
}

const char* quote_type_to_string(t_quote_type type)
{
    switch(type)
    {
        case DOUBLE_QUOTE: return "DOUBLE";
        case SINGLE_QUOTE: return "SINGLE";
        case NONE_QUOTE: return "NONE";
        default: return "UNKNOWN";
    }
}

// Daha okunabilir token debug fonksiyonu
void    print_tokens_readable(t_token *token)
{
    t_token *head;
    int i = 0;

    head = token;
    printf("=== READABLE TOKEN DEBUG ===\n");
    while (head)
    {
        printf("Token[%d]:\n", i++);
        printf("  Content: '%s'\n", head->content ? head->content : "(null)");
        printf("  Type: %s\n", token_type_to_string(head->type));
        printf("  Quote: %s\n", quote_type_to_string(head->q_type));
        printf("  Next: %s\n", head->next ? "exists" : "null");
        printf("---\n");
        head = head->next;
    }
    printf("=============================\n\n");
}