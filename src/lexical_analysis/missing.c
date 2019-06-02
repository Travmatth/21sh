/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   missing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 13:21:17 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/02 13:10:51 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		prep_missing_terminal(struct termios tty[2]
							, char **line
							, char next[2]
							, int *len)
{
	ft_putstr("> ");
	*len = 0;
	next[1] = '\0';
	if (!(*line = ft_strnew(0))
		|| ERR(prep_terminal(tty, ~(ISIG | ICANON | ECHO))))
		return (ERROR);
	return (SUCCESS);
}

int		restore_missing_terminal(t_list **tokens
							, char *input
							, char *line
							, char *tmp)
{
	ft_putchar('\n');
	if (!(tmp = ft_strjoin(input, line)))
		return (ERROR);
	free(line);
	return (lexical_analysis(tmp, tokens));
}

void	add_missing_char(char **line, char *tmp, int *len, char c)
{
	free(*line);
	*line = tmp;
	*len += 1;
	ft_putchar(c);
}

int		manage_backspace(char *line, int *len)
{
	*len -= 1;
	line[*len] = '\0';
	return (write(STDIN, "\b \b", 3));
}

int		manage_missing_closures(char *input, t_list **tokens, t_list **missing)
{
	char			next[2];
	struct termios	tty[2];
	int				status;
	char			*new[2];
	int				len;

	clear_current_tokens(tokens, missing);
	status = prep_missing_terminal(tty, &new[LINE], next, &len);
	while (OK(status) && !ERR((status = read(STDIN, &next, 1))))
	{
		if (next[0] == INTR || next[0] == EOF)
			status = NIL;
		else if (next[0] == '\n')
			break ;
		else if (next[0] == DEL && len)
			status = ERR(manage_backspace(new[LINE], &len)) ? ERROR : status;
		else if (next[0] == DEL)
			continue ;
		else if (!(new[TMP] = ft_strjoin(new[LINE], next)))
			status = ERROR;
		else
			add_missing_char(&new[LINE], new[TMP], &len, next[0]);
	}
	return (ERR(restore_terminal(&tty[1])) ? ERROR
		: restore_missing_terminal(tokens, input, new[LINE], new[TMP]));
}
