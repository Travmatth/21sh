/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_constructs.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 22:50:37 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 22:53:53 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOTE_CONSTRUCTS_H
# define QUOTE_CONSTRUCTS_H

/*
** Used by quote management functions to give context pecific functionality to
** functions as they parse different quote types - lexical analysis needs to
** store unclosed quotes, word expansion needs to execute closed quoted strings
*/

typedef	int	(*t_quote)(char **str, int start, int end);

# define NEXT_BRACE(s, i) (s[i + 1] && s[i + 1] == '{')
# define IS_PAREN(s, i) (s[i] == '(')
# define PARENS(s, i) (IS_PAREN(s, i) && IS_PAREN(s, i + 1))
# define TWO_CLOSING(s, i) (s[i] == ')' && s[i + 1] == ')')
# define UPPER(x) ((x >= 'A' && x <= 'Z'))
# define IS_VAR_CHAR(x) (((UPPER(x)) || (x >= '0' && x <= '9') || x == '_'))
# define IS_SPACE(s, i) ((!s[i + 1] || IS_WHITESPACE(s[i + 1])))
# define IS_CURSH(s, i) ((s[i] == '{' && first_word(s, i) && IS_SPACE(s, i)))
# define IS_SUB(s, i) ((s[i] == '(' && first_word(s, i)))

# define SNGL_QUOTE(s, i) ((s[i] == '\''))
# define DBL_QUOTE(s, i) ((s[i] == '"'))
# define EPARAM_EXP(s, i) ((s[i] == '$' && (NEXT_BRACE(s, i))))
# define UPARAM_EXP(s, i) ((s[i] == '$' && (IS_VAR_CHAR(s[i + 1]))))
# define CMD_SUB(s, i) ((s[i] == '$' && IS_PAREN(s, i + 1)))
# define BACKTICK(s, i) ((s[i] == '`'))
# define BACKSLASH(s, i) ((s[i] == '\\'))
# define ARITH_EXP(s, i) ((s[i] == '$' && PARENS(s, i + 1) || PARENS(s, i)))

# define BS(s, in, f, i, j) (OK((s = backslash(in, i, j, f))))
# define SQ(s, in, f, i, j) (OK((s = quote(in, i, j, f))))
# define DQ(s, in, f, i, j) (OK((s = dbl_quote(in, i, j, f))))
# define BT(s, in, f, i, j) (OK((s = backtick(in, i, j, f))))
# define CE(s, in, f, i, j) (OK((s = command_sub(in, i, j, f))))
# define UPE(s, in, f, i, j) (OK((s = uparam_exp(in, i, j, f))))
# define EPE(s, in, f, i, j) (OK((s = eparam_exp(in, i, j, f))))
# define AE(s, in, f, i, j) (OK((s = arith_exp(in, i, j, f))))
# define SUB(s, in, f, i, j) (OK((s = subshell(in, i, j, f))))
# define CUR(s, in, f, i, j) (OK((s = cur_shell(in, i, j, f))))

# define P_BACKSLASH(s, in, f, i, j) (BACKSLASH((*in), i) && BS(s, in, f, i, j))
# define P_QUOTE(s, in, f, i, j) (SNGL_QUOTE((*in), i) && SQ(s, in, f, i, j))
# define P_DQUOTE(s, in, f, i, j) (DBL_QUOTE((*in), i) && DQ(s, in, f, i, j))
# define P_BACKTICK(s, in, f, i, j) (BACKTICK((*in), i) && BT(s, in, f, i, j))
# define P_CMD(s, in, f, i, j) (CMD_SUB((*in), i) && CE(s, in, f, i, j))
# define P_UPARAM(s, in, f, i, j) (UPARAM_EXP((*in), i) && UPE(s, in, f, i, j))
# define P_EPARAM(s, in, f, i, j) (EPARAM_EXP((*in), i) && EPE(s, in, f, i, j))
# define P_ARITH(s, in, f, i, j) (ARITH_EXP((*in), i) && AE(s, in, f, i, j))
# define P_SUBSHELL(s, in, f, i, j) (IS_SUB((*in), i) && SUB(s, in, f, i, j))
# define P_CURSHELL(s, in, f, i, j) (IS_CURSH((*in), i) && CUR(s, in, f, i, j))

typedef struct	s_keyval
{
	int			type;
	char		*value;
}				t_keyval;
#endif

