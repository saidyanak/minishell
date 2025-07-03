/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:39:59 by yuocak            #+#    #+#             */
/*   Updated: 2025/07/03 15:45:41 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ========================================================================== */
/*                                 📚 INCLUDES                                */
/* ========================================================================== */

# include "libft/libft.h"
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/* ========================================================================== */
/*                              🏷️  ENUMERATIONS                              */
/* ========================================================================== */

/*
 * 🔤 Quote types for parsing
 * Used to track what type of quotes are being processed
 */
typedef enum e_quote_type
{
	DOUBLE_QUOTE, /* 📝 Double quotes: "text" */
	SINGLE_QUOTE, /* 📝 Single quotes: 'text' */
	NONE_QUOTE    /* 📝 No quotes: text */
}		t_quote_type;

/*
 * 🎯 Token types for command parsing
 * Represents different elements in shell commands
 */
typedef enum e_token_type
{
	TOKEN_WORD,         /* 📄 Regular word/command */
	TOKEN_PIPE,         /* 🔗 Pipe: "|" */
	TOKEN_REDIRECT_IN,  /* 📥 Input redirect: "<" */
	TOKEN_REDIRECT_OUT, /* 📤 Output redirect: ">" */
	TOKEN_APPEND,       /* ➕ Append redirect: ">>" */
	TOKEN_HEREDOC,      /* 📜 Here document: "<<" */
	TOKEN_QUOTED_WORD   /* 🔠 Quoted word */
}		t_token_type;

/* ========================================================================== */
/*                              📦 STRUCTURES                                */
/* ========================================================================== */

/*
 * 🔗 Token structure - Linked list of command tokens
 * Each token represents a word, operator, or special character
 */
typedef struct s_token
{
	char *content;        /* 📝 The actual text content */
	t_token_type type;    /* 🏷️  Type of token */
	struct s_token *next; /* ➡️  Next token in the list */
	t_quote_type q_type;  /* 🔤 Quote type if applicable */
}		t_token;

/*
 * 🌍 Environment variable structure
 * Stores environment variables as key-value pairs
 */
typedef struct s_env
{
	char *key;          /* 🔑 Variable name (e.g., "PATH") */
	char *value;        /* 💎 Variable value (e.g., "/bin:/usr/bin") */
	int exported;       /* 🌐 Whether variable is exported */
	struct s_env *next; /* ➡️  Next environment variable */
}		t_env;

/*
 * 🏗️  Main program structure
 * Contains all the essential data for the minishell
 */
typedef struct s_base
{
	t_token *token;  /* 📋 List of parsed tokens */
	t_env *env;      /* 🌍 Environment variables */
	int exit_status; /* 🚪 Last command exit status */
}		t_base;

/* ========================================================================== */
/*                           🔧 FUNCTION PROTOTYPES                          */
/* ========================================================================== */

/* ─────────────────── 🎯 TOKENIZER FUNCTIONS ─────────────────── */
/*
 * 📝 Parse input string and convert to tokens
 */
char	*parse_word_with_quotes(char *input, int *i, t_token_type *type);
void	tokenize_input(char *input, t_base *base);
void	add_token(t_token **head, char *str, t_token_type type);
int		is_special(char c);

// Expand functions
void	expand_tokens(t_base *base);

// Environment functions

/* ─────────────────── 🌍 ENVIRONMENT FUNCTIONS ─────────────────── */
/*
 * 🌱 Environment variable management
 */

t_env	*create_new_node(char *env);
void	add_new_node(t_env **head, t_env *new_node);
t_env	*init_env(char **env);
char	*get_env_value(t_base base, char *key);

/* ─────────────────── 🛠️  UTILITY FUNCTIONS ─────────────────── */
/*
 * 🔧 Helper functions for string manipulation and utilities
 */
char	*ft_strdup(const char *s1);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *s, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_isspace(char c);
char	*ft_strjoin_free(char *s1, char *s2);

/* ─────────────────── 🧹 CLEANUP FUNCTIONS ─────────────────── */
/*
 * 🗑️  Memory management and cleanup
 */
void	free_tokens(t_token *tokens);
void	free_env(t_env *env);
void	cleanup_base(t_base *base);

/* ─────────────────── ⚙️  EXECUTION FUNCTIONS ─────────────────── */
/*
 * 🚀 Command execution and process management
 */
void	ft_build_in(t_token *current_prompt, t_base *base);
void	execute_command(t_base *base);

/* ─────────────────── 🐛 DEBUG FUNCTIONS ─────────────────── */
/*
 * 🔍 Development and debugging utilities
 */
void	print_tokens(t_token *token);
void	debug_parse_quotes(char *input);

/* ─────────────────── 🏠 BUILT-IN COMMANDS ─────────────────── */
/*
 * 🎮 Shell built-in commands implementation
 */
int		ft_echo(t_token *current_prompt, t_base base);
t_base	*ft_cd(t_token *current_prompt, t_base *base);
int		ft_pwd(t_base base);
t_base	*ft_export(t_token *token, t_base *base);
int		ft_env(t_base *base);
t_base	*ft_unset(t_token *token, t_base *base);
void	ft_exit(t_token *current_prompt, t_base *base);

#endif
