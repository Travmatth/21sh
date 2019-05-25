/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_substitution.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:38:44 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/22 15:48:05 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

/*
** 21sh does not support command substitution, so on detection of an command
** substitution shell should fail gracefully so that
** next command may be inputted
*/

int		cmd_sub_err(char **str, int start, int end)
{
	(void)str;
	(void)start;
	(void)end;
	if (ERR(end))
		return (NIL);
	else if (BACKTICK((*str), start) || CMD_SUB((*str), start))
	{
		ft_printf("Semantic Error: command substitution not implemented\n");
		return (NIL);
	}
	return (SUCCESS);
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
	int		i;
	size_t	end;
	int		state;
	char	*name;

	i = 0;
	if (!(name = ft_strdup(*parameter)))
		return (ERROR);
	state = SUCCESS;
	while (OK(state) && name[i])
	{
		if (name[i] == '\\')
			i += 1;
		else if ((BACKTICK(name, i)
			&& OK((state = backtick(&name, i, &end, cmd_sub_err))))
			|| (ARITH_EXP(name, i)
			&& OK((state = arith_exp(&name, i, &end, NULL))))
			|| (CMD_SUB(name, i)
			&& OK((state = command_sub(&name, i, &end, cmd_sub_err)))))
			i += end;
		i += 1;
	}
	free(*parameter);
	*parameter = name;
	return (state);
}
