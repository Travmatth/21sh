/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion_actions.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 17:05:04 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/05 12:33:17 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

int		substitute_word(char **parameter, char *param[3])
{
	int		status;
	char	*expanded;

	expanded = NULL;
	if (!OK((status = sub_expansion(&expanded, param[FULL_PARAM]))))
		return (status);
	free(*parameter);
	*parameter = expanded;
	return (SUCCESS);
}

int		substitute_parameter(char **parameter, char *env_lookup)
{
	char	*tmp;

	if (!(tmp = ft_strdup(env_lookup)))
		return (ERROR);
	free(*parameter);
	*parameter = tmp;
	return (SUCCESS);
}

int		substitute_null(char **parameter)
{
	char	*tmp;

	if (!(tmp = ft_strnew(0)))
		return (ERROR);
	free(*parameter);
	*parameter = tmp;
	return (SUCCESS);
}

int		assign_word(char **parameter, char *param[3])
{
	char	*tmp;

	if (!(tmp = ft_strdup(param[WORD])))
		return (ERROR);
	set_env_var(param[NAME], param[WORD]);
	free(*parameter);
	*parameter = tmp;
	return (SUCCESS);
}

int		error_exit(char **parameter, char *param[3])
{
	(void)parameter;
	ft_printf(ERR_UNSET_PARAM, param[NAME]);
	return (NIL);
}
