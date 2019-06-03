/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dysotoma <dysotoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:40:47 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/02 01:27:13 by dysotoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_H
# define INTERFACE_H

#ifndef LIBFT_H
# include "../libftprintf/srcs/includes/ft_printf.h"
#endif

/*
** #include <Holy_Spirit>
*/

/*
** The max Length of input
*/

#define INPUT_LEN 4096

# define IS_WHITESPACE(x) ((x == ' ' || x == '\t' || x == '\n'))

/*
** Ascii characters that will display on the tty
*/

# define PRINTABLE_CHAR(x) (x >= 32 && x <= 126)

/*
** String command used to query tty for cursor coordinates
*/

# define CURSOR_POS_REQ "\033[6n"

/*
** Data structure used to manage the position of the cursor, every segment
** is composed of a word and leading / trailing whitespaces.
*/

typedef struct s_uisegment t_uisegment;
struct			s_uisegment
{
	t_uisegment	*next;
	t_uisegment	*prev;
	int					x;
	int					y;
	int					start;
	int					end;
	int					word_end;
}						;


typedef struct s_history	t_history;
struct			s_history
{
	char		*content;
	t_history	*prev;
	t_history	*next;
};

typedef struct s_h_list	t_h_list;
struct			s_h_list
{
	int			fd;
	char		*old;
	t_history	*hst;
};

// typedef struct s_line	t_line;
// struct			s_line
// {
// 	char	c;
// 	int		x;
// 	int		y;
// 	int		highlight;
// 	t_line	*next;
// 	t_line	*prev;
	
// };

typedef struct	s_interface
{
	// t_line			list[];
	char			*curr_last_history;
	int				line_row[1000];
	int				line_len;
	int				line_index;
	int				written;
	t_uisegment		*segments[INPUT_LEN / 2];
	t_uisegment		*current;
	struct termios	tty_old;
	struct termios	tty_new;
	struct winsize	ws;
	t_h_list		h_list;
}				t_interface;

// arr[]->len of row && can be offset by prompt

/*
** src/interface/interface.c
*/

int						interface(char **line);
void					movement(unsigned long c, char **line,
						t_interface *interface);
void					history(unsigned long c, char **line, t_h_list *h_list,
														t_interface *interface);

/*
** src/interface/utils.c
*/

t_uisegment				*new_segment(void);
void					add_segment(t_uisegment **segments, t_uisegment *seg);
int						get_cursor_position(int *x, int *y);

/*
** src/utils/history.c
*/

void					init_history(t_h_list *h_list);
int						push_history(t_history **history, char *content);
void					write_to_history(char **line, t_interface *interface);

#endif
