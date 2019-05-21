/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dysotoma <dysotoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:40:47 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/20 23:01:18 by dysotoma         ###   ########.fr       */
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
	int				line_len;
	int				line_index;
	struct termios	tty_old;
	struct termios	tty_new;
	
}				t_interface;

/*
** src/interface/interface.c
*/

int						interface(char **line);

/*
** src/interface/utils.c
*/

t_uisegment				*new_segment(void);
void					add_segment(t_uisegment **segments, t_uisegment *seg);
int						get_cursor_position(int *x, int *y);
#endif
