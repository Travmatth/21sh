/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface_constructs.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/02 15:43:03 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/07 22:32:38 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERFACE_CONSTRUCTS_H
# define INTERFACE_CONSTRUCTS_H

# ifndef LIBFT_H
#  include "../libftprintf/srcs/includes/ft_printf.h"
# endif

/*
** If movement would result in invalid index position
*/

# define INVALID -2

/*
** Character sent by terminal line discipline in response to RETURN key
*/

# define RETURN 0x0a

/*
** Character sent by terminal line discipline in response to BACKSPACE key
*/

# define DEL 0x7f

/*
** Character sent by terminal line discipline in response to ctrl-c
*/

# define INTR 0x03
# define COPY 0x03

/*
** Character sent by terminal line discipline in response to ctrl-d
*/

# define EOT 0x04

/*
** Character sent by terminal line discipline in response to ctrl-v key
*/

# define PASTE 0x16

/*
** Character sent by terminal line discipline in response to ctrl-x key
*/

# define CUT 0x18

/*
** Character sent by terminal line discipline in response to DELETE key
*/

# define DEL2 0x7E335B1B

/*
** Character sent by terminal line discipline in response to UP key
*/

# define UP 0x415B1B

/*
** Character sent by terminal line discipline in response to SHIFT_UP key
*/

# define SHIFT_UP 0x41323B315B1B

/*
** Character sent by terminal line discipline in response to CTL_UP key
*/

# define CTL_UP 0x41353B315B1B

/*
** Character sent by terminal line discipline in response to DOWN key
*/

# define DOWN 0x425B1B

/*
** Character sent by terminal line discipline in response to SHIFT_DOWN key
*/

# define SHIFT_DOWN 0x42323B315B1B

/*
** Character sent by terminal line discipline in response to CTL_DOWN key
*/

# define CTL_DOWN 0x42353B315B1B

/*
** Character sent by terminal line discipline in response to RIGHT key
*/

# define RIGHT 0x435B1B

/*
** Character sent by terminal line discipline in response to SHIFT_RIGHT key
*/

# define SHIFT_RIGHT 0x43323B315B1B

/*
** Character sent by terminal line discipline in response to CTL_RIGHT key
*/

# define CTL_RIGHT 0x43353B315B1B

/*
** Character sent by terminal line discipline in response to LEFT key
*/

# define LEFT 0x445B1B

/*
** Character sent by terminal line discipline in response to SHIFT_LEFT key
*/

# define SHIFT_LEFT 0x44323B315B1B

/*
** Character sent by terminal line discipline in response to CTL_LEFT key
*/

# define CTL_LEFT 0x44353B315B1B

/*
** Character sent by terminal line discipline in response to HOME key
*/

# define HOME 0x485B1B

/*
** Character sent by terminal line discipline in response to END key
*/

# define END 0x465B1B

/*
** The max Length of input
*/

# define INPUT_LEN 4096

/*
** Determine whether given character is a whitespace
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

# define IS_END(next, line, ui) (next == RETURN && !escaped(*line, ui->line_len)

# define ACCEPT_LINE(s, next, line, ui) (OK(s) && IS_END(next, line, ui)))

enum						e_line_exists
{
	PREV,
	NEXT
};

typedef struct s_history	t_history;

struct						s_history
{
	char					*content;
	t_history				*prev;
	t_history				*next;
};

typedef struct				s_h_list
{
	int						fd;
	char					*old;
	t_history				*hst;
}							t_h_list;

typedef struct				s_interface
{
	int						select;
	int						ccp_start;
	int						ccp_end;
	int						ccp_orig;
	int						line_len;
	int						line_index;
	struct termios			tty[2];
	struct winsize			ws;
	t_h_list				h_list;
	char					*curr_last_history;
}							t_interface;
#endif
