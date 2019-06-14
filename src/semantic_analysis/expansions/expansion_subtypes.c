/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_subtypes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:46:08 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/13 16:48:15 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

/*
** ${parameter}
** The value, if any, of parameter shall be substituted.
** The parameter name or symbol can be enclosed in braces, which are optional
** except for positional parameters with more than one digit or when parameter
** is a name and is followed by a character that could be interpreted as part
** of the name. The matching closing brace shall be determined by counting
** brace levels, skipping over enclosed quoted strings, and command
** substitutions.
*/

int		swap_param(char **parameter, char *param[3], int quoted)
{
	char	*tmp;
	char	*env_lookup;

	if (!(env_lookup = get_env_var(param[NAME])))
		env_lookup = "\n";
	if (!(tmp = ft_strdup(env_lookup)))
		return (ERROR);
	free(*parameter);
	if (quoted)
	{
		if (!(*parameter = ft_quotestr(tmp, '"')))
			return (ERROR);
		free(tmp);
	}
	else
		*parameter = tmp;
	ft_freearr(param, FALSE);
	return (SUCCESS);
}

int		positional_param(char *param[3])
{
	ft_freearr(param, FALSE);
	ft_putendl("Semantic Error: Positional Parameters not implemented");
	return (NIL);
}

int		plain_param_expansion(char **parameter, char *var, size_t *i)
{
	char	*param[3];
	int		position;
	size_t	len[4];
	int		quoted;
	char	*tmp;

	if (ERR(init_param(param, len, NIL, var)) || ERR(create_param(param, len)))
		return (ERROR);
	if (len[NAME] == 0)
	{
		ft_freearr(param, FALSE);
		ft_putendl("Semantic Error: bad substitution");
		return (NIL);
	}
	if ((quoted = param[NAME][0] == '"' ? TRUE : FALSE))
	{
		if (!(tmp = ft_stripquote(param[NAME])))
			return (ERROR);
		free(param[NAME]);
		param[NAME] = tmp;
	}
	if (OK(ft_safeatoi(param[NAME], &position)))
		return (positional_param(param));
	*i = *i + len[FULL_PARAM];
	return (swap_param(parameter, param, quoted));
}
