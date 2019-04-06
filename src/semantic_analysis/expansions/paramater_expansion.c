/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paramater_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:46:08 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/05 12:26:37 by tmatthew         ###   ########.fr       */
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

int		plain_param_expansion(char **parameter, char *var, size_t *skip)
{
	char	*new;
	char	*replacement;
	char	*env_lookup;
	char	*param[3];
	size_t	len[4];

	if (ERR(init_param_state(param, len, NIL, var))
		|| ERR(create_param_state(param, len)))
		return (ERROR);
	env_lookup = get_env_var(param[NAME]);
	new = env_lookup ? env_lookup : "";
	if (!(replacement = ft_swap(*parameter, param[FULL_PARAM], new)))
		return (ERROR);
	free(*parameter);
	*parameter = replacement;
	*skip = *skip + len[FULL_PARAM];
	ft_freearr(param);
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
	int		status;
	char	*env_lookup;
	char	*param[3];
	size_t	len[4];

	if (ERR(init_param_state(param, len, '-', var))
		|| ERR(create_param_state(param, len)))
		return (ERROR);
	env_lookup = get_env_var(param[NAME]);
	if (!env_lookup)
		status = substitute_word(parameter, param);
	else if (env_lookup[0])
		status = substitute_parameter(parameter, param, env_lookup);
	else if (!env_lookup[0] && len[TEST])
		status = substitute_word(parameter, param);
	else if (!env_lookup[0] && !len[TEST])
		status = substitute_null(parameter, param);
	else
		status = ERROR;
	*i = *i + len[FULL_PARAM];
	ft_freearr(param);
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

	if (ERR(init_param_state(param, len, '-', var))
		|| ERR(create_param_state(param, len)))
		return (ERROR);
	env_lookup = get_env_var(param[NAME]);
	if (!env_lookup)
		status = assign_word(parameter, param);
	else if (env_lookup[0])
		status = substitute_parameter(parameter, param, env_lookup);
	else if (!env_lookup[0] && len[TEST])
		status = assign_word(parameter, param);
	else if (!env_lookup[0] && !len[TEST])
		status = substitute_null(parameter, param);
	else
		status = ERROR;
	*i = *i + len[FULL_PARAM];
	ft_freearr(param);
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

	if (ERR(init_param_state(param, len, '-', var))
		|| ERR(create_param_state(param, len)))
		return (ERROR);
	env_lookup = get_env_var(param[NAME]);
	if (!env_lookup)
		status = error_exit(parameter, param);
	else if (env_lookup[0])
		status = substitute_parameter(parameter, param, env_lookup);
	else if (!env_lookup[0] && len[TEST])
		status = assign_word(parameter, param);
	else if (!env_lookup[0] && !len[TEST])
		status = substitute_null(parameter, param);
	else
		status = ERROR;
	*i = *i + len[FULL_PARAM];
	ft_freearr(param);
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

	if (ERR(init_param_state(param, len, '-', var))
		|| ERR(create_param_state(param, len)))
		return (ERROR);
	env_lookup = get_env_var(param[NAME]);
	if (!env_lookup)
		status = substitute_null(parameter, param);
	else if (env_lookup[0])
		status = substitute_word(parameter, param);
	else if (!env_lookup[0] && len[TEST])
		status = substitute_null(parameter, param);
	else if (!env_lookup[0] && !len[TEST])
		status = substitute_word(parameter, param);
	else
		status = NIL;
	*i = *i + len[FULL_PARAM];
	ft_freearr(param);
	return (status);
}
