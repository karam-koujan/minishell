/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:25:19 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/24 17:34:09 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "./parser.h"
# include <limits.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/stat.h>
# include "./env.h"

# define PATH "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

typedef struct s_elem
{
	char	**env;
	int		fd_save;
}				t_elem;

t_env	*init_env_list(char **envp, t_gc **gc);
void	init_empty_env(t_env **env, t_gc **gc);
void	handle_shlvl(t_env **env, t_gc **gc);
void	init_pwd(t_env **env, t_gc **gc);
t_env	*create_env_node(char *key, char *value);
void	add_node(t_env **env, t_env *new);
char	*ft_getenv(t_env *env, char *key);
char	**env_list_to_array(t_env *env);
void	exec(t_cmd_table *data, t_env **env, t_gc **gc);
int		is_builtin(char *cmd);
void	exec_builtin(t_simple_cmd **data, t_env **env, t_elem **elem, \
	t_gc **gc);
void	builtin_echo(t_simple_cmd **data);
int		is_n_flage(char *str);
void	builtin_env(t_simple_cmd **data, t_env *env);
void	builtin_cd(t_simple_cmd **data, t_env **env, t_gc **gc, t_elem **elem);
void	builtin_cd_child(t_simple_cmd **data, t_env **env, t_gc **gc, \
	t_elem **elem);
void	update_pwd(t_env **env, char *key, char *value);
void	builtin_pwd(void);
void	builtin_export(t_simple_cmd **data, t_env **env, t_gc **gc);
void	builtin_export_child(t_simple_cmd **data, t_env **env,  t_gc **gc , \
	t_elem **elem);
void	print_export(t_env *env);
int		valid_export(char *str, t_gc **gc);
void	var_set(char *str, t_env **env, t_gc **gc);
void	handle_plus(t_env **env, char **key, char **value, t_gc **gc);
void	builtin_exit(t_simple_cmd **data, t_gc **gc, t_elem **elem);
void	builtin_exit_child(t_simple_cmd **data, t_gc **gc, t_elem **elem);
void	builtin_unset(t_simple_cmd **data, t_env **env, t_gc **gc);
void	exec_cmd(t_simple_cmd **data, t_env *env, t_elem **elem, t_gc **gc);
void	exec_proc(t_simple_cmd **data, t_env *env, t_elem **elem, t_gc **gc);
void	check_exec_cmd(char **cmd, t_elem **elem, t_gc **gc);
int		ft_check_path_cmd(char *cmd);
char	*fet_path(char **env);
char	*ft_found_cmd(char *cmd, char **path);
void	pars_cmd_1(char *cmd, t_elem **elem, t_gc **gc);
void	pars_cmd_2(char **cmd, t_elem **elem, t_gc **gc);
void	pars_cmd_3(char **cmd, t_elem **elem, t_gc **gc);
void	inf_outf_cmd(t_simple_cmd **data, int flag, t_elem **elem, t_gc **gc);
void	in_cas(t_redirection *in, t_elem **elem, t_gc **gc);
void	ou_cas(t_redirection *ou, t_elem **elem, t_gc **gc);
int		in_cas_p(t_redirection *in, t_elem **elem, t_gc **gc);
int		ou_cas_p(t_redirection *ou, t_elem **elem, t_gc **gc);
int		is_directory(char *path);
int		ft_strcmp(const char *s1, const char *s2);
void	pipe_case(t_cmd_table *data, t_env *env, t_elem *elem, t_gc **gc);
void	ft_dup2(int fd_src, int fd_dest, int fd_close);
void	ft_close(int fd);
int		here_doc(char *delimiter, t_elem **elem, t_gc **gc);
void	handle_herdoc(t_cmd_table **data, t_elem *elem, t_gc **gc);
void	close_single_fd(t_simple_cmd	*cmd);
void	close_pipe_fd(t_cmd_table **data);
char	*gene_name_here_doc(void);
int		exit_stat(int value, int action, t_gc **gc, char **env);
void	exit_status(int status);
int		check_redir_in_parent(t_simple_cmd *cmd, t_elem **elem, t_gc **gc);


#endif