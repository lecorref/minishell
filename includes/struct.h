typedef struct s_command {
    char *command;
    int fd[2];
} t_command;


typedef struct s_env {
    char *key;
    char *value;
} t_env;
