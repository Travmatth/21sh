/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arithmetic_expansion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:40:09 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/05 18:43:43 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

/*
** Arithmetic Expansion
** Arithmetic expansion provides a mechanism for evaluating an arithmetic
** expression and substituting its value. The format for arithmetic expansion
** shall be as follows:
**
** $((expression))
**
** The expression shall be treated as if it were in double-quotes, except that
** a double-quote inside the expression is not treated specially. The shell
** shall expand all tokens in the expression for parameter expansion, command
** substitution, and quote removal.
**
** Next, the shell shall treat this as an arithmetic expression and substitute
** the value of the expression. The arithmetic expression shall be processed
** according to the rules given in Arithmetic Precision and Operations, with
** the following exceptions:
**
** Only signed long integer arithmetic is required.
**
** Only the decimal-constant, octal-constant, and hexadecimal-constant constants
** specified in the ISO C standard, Section 6.4.4.1 are required to be
** recognized as constants.
**
** The sizeof() operator and the prefix and postfix "++" and "--" operators are
** not required.
**
** Selection, iteration, and jump statements are not supported.
**
** All changes to variables in an arithmetic expression shall be in effect after
** the arithmetic expansion, as in the parameter expansion "${x=value}".
**
** If the shell variable x contains a value that forms a valid integer constant,
** optionally including a leading <plus-sign> or <hyphen-minus>, then the
** arithmetic expansions "$((x))" and "$(($x))" shall return the same value.
**
** As an extension, the shell may recognize arithmetic expressions beyond
** those listed. The shell may use a signed integer type with a rank larger
** than the rank of signed long. The shell may use a real-floating type
** instead of signed long as long as it does not affect the results in cases
** where there is no overflow. If the expression is invalid, or the contents
** of a shell variable used in the expression are not recognized by the shell,
** the expansion fails and the shell shall write a diagnostic message to
** standard error indicating the failure.
*/

int		process_arithmetic_expansion(char *cmd, int *skip)
{
	int		i;
	int		status;
	int		prefixed;

	i = 0;
	prefixed = 0;
	status = SUCCESS;
	while (OK(status) && cmd[i])
	{
		if (cmd[i] == '$')
			prefixed += 1;
		else if (prefixed == 1 && cmd[i] == '(')
			prefixed += 1;
		else if (prefixed == 2 && cmd[i] == '(')
		{
			status = process_arithmetic_expansion(&cmd[i], skip);
		}
		else if (cmd[i] == ')')
		{
			ft_printf("Semantic Error: Arithmetic Expansion not implemented");
			return (NIL);
		}
		else
			i += 1;
	}
	*skip = *skip + i;
	return (status);
}

int		arithmetic_expansion(char **parameter)
{
	int		i;
	int		status;
	int		prefixed;
	char	*param;

	if (!(param = ft_strdup(*parameter)))
		return (ERROR);
	i = 0;
	prefixed = 0;
	status = SUCCESS;
	while (OK(status) && param[i])
	{
		if (param[i] == '$' && (i += 1))
			prefixed += 1;
		else if (prefixed == 1 && param[i] == '(' && (i += 1))
			prefixed += 1;
		else if (prefixed == 2 && param[i] == '(')
		{
			if (!OK((status = process_arithmetic_expansion(&param[i], &i))))
				break ;
		}
		else if ((i += 1))
			prefixed = 0;
	}
	free(param);
	return (status);
}
