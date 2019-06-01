/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   missing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 13:21:17 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/31 17:44:22 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Returns next closing character sequence needed to correctly close
** given command
*/

int		next_missing_symbol(t_list *missing)
{
	t_list	*node;
	int		type;

	if (!(node = ft_lstpeekhead(missing)))
		return (NIL);
	type = *(short*)node->content;
	return (type);
}

/*
** When given command has elements that are not properly closed, the sequence
** of quotes or command substitutions are saved so as to allow correct sequence
** of closing characters needed.
*/

int		push_missing_symbol(short type, t_list **missing)
{
	t_list	*node;

	if (!(node = ft_lstnew(&type, sizeof(short))))
		return (ERROR);
	ft_lstpushfront(missing, node);
	return (SUCCESS);
}

/*
** When needed closing character detected, it's corresponding opening sequence
** can be popped from the stack. When stack is empty command is correctly
** closed.
*/

int		pop_missing_symbol(t_list **missing, short *type)
{
	t_list	*node;

	node = ft_lsthead(missing);
	if (type)
		*type = *(short*)node->content;
	free(node->content);
	free(node);
	return (SUCCESS);
}

void	print_missing_sym(t_list *elem)
{
	short	sym;

	sym = *(short*)elem->content;
	if (sym == QUOTE)
		ft_putstr("QUOTE ");
	else if (sym == DQUOTE)
		ft_putstr("DQUOTE ");
	else if (sym == BRACE_SUB)
		ft_putstr("BRACE_SUB ");
	else if (sym == MATH_SUB)
		ft_putstr("MATH_SUB ");
	else if (sym == CMD_SUB)
		ft_putstr("CMD_SUB ");
	else if (sym == BQUOTE)
		ft_putstr("BQUOTE_SUB ");
	else if (sym == BACKSLASH_ESC)
		ft_putstr("BACKSLASH ");
}

/*
**
*/

int		manage_missing_closures(char *input,t_list **tokens, t_list **missing)
{
	char			next[2];
	struct termios	tty[2];
	int				status;
	char			*line;
	char			*tmp;
	int				len;

	ft_lstdel(tokens, del_token);
	*tokens = NULL;
	ft_lstiter(*missing, print_missing_sym);
	ft_lstdel(missing, del_missing);
	*missing = NULL;
	ft_putstr("> ");
	len = 0;
	next[1] = '\0';
	if (!(line = ft_strnew(0))
		|| ERR((status = prep_terminal(tty, ~(ISIG | ICANON | ECHO), 0, 0))))
		return (ERROR);
	while (OK(status) && !ERR((status = read(STDIN, &next, 1))))
	{
		if (next[0] == INTR || next[0] == EOF)
			status = NIL;
		else if (next[0] == '\n')
		{
			ft_putchar(next[0]);
			break ;
		}
		else if (next[0] == DEL && len)
		{
			line[--len] = '\0';
			status = ERR(write(STDIN, "\b \b", 3)) ? ERROR : status;
		}
		else if (next[0] == DEL)
			continue ;
		else if (!(tmp = ft_strjoin(line, next)))
			status = ERROR;
		else
		{
			free(line);
			line = tmp;
			len += 1;
			ft_putchar(next[0]);
		}
	}
	if (ERR(restore_terminal(&tty[1])) || ERR(status))
		return (ERROR);
	if (!(tmp = ft_strjoin(input, line)))
		return (ERROR);
	free(line);
	return (OK(status) ? lexical_analysis(tmp, tokens) : status);
}
