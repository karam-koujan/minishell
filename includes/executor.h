#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "./parser.h"
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "./env.h"

# define PATH "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

t_env *init_env_list(char **envp, t_gc **gc);
void *init_empty_env(t_env **env, t_gc **gc);
void handle_shlvl(t_env **env, t_gc **gc);
void init_pwd(t_env **env, t_gc **gc);
t_env *create_env_node(char *key, char *value);
void	add_node(t_env **env, t_env *new);
char *ft_getenv(t_env *env, char *key);
char **env_list_to_array(t_env *env) ;

void exec(t_cmd_table *data, t_env **env, t_gc **gc);
int is_builtin(char *cmd);
void exec_builtin(t_simple_cmd **data, t_env **env, char **env_array, t_gc **gc);

void builtin_echo(t_simple_cmd **data);
int  is_n_flage(char *str);

void builtin_env(t_simple_cmd **data, t_env *env);

void builtin_cd(t_simple_cmd **data, t_env **env, t_gc **gc);

void builtin_pwd();

void builtin_export(t_simple_cmd **data, t_env **env, t_gc **gc);
void print_export(t_env *env);
int  valid_export(char *str, t_gc **gc);
void var_set(char *str, t_env **env, t_gc **gc);
void	handle_plus(t_env **env, char **key, char **value, t_gc **gc);

void builtin_exit(t_simple_cmd **data);

void  builtin_unset(t_simple_cmd **data, t_env **env);

void exec_cmd(t_simple_cmd **data, t_env *env, char **env_arr);
void exec_proc(t_simple_cmd **data, t_env *env, char **env_arr);
void	check_exec_cmd(char **cmd, char **env);
int	ft_check_path_cmd(char *cmd);
char	*fet_path(char **env);
char	*ft_found_cmd(char *cmd, char **path);
void	pars_cmd_1(char *cmd);
void	pars_cmd_2(char **cmd, char **env);
void	pars_cmd_3(char **cmd, char **env);

int inf_outf_cmd(t_simple_cmd **data, int flag);
void  in_cas(t_redirection *in);
void ou_cas(t_redirection *ou);
int in_cas_p(t_redirection *in);
int	ou_cas_p(t_redirection *ou);
int is_directory(char *path);
int ft_strcmp(const char *s1, const char *s2);

void pipe_case(t_cmd_table *data, t_env *env, char ** env_arr);
void	ft_dup2(int fd_src, int fd_dest, int fd_close);
void	ft_close(int fd);

int	here_doc(char *delimiter);

int  exit_stat(int value, int action);
void exit_status(int status);
#endif