/* 
🎯 MINISHELL EXECUTION ARCHITECTURE GUIDE
=======================================

1️⃣ SINGLE COMMAND EXECUTION (Şu anki durum)
├── buildin mi kontrol et
├── buildin ise: ft_build_in() çağır
└── değilse: external command çalıştır
    ├── PATH'te ara
    ├── fork() yap
    ├── child'da execve() çağır
    └── parent'ta wait() yap

2️⃣ MULTIPLE COMMAND EXECUTION (Gelecek)
├── pipe var mı kontrol et
├── her komut için ayrı process
├── pipe setup yap
└── paralel execution

3️⃣ REDIRECTION HANDLING
├── input redirection (<)
├── output redirection (>)
├── append redirection (>>)
└── heredoc (<<)
*/





execute_command()
├── is_single_execute() ? 
│   ├── YES → single_execute_command()
│   │   ├── builtin? → ft_build_in() (fork yok)
│   │   └── external → execute_external_simple() (fork var)
│   └── NO → multiple_execute_command()
│       ├── has_pipe() → execute_pipeline()
│       ├── has_background() → execute_background()  
│       └── sequential → execute_sequential()





static void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static char	*find_command_path(char *command, t_base *base)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	// Mutlak yol mu kontrol et
	if (access(command, F_OK) == 0)
		return (ft_strdup(command));
	// PATH environment'ı al
	path_env = get_env_value(*base, "PATH");
	if (!path_env)
		return (NULL);
	// PATH'i parçala
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	// Her PATH'te ara
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, command);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			// paths array'ini temizle
			free_argv(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	// paths array'ini temizle
	free_argv(paths);
	return (NULL);
}

// Token'ları argv array'ine çevir
static char	**tokens_to_argv(t_token *token)
{
	t_token	*current;
	char	**argv;
	int		count;
	int		i;

	// Token sayısını say
	count = 0;
	current = token;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
			count++;
		current = current->next;
	}
	// argv array'i oluştur
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	// Token'ları kopyala
	i = 0;
	current = token;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_QUOTED_WORD)
		{
			argv[i] = ft_strdup(current->content);
			i++;
		}
		current = current->next;
	}
	argv[i] = NULL;
	return (argv);
}

// Environment'ı execve için char** formatına çevir
static char	**env_to_envp(t_env *env)
{
	t_env	*current;
	char	**envp;
	char	*temp;
	int		count;
	int		i;

	// Environment variable sayısını say
	count = 0;
	current = env;
	while (current)
	{
		if (current->exported)
			count++;
		current = current->next;
	}
	// envp array'i oluştur
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	// Environment variables'ı kopyala
	i = 0;
	current = env;
	while (current)
	{
		if (current->exported)
		{
			temp = ft_strjoin(current->key, "=");
			envp[i] = ft_strjoin(temp, current->value);
			free(temp);
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}

// envp array'ini temizle
static void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

// External command execution (fork + execve)
static int	execute_external_command(t_token *token, t_base *base)
{
	char	*command_path;
	char	**argv;
	char	**envp;
	pid_t	pid;
	int		status;

	// PATH'te komutu ara
	command_path = find_command_path(token->content, base);
	if (!command_path)
	{
		printf("minishell: %s: command not found\n", token->content);
		base->exit_status = 127;
		return (127);
	}
	// argv array'i oluştur
	argv = tokens_to_argv(token);
	if (!argv)
	{
		free(command_path);
		return (1);
	}
	// envp array'ini oluştur
	envp = env_to_envp(base->env);
	if (!envp)
	{
		free(command_path);
		free_argv(argv);
		return (1);
	}
	// Fork yap
	pid = fork();
	if (pid == 0)
	{
		// Child process
		execve(command_path, argv, envp); // Burada execve çağrısı!
		perror("execve failed");
		exit(127);
	}
	else if (pid > 0)
	{
		// Parent process
		waitpid(pid, &status, 0);
		base->exit_status = WEXITSTATUS(status);
	}
	else
	{
		perror("fork failed");
		base->exit_status = 1;
	}
	// Cleanup
	free(command_path);
	free_argv(argv);
	free_envp(envp);
	return (base->exit_status);
}