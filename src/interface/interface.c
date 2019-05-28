/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dysotoma <dysotoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:42:31 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/28 00:27:36 by dysotoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

// static t_line *init(char c)
// {
// 	t_line	*new;

// 	if (!(new = (t_line*)ft_memalloc(sizeof(t_line))))
// 		return (NULL);
// 	get_cursor_position(&new->x, &new->y);
// 	new->c = c;
// 	new->highlight = 0;
// 	new->next = NULL;
// 	new->prev = NULL;
// 	return (new);
// }

static void insert(char c, char **line, t_interface *interface)
{
	ft_memmove((void*)(*line + interface->line_index + 1), (void*)(*line +
	interface->line_index),	INPUT_LEN - interface->line_index - 1);
	(*line)[interface->line_index] = c;
	interface->line_len++;
	interface->line_index++;
	if (interface->line_index != interface->line_len)
	{
		tputs(tgetstr("sc", NULL), 1, ft_termprint);
		tputs(tgetstr("cd", NULL), 1, ft_termprint);
		tputs(tgetstr("vi", NULL), 1, ft_termprint);
		ft_printf("%s\n", *line + interface->line_index - 1);
		tputs(tgetstr("rc", NULL), 1, ft_termprint);
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		tputs(tgetstr("ve", NULL), 1, ft_termprint);
	}
	else
		write(1, &c, 1);
}

static void delete(unsigned long c, char **line, t_interface *interface)
{
	if (c == DEL && line && interface->line_index > 0)
	{
		ft_memmove((void*)(*line + interface->line_index - 1), (void*)(*line +
		interface->line_index), INPUT_LEN - interface->line_index - 1);
		(*line)[interface->line_len] = '\0';
		interface->line_index--;
		interface->line_len--;
		tputs(tgetstr("le", NULL), 1, ft_termprint);
		ft_printf("%s%s%s%s%s%s", tgetstr("sc", NULL), tgetstr("vi", NULL),
		tgetstr("cd", NULL), *line + interface->line_index,
		tgetstr("rc", NULL), tgetstr("ve", NULL));
	}
	else if (c == DEL2 && line && interface->line_index != interface->line_len)
	{
		ft_memmove((void*)(*line + interface->line_index), (void*)(*line +
		interface->line_index + 1), INPUT_LEN - interface->line_index - 1);
		(*line)[interface->line_len] = '\0';
		interface->line_len--;
		ft_printf("%s%s%s%s%s%s", tgetstr("sc", NULL), tgetstr("vi", NULL),
		tgetstr("cd", NULL), *line + interface->line_index,
		tgetstr("rc", NULL), tgetstr("ve", NULL));
	}
}

void		history(unsigned long c, char **line, t_h_list *h_list,
t_interface *interface)
{
	
	if (h_list->hst)
	{
		if (c == UP && h_list->hst)
		{
			*line = h_list->hst->content;
			if (h_list->hst->prev)
				h_list->hst = h_list->hst->prev;
		}
		else if (c == DOWN && h_list->hst)
		{
			if (h_list->hst->next)
				h_list->hst = h_list->hst->next;
			*line = h_list->hst->next ? h_list->hst->content : h_list->old;
		}
		tputs(tgetstr("vi", NULL), 1, ft_termprint);
		while (interface->line_index > 0)
			movement(LEFT, line, interface);
		tputs(tgetstr("cd", NULL), 1, ft_termprint);
		ft_printf("%s", *line);
		tputs(tgetstr("ve", NULL), 1, ft_termprint);
		interface->line_len = ft_strlen(*line);
		interface->line_index = interface->line_len;
	}
}

/*
** Read single character from STDIN. Break reading on CTRL-C and delete input
** on BACKSPACE, otherwise append character to line and echo to STDOUT
*/

int		interface(char **line)
{
	static t_interface	interface;
	static char			tmp[INPUT_LEN];
	unsigned long		next;
	struct termios		tty[2];
	int					status;
	size_t				len;
	// t_uisegment		**segments;
	// t_uisegment		*current;
	int					in_word;

	interface.h_list.hst = NULL;
	init_history(&interface.h_list);
	len = 0;
	*line = tmp;
	status = prep_terminal(tty, ~(ICANON | ISIG | ECHO));
	// segments = NULL;
	in_word = FALSE;
	while (OK(status))
	{
		next = 0;
		read(STDIN, &next, 6);
		movement(next, line, &interface);
		// ft_printf("next = %#lx\n", next);
		// if char is newline or CTRL-C, end current line
		if (next == INTR || next == RETURN)
		{
			status = write(STDOUT, "\n", 1);
			ft_printf("%s\n", *line);
			write_to_history(line, &interface);
			close(interface.h_list.fd);
			push_history(&interface.h_list.hst, *line);
			restore_terminal(&tty[1]);
			exit(0) ;
		}
		else if (next == DEL || next == DEL2)
			delete(next, line, &interface);
		if (PRINTABLE_CHAR(next))
			insert((char)next, line, &interface);
		// else add char to line and STDOUT
		// else if ((tmp = ft_strjoin(*line, next)))
		// {
		// 	// if char is start of new word, add new segment to chain
		// 	if (!in_word && PRINTABLE_CHAR(next) && !IS_WHITESPACE(next))
		// 	{
		// 		if (current)
		// 			current->end = len - 1; 
		// 		current = new_segment();
		// 		add_segment(segments, current);
		// 		current->start = len;
		// 		in_word = TRUE;
		// 		if (ERR((status = get_cursor_position(&current->x, &current->y))))
		// 			break ;
		// 	}
		// 	// if char is whitespace and not in word, add blank segment to chain
		// 	if (!in_word && PRINTABLE_CHAR(next) && !IS_WHITESPACE(next))
		// 	{
		// 		if (current)
		// 			current->end = (int)len - 1; 
		// 		current = new_segment();
		// 		add_segment(segments, current);
		// 		current->start = (int)len;
		// 		if (ERR((status = get_cursor_position(&current->x, &current->y))))
		// 			break ;
		// 	}
			// if in word and char is not whitespace, continue current word
			// if (in_word && PRINTABLE_CHAR(next) && !IS_WHITESPACE(next))
			// {
			// }
			// if in word and char is whitespace, end current word
			// if (in_word && PRINTABLE_CHAR(next) && IS_WHITESPACE(next))
			// {
			// 	current->word_end = len - current->start;
			// 	in_word = FALSE;
			// }

			// if char is escaped newline, write prompt 
			// if (next == RETURN && len && (*line)[len - 1] == '\\')
			// {
			// 	status = ERR(write(STDOUT, "\n> ", 3)) ? ERROR : status;
			// 	len = 0;
			// }
			// else
			// 	free(*line);
			// *line = tmp;
			// len += 1;
			
		// }
		// else
		// 	status = ERROR;
	}
	return (ERR(restore_terminal(&tty[1])) ? ERROR : status);
}

















// int		interface(char **line)
// {
// 	char			*tmp;
// 	char			next[2];
// 	struct termios	tty[2];
// 	int				status;
// 	size_t			len;
// 	t_uisegment		**segments;
// 	t_uisegment		*current;
// 	int				in_word;

// 	len = 0;
// 	next[1] = '\0';
// 	if (!*line && !(*line = ft_strnew(0)))
// 		return (ERROR);
// 	status = prep_terminal(tty, ~(ICANON | ISIG | ECHO));
// 	segments = NULL;
// 	in_word = FALSE;
// 	while (OK(status) && !ERR((status = read(STDIN, &next, 1))))
// 	{
// 		// if char is newline or CTRL-C, end current line
// 		if (next[0] == INTR || next[0] == RETURN)
// 		{
// 			status = write(STDOUT, "\n", 1);
// 			break ;
// 		}
// 		// if char is delete, remove last char from line and STDOUT
// 		else if (next[0] == DEL && len)
// 		{
// 			(*line)[--len] = '\0';
// 			status = ERR(write(STDIN, "\b \b", 3)) ? ERROR : status;
// 		}
// 		else if (next[0] == DEL && !len)
// 			continue ;
// 		// else add char to line and STDOUT
// 		else if ((tmp = ft_strjoin(*line, next)))
// 		{
// 			// if char is start of new word, add new segment to chain
// 			if (!in_word && PRINTABLE_CHAR(next[0]) && !IS_WHITESPACE(next[0]))
// 			{
// 				if (current)
// 					current->end = len - 1; 
// 				current = new_segment();
// 				add_segment(segments, current);
// 				current->start = len;
// 				in_word = TRUE;
// 				if (ERR((status = get_cursor_position(&current->x, &current->y))))
// 					break ;
// 			}
// 			// if char is whitespace and not in word, add blank segment to chain
// 			if (!in_word && PRINTABLE_CHAR(next[0]) && !IS_WHITESPACE(next[0]))
// 			{
// 				if (current)
// 					current->end = (int)len - 1; 
// 				current = new_segment();
// 				add_segment(segments, current);
// 				current->start = (int)len;
// 				if (ERR((status = get_cursor_position(&current->x, &current->y))))
// 					break ;
// 			}
// 			// if in word and char is not whitespace, continue current word
// 			// if (in_word && PRINTABLE_CHAR(next[0]) && !IS_WHITESPACE(next[0]))
// 			// {
// 			// }
// 			// if in word and char is whitespace, end current word
// 			if (in_word && PRINTABLE_CHAR(next[0]) && IS_WHITESPACE(next[0]))
// 			{
// 				current->word_end = len - current->start;
// 				in_word = FALSE;
// 			}

// 			// if char is escaped newline, write prompt 
// 			if (next[0] == RETURN && len && (*line)[len - 1] == '\\')
// 			{
// 				status = ERR(write(STDOUT, "\n> ", 3)) ? ERROR : status;
// 				len = 0;
// 			}
// 			else
// 				status = ERR(write(STDOUT, next, 1)) ? ERROR : status;
// 			free(*line);
// 			*line = tmp;
// 			len += 1;
			
// 		}
// 		else
// 			status = ERROR;
// 	}
// 	return (ERR(restore_terminal(&tty[1])) ? ERROR : status);
// }
