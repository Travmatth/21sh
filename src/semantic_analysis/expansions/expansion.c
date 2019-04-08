/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 17:03:30 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/07 17:43:12 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

/*
** Parameter Expansion
** The format for parameter expansion is as follows:
**
** ${expression}
** where expression consists of all characters until the matching '}'. Any '}'
** escaped by a <backslash> or within a quoted string, and characters in
** embedded arithmetic expansions, command substitutions, and variable
** expansions, shall not be examined in determining the matching '}'.
** If the parameter is not enclosed in braces, and is a name, the expansion
** shall use the longest valid name (see XBD Name), whether or not the variable
** represented by that name exists. Otherwise, the parameter is a
** single-character symbol, and behavior is unspecified if that character is
** neither a digit nor one of the special parameters (see Special Parameters).
**
** If a parameter expansion occurs inside double-quotes:
** Pathname expansion shall not be performed on the results of the expansion.
** Field splitting shall not be performed on the results of the expansion.
*/

int		parameter_expansion(char **parameter)
{
	size_t	i;
	char	*new;
	char	*param;
	int		status;

	if (!(param = ft_strdup(*parameter))
		|| !(new = ft_strdup(*parameter)))
		return (ERROR);
	i = 0;
	status = SUCCESS;
	while (OK(status) && (param = ft_strchr(&param[i], '$')))
	{
		if (!ft_strncmp("$(", param, 2))
			status = skip_parens(param, &i);
		else if (enclosed(param, '-'))
			status = use_defaults_param_expansion(&new, param, &i);
		else if (enclosed(param, '='))
			status = assign_defaults_param_expansion(&new, param, &i);
		else if (enclosed(param, '?'))
			status = error_unset_param_expansion(&new, param, &i);
		else if (enclosed(param, '+'))
			status = alternative_param_expansion(&new, param, &i);
		else
			status = plain_param_expansion(&new, param, &i);
	}
	free(param);
	return (status);
}

/*
** Not all expansions are performed on every word, as explained in the following
** sections.
** The order of word expansion shall be as follows:
** 	Tilde expansion (see Tilde Expansion),
** 	parameter expansion (see Parameter Expansion)
** 	command substitution (see Command Substitution)
** 	arithmetic expansion (see Arithmetic Expansion)
** 	Field splitting (see Field Splitting)
** 		shall be performed on the portions of
** 		the fields generated by the above, unless IFS is null.
** 	Pathname expansion (see Pathname Expansion) shall be performed,
** 		unless set -f is in effect.
** 	Quote removal (see Quote Removal) shall always be performed last.
** The expansions described in this section shall occur in the same shell
** environment as that in which the command is executed.
**
** If the complete expansion appropriate for a word results in an empty field,
** that empty field shall be deleted from the list of fields that form the
** completely expanded command, unless the original word contained single-quote
** or double-quote characters.
**
** The '$' character is used to introduce parameter expansion, command
** substitution, or arithmetic evaluation. If an unquoted '$' is followed by
** a character that is not one of the following:
**
** A numeric character
** The name of one of the special parameters (see Special Parameters)
** A valid first character of a variable name
** A <left-curly-bracket> ( '{' )
** A <left-parenthesis>
** the result is unspecified.
*/

int		full_word_expansion(char ***new, char *old)
{
	int		status;
	char	*parameter;
	char	**fields;

	fields = NULL;
	parameter = old;
	if (OK((status = tilde_expansion(&parameter)))
		&& OK((status = parameter_expansion(&parameter)))
		&& OK((status = command_substitution(&parameter)))
		&& OK((status = arithmetic_expansion(&parameter)))
		&& OK((status = field_splitting(&fields, parameter)))
		&& OK((status = pathname_expansion(&fields)))
		&& OK((status = quote_removal(&fields))))
		*new = fields;
	return (status);
}

/*
** In each case that a value of word is needed (based on the state of parameter,
** as described below), word shall be subjected to tilde expansion, parameter
** expansion, command substitution, and arithmetic expansion.
*/

int		sub_expansion(char **new, char *old)
{
	int		status;
	char	*parameter;

	parameter = old;
	if (OK((status = tilde_expansion(&parameter)))
		&& OK((status = parameter_expansion(&parameter)))
		&& OK((status = command_substitution(&parameter)))
		&& OK((status = arithmetic_expansion(&parameter))))
		*new = parameter;
	return (status);
}
