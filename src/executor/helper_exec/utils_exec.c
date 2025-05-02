#include "../../../includes/minishell.h"

char *join_env_var(t_env *env)
{
    char *env_var;

    env_var = ft_strjoin(env->key, env->value);
    if(!env_var)
        return (NULL);
    return (env_var);
}

char **env_list_to_array(t_env *env)
{
    t_env	*temp;
	int		i;
    char **env_arr;

	i = 0;
	temp = env;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	env_arr = (char**)malloc((i + 1) * sizeof(char *));
	i = 0;
	while (env)
	{
		env_arr[i] = join_env_var(env);
		env = env->next;
		i++;
	}
	env_arr[i] = NULL;
    return env_arr;
}

void	add_node(t_env **env, t_env *new)
{
	t_env	*l;
	if (!env || !new)
		return ;
	if (*env == NULL)
	{
		*env = new;
	}
	else
	{
		l = *env;
		while (l->next != NULL)
		{
			l = l->next;
		}
		l->next = new;
	}
}

t_env *create_env_node(char *key, char *value)
{
    t_env *node;

    node = malloc(sizeof(t_env));
    if(!node)
        return NULL;
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}