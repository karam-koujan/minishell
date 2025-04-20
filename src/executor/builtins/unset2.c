#include  "../../../includes/minishell.h"



int valide_unset(char *str)
{
    if (!str || !str[0])
        return (0);
    if (!isalpha(str[0]) && str[0] != '_')
        return (0);
    i = 1;
    while (str[i])
    {
        if (!isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return 1;
}