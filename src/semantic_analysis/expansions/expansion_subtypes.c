/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_subtypes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:46:08 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/28 18:45:29 by tmatthew         ###   ########.fr       */
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

int		plain_param_expansion(char **parameter, char *var, size_t *i)
{
	char	*tmp;
	char	*env_lookup;
	char	*param[3];
	int		position;
	size_t	len[4];
	int		quoted;

	if (ERR(init_param_state(param, len, NIL, var))
		|| ERR(create_param_state(param, len)))
		return (ERROR);
	if ((quoted = param[NAME][0] == '"' ? TRUE : FALSE))
	{
		if (!(tmp = ft_stripquote(param[NAME])))
			return (ERROR);
		free(param[NAME]);
		param[NAME] = tmp;
	}
	if (OK(ft_safeatoi(param[NAME], &position)))
	{
		ft_printf("Semantic Error: Positional Parameters not implemented");
		return (NIL);
	}
	else if (!(env_lookup = get_env_var(param[NAME])))
	{
		ft_printf("Semantic Error: bad substitution");
		return (NIL);
	}
	if (!(tmp = ft_strdup(env_lookup ? env_lookup : "")))
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
	*i = *i + len[FULL_PARAM];
	ft_freearr(param, FALSE);
	return (SUCCESS);
}

/*
** ${parameter:-[word]}
** Use Default Values. If parameter is unset or null, the expansion of word (or
** an empty string if word is omitted) shall be substituted; otherwise, the
** value of parameter shall be substituted.
** In each case that a value of word is needed as described below), word shall
** be subjected to tilde expansion, parameter expansion, command substitution,
** and arithmetic expansion. If word is not needed, it shall not be expanded.
** The '}' character that delimits the following parameter expansion
** modifications shall be determined as described previously in this section
** and in Double-Quotes.
** In the parameter expansions shown previously, use of the <colon> in the
** format shall result in a test for a parameter that is unset or null;
** omission of the <colon> shall result in a test for a parameter that is only
** unset. If parameter is '#' and the colon is omitted, the application shall
** ensure that word is specified (this is necessary to avoid ambiguity with the
** string length expansion).In all cases shown with "substitute", the expression
** is replaced with the value shown. In all cases shown with "assign", parameter
** is assigned that value, which also replaces the expression.
*/

int		use_defaults_param_expansion(char **parameter, char *var, size_t *i)
{
	int		quoted;
	int		status;
	char	*env_lookup;
	char	*param[3];
	size_t	len[4];

	if (ERR(init_param_state(param, len, '-', var))
		|| ERR(create_param_state(param, len)))
		return (ERROR);
	if ((quoted = param[NAME][0] == '"' ? TRUE : FALSE))
	{
		if (!(env_lookup = ft_stripquote(param[NAME])))
			return (ERROR);
		free(param[NAME]);
		param[NAME] = env_lookup;
	}
	env_lookup = get_env_var(param[NAME]);
	if (!env_lookup)
		status = substitute_word(parameter, param);
	else if (env_lookup[0])
		status = substitute_parameter(parameter, env_lookup);
	else if (!env_lookup[0] && len[TEST])
		status = substitute_word(parameter, param);
	else if (!env_lookup[0] && !len[TEST])
		status = substitute_null(parameter);
	else
		status = ERROR;
	if (quoted)
	{
		if (!(env_lookup = ft_quotestr(*parameter, '"')))
			return (ERROR);
		free(*parameter);
		*parameter = env_lookup;
	}
	*i = *i + len[FULL_PARAM];
	ft_freearr(param, FALSE);
	return (status);
}

/*
** ${parameter:=[word]}
** Assign Default Values. If parameter is unset or null, the expansion of word
** (or an empty string if word is omitted) shall be assigned to parameter. In
** all cases, the final value of parameter shall be substituted. Only variables,
** not positional parameters or special parameters, can be assigned in this way.
** In each case that a value of word is needed as described below), word shall
** be subjected to tilde expansion, parameter expansion, command substitution,
** and arithmetic expansion. If word is not needed, it shall not be expanded.
** The '}' character that delimits the following parameter expansion
** modifications shall be determined as described previously in this section
** In the parameter expansions shown previously, use of the <colon> in the
** format shall result in a test for a parameter that is unset or null;
** omission of the <colon> shall result in a test for a parameter that is only
** unset. If parameter is '#' and the colon is omitted, the application shall
** ensure that word is specified (this is necessary to avoid ambiguity with the
** string length expansion).In all cases shown with "substitute", the expression
** is replaced with the value shown. In all cases shown with "assign", parameter
** is assigned that value, which also replaces the expression.
** and in Double-Quotes.
*/

int		assign_defaults_param_expansion(char **parameter, char *var, size_t *i)
{
	int		status;
	char	*env_lookup;
	char	*param[3];
	size_t	len[4];
	int		quoted;

	if (ERR(init_param_state(param, len, '-', var))
		|| ERR(create_param_state(param, len)))
		return (ERROR);
	if ((quoted = param[NAME][0] == '"' ? TRUE : FALSE))
	{
		if (!(env_lookup = ft_stripquote(param[NAME])))
			return (ERROR);
		free(param[NAME]);
		param[NAME] = env_lookup;
	}
	env_lookup = get_env_var(param[NAME]);
	if (!env_lookup)
		status = assign_word(parameter, param);
	else if (env_lookup[0])
		status = substitute_parameter(parameter, env_lookup);
	else if (!env_lookup[0] && len[TEST])
		status = assign_word(parameter, param);
	else if (!env_lookup[0] && !len[TEST])
		status = substitute_null(parameter);
	else
		status = ERROR;
	if (quoted)
	{
		if (!(env_lookup = ft_quotestr(*parameter, '"')))
			return (ERROR);
		free(*parameter);
		*parameter = env_lookup;
	}
	*i = *i + len[FULL_PARAM];
	ft_freearr(param, FALSE);
	return (status);
}

/*
** ${parameter:?[word]}
** Indicate Error if Null or Unset. If parameter is unset or null, the expansion
** of word (or a message indicating it is unset if word is omitted) shall be
** written to standard error and the shell exits with a non-zero exit status.
** Otherwise, the value of parameter shall be substituted. An interactive shell
** need not exit.
** In each case that a value of word is needed as described below), word shall
** be subjected to tilde expansion, parameter expansion, command substitution,
** and arithmetic expansion. If word is not needed, it shall not be expanded.
** The '}' character that delimits the following parameter expansion
** modifications shall be determined as described previously in this section
** and in Double-Quotes.
** In the parameter expansions shown previously, use of the <colon> in the
** format shall result in a test for a parameter that is unset or null;
** omission of the <colon> shall result in a test for a parameter that is only
** unset. If parameter is '#' and the colon is omitted, the application shall
** ensure that word is specified (this is necessary to avoid ambiguity with the
** string length expansion).In all cases shown with "substitute", the expression
** is replaced with the value shown. In all cases shown with "assign", parameter
** is assigned that value, which also replaces the expression.
*/

int		error_unset_param_expansion(char **parameter, char *var, size_t *i)
{
	int		status;
	char	*env_lookup;
	char	*param[3];
	size_t	len[4];
	int		quoted;

	if (ERR(init_param_state(param, len, '-', var))
		|| ERR(create_param_state(param, len)))
		return (ERROR);
	if ((quoted = param[NAME][0] == '"' ? TRUE : FALSE))
	{
		if (!(env_lookup = ft_stripquote(param[NAME])))
			return (ERROR);
		free(param[NAME]);
		param[NAME] = env_lookup;
	}
	env_lookup = get_env_var(param[NAME]);
	if (!env_lookup)
		status = error_exit(parameter, param);
	else if (env_lookup[0])
		status = substitute_parameter(parameter, env_lookup);
	else if (!env_lookup[0] && len[TEST])
		status = assign_word(parameter, param);
	else if (!env_lookup[0] && !len[TEST])
		status = substitute_null(parameter);
	else
		status = ERROR;
	if (quoted)
	{
		if (!(env_lookup = ft_quotestr(*parameter, '"')))
			return (ERROR);
		free(*parameter);
		*parameter = env_lookup;
	}
	*i = *i + len[FULL_PARAM];
	ft_freearr(param, FALSE);
	return (status);
}

/*
** ${parameter:+[word]}
** Use Alternative Value. If parameter is unset or null, null shall be
** substituted; otherwise, the expansion of word (or an empty string if word is
** omitted) shall be substituted.
** In each case that a value of word is needed as described below), word shall
** be subjected to tilde expansion, parameter expansion, command substitution,
** and arithmetic expansion. If word is not needed, it shall not be expanded.
** The '}' character that delimits the following parameter expansion
** modifications shall be determined as described previously in this section
** and in Double-Quotes.
** In the parameter expansions shown previously, use of the <colon> in the
** format shall result in a test for a parameter that is unset or null;
** omission of the <colon> shall result in a test for a parameter that is only
** unset. If parameter is '#' and the colon is omitted, the application shall
** ensure that word is specified (this is necessary to avoid ambiguity with the
** string length expansion).In all cases shown with "substitute", the expression
** is replaced with the value shown. In all cases shown with "assign", parameter
** is assigned that value, which also replaces the expression.
*/

int		alternative_param_expansion(char **parameter, char *var, size_t *i)
{
	int		status;
	char	*env_lookup;
	char	*param[3];
	size_t	len[4];
	int		quoted;

	if (ERR(init_param_state(param, len, '-', var))
		|| ERR(create_param_state(param, len)))
		return (ERROR);
	if ((quoted = param[NAME][0] == '"' ? TRUE : FALSE))
	{
		if (!(env_lookup = ft_stripquote(param[NAME])))
			return (ERROR);
		free(param[NAME]);
		param[NAME] = env_lookup;
	}
	env_lookup = get_env_var(param[NAME]);
	if (!env_lookup)
		status = substitute_null(parameter);
	else if (env_lookup[0])
		status = substitute_word(parameter, param);
	else if (!env_lookup[0] && len[TEST])
		status = substitute_null(parameter);
	else if (!env_lookup[0] && !len[TEST])
		status = substitute_word(parameter, param);
	else
		status = NIL;
	if (quoted)
	{
		if (!(env_lookup = ft_quotestr(*parameter, '"')))
			return (ERROR);
		free(*parameter);
		*parameter = env_lookup;
	}
	*i = *i + len[FULL_PARAM];
	ft_freearr(param, FALSE);
	return (status);
}
