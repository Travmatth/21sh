/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 17:03:30 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/03 17:33:13 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Tilde Expansion
** A "tilde-prefix" consists of an unquoted <tilde> character at the beginning
** of a word, followed by all of the characters preceding the first unquoted
** <slash> in the word, or all the characters in the word if there is no
** <slash> ... The tilde-prefix is replaced by the value of the variable HOME.
** The pathname resulting from tilde expansion shall be treated as if quoted to
** prevent it being altered by field splitting and pathname expansion.
*/

char	ft_quotestr(char *str)
{
	size_t	len;
	char	*new;

	if (!str)
		return (NULL);
	len = LEN(str, 0);
	if (!(new = ft_strnew(len + 2)))
		return (NULL);
	new[0] = '"';
	return (ft_strcat(ft_strcat(new, str), "\""));
}

int		tilde_expansion(char **parameter)
{
	char	*str;
	char	*var;
	char	*tmp;

	if (TILDE_PREFIX(*parameter) && !(str = ft_quotestr(ft_strdup(*parameter))))
		return (ERROR);
	else if (TILDE_PREFIX(*parameter))
	{
		if (!(var = get_env_var("HOME"))
			|| !(tmp = ft_swap(str, "~", var)))
			return (NIL);
		free(str);
		str = tmp;
	}
	else if (!TILDE_PREFIX(*parameter) && !(str = ft_strdup(*parameter)))
		return (ERROR);
	free(*parameter);
	*parameter = str;
	return (SUCCESS);
}

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

int		alternative_param_expansion(char **parameter, char *var)
{
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

int		use_defaults_param_expansion(char **parameter, char *var)
{
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

int		assign_defaults_param_expansion(char **parameter, char *var)
{
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

int		error_unset_param_expansion(char **parameter, char *var)
{
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

int		alternative_param_expansion(char **parameter, char *var)
{
}

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

int		enclosed(char *str, char c)
{
	char	*occurs;
	char	*close;

	occurs = ft_strchr(str, c);
	if (!(occurs))
		return (NIL);
	close = ft_strchr(str, '}');
	return (occurs < close);
}

int		parameter_expansion(char **parameter)
{
	char	*new;
	char	*param;
	int		status;

	if (!(param = ft_strdup(*parameter))
		|| !(new = ft_strdup(*parameter)))
		return (ERROR);
	status = SUCCESS;
	while (OK(status) && (param = ft_strchr(param, '$')))
	{
		if (!ft_strncmp("$(", param, 2))
			continue ;
		else if (enclosed(param, '-'))
			status = use_defaults_param_expansion(&new, param);
		else if (enclosed(param, '='))
			status = assign_defaults_param_expansion(&new, param);
		else if (enclosed(param, '?'))
			status = error_unset_param_expansion(&new, param);
		else if (enclosed(param, '+'))
			status = alternative_param_expansion(&new, param);
		else
			status = plain_param_expansion(&new, param);
	}
	free(tmp);
	free(*parameter);
	*parameter = new;
	return (status);
}

/*
** Command Substitution
** Command substitution allows the output of a command to be substituted in
** place of the command name itself. Command substitution shall occur when
** the command is enclosed as follows:
**
** $(command)
**
** or (backquoted version):
**
** `command`
**
** The shell shall expand the command substitution by executing command in a
** subshell environment (see Shell Execution Environment) and replacing the
** command substitution (the text of command plus the enclosing "$()" or
** backquotes) with the standard output of the command, removing sequences of
** one or more <newline> characters at the end of the substitution. Embedded
** <newline> characters before the end of the output shall not be removed;
** however, they may be treated as field delimiters and eliminated during field
** splitting, depending on the value of IFS and quoting that is in effect. If
** the output contains any null bytes, the behavior is unspecified.
**
** Within the backquoted style of command substitution, <backslash> shall retain
** its literal meaning, except when followed by: '$', '`', or <backslash>. The
** search for the matching backquote shall be satisfied by the first unquoted
** non-escaped backquote; during this search, if a non-escaped backquote is
** encountered within a shell comment, a here-document, an embedded command
** substitution of the $(command) form, or a quoted string, undefined results
** occur. A single-quoted or double-quoted string that begins, but does not end,
** within the "`...`" sequence produces undefined results.
**
** With the $(command) form, all characters following the open parenthesis to
** the matching closing parenthesis constitute the command. Any valid shell
** script can be used for command, except a script consisting solely of
** redirections which produces unspecified results.
**
** The results of command substitution shall not be processed for further tilde
** expansion, parameter expansion, command substitution, or arithmetic
** expansion. If a command substitution occurs inside double-quotes, field
** splitting and pathname expansion shall not be performed on the results of
** the substitution.
**
** Command substitution can be nested. To specify nesting within the backquoted
** version, the application shall precede the inner backquotes with <backslash>
** characters; for example:
**
** \`command\`
**
** The syntax of the shell command language has an ambiguity for expansions
** beginning with "$((", which can introduce an arithmetic expansion or a
** command substitution that starts with a subshell. Arithmetic expansion has
** precedence; that is, the shell shall first determine whether it can parse the
** expansion as an arithmetic expansion and shall only parse the expansion as a
** command substitution if it determines that it cannot parse the expansion as
** an arithmetic expansion. The shell need not evaluate nested expansions when
** performing this determination. If it encounters the end of input without
** already having determined that it cannot parse the expansion as an arithmetic
** expansion, the shell shall treat the expansion as an incomplete arithmetic
** expansion and report a syntax error. A conforming application shall ensure
** that it separates the "$(" and '(' into two tokens (that is, separate them
** with white space) in a command substitution that starts with a subshell.
** For example, a command substitution containing a single subshell could be
** written as:
** $( (command) )
*/

int		command_substitution(char **parameter)
{
}

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

int		arithmetic_expansion(char **parameter)
{
}

/*
** Field Splitting
** After parameter expansion (Parameter Expansion), command substitution
** (Command Substitution), and arithmetic expansion (Arithmetic Expansion), the
** shell shall scan the results of expansions and substitutions that did not
** occur in double-quotes for field splitting and multiple fields can result.
**
** The shell shall treat each character of the IFS as a delimiter and use the
** delimiters as field terminators to split the results of parameter expansion,
** command substitution, and arithmetic expansion into fields.
**
** If the value of IFS is a <space>, <tab>, and <newline>, or if it is unset,
** any sequence of <space>, <tab>, or <newline> characters at the beginning or
** end of the input shall be ignored and any sequence of those characters
** within the input shall delimit a field. For example, the input:
**
** <newline><space><tab>foo<tab><tab>bar<space>
**
** yields two fields, foo and bar.
**
** If the value of IFS is null, no field splitting shall be performed.
** Otherwise, the following rules shall be applied in sequence. The term "IFS
** white space" is used to mean any sequence (zero or more instances) of
** white-space characters that are in the IFS value (for example, if IFS
** contains <space>/ <comma>/ <tab>, any sequence of <space> and <tab>
** characters is considered IFS white space).
**
** IFS white space shall be ignored at the beginning and end of the input.
**
** Each occurrence in the input of an IFS character that is not IFS white space,
** along with any adjacent IFS white space, shall delimit a field, as described
** previously.
**
** Non-zero-length IFS white space shall delimit a field.
*/

int		field_splitting(char ***fields, char *parameter)
{
}

/*
** Pathname Expansion
** After field splitting, if set -f is not in effect, each field in the
** resulting command line shall be expanded using the algorithm described in
** Pattern Matching Notation, qualified by the rules in Patterns Used for
** Filename Expansion.
*/

int		pathname_expansion(char ***fields)
{
}

/*
** Quote Removal
** The quote characters (<backslash>, single-quote, and double-quote) that were
** present in the original word shall be removed unless they have
** themselves been quoted.
*/

int		quote_removal(char ***fields)
{
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

	if (OK((status = tilde_expansion(&parameter)))
		|| OK((status = parameter_expansion(&parameter)))
		|| OK((status = command_substitution(&parameter)))
		|| OK((status = arithmetic_expansion(&parameter)))
		|| OK((status = field_splitting(&fields, parameter)))
		|| OK((status = pathname_expansion(&fields)))
		|| OK((status = quote_removal(&fields))))
		*new = fields;
	return (status);
}
