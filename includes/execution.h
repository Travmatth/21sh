/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic_analysis.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:29:05 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/31 16:29:07 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef semantic_analysis_H
# define semantic_analysis_H

#ifndef LIBFT_H
# include "../libftprintf/srcs/includes/ft_printf.h"
#endif

# include "semantic_analysis.h"

/*
** Character sent by terminal line discipline in response to BACKSPACE key
*/

# define DEL 0x7f

/*
** Character sent by terminal line discipline in response to ctrl-c
*/

# define INTR 0x03

/*
** Character sent by terminal line discipline in response to ctrl-d
*/

# define EOT 0x04
# define EXIT_OK(x) (x == 0)

/*
** Used in HEREDOC processing to detect signals sent to terminal
*/

# define IS_DONE(state, c) (state[STATE] == state[ACCEPT] && buf == '\n')
# define IS_SIG(state) (state[STATE] == state[SIG])

/*
** enums used by HEREDOC processing
*/

enum			e_here_end_buf
{
	HERE_END_BUF,
	IN_BUF
};

enum			e_here_end_dfa
{
	ACCEPT,
	SIG,
	STATE,
	LAST_STATE,
	CHARS
};

/*
** src/execution/exec.c
*/

int		builtin_command(char **argv);
int		execute_command(char **av);
int		execute(t_program *program);
int		execute_commands(char *complete_cmd);

/*
** src/execution/here_end.c
*/

int		manage_del(int **dfa, int dfa_state[5], char **read_buf);
int		manage_read(int dfa_state[5], char buf, char **read_buf);
int		read_until_here_end(char *here_end, int **dfa, int dfa_state[5], int fd);
int		swap_here_end_fd(int pipe_fds[2], int dfa_state[5], int io_num);

/*
** src/execution/here_end_utils.c
*/

char	**init_bufs(size_t len);
int		create_here_end_dfa(char *here_end, int ***dfa, int dfa_state[5]);
int		prep_here_end(struct termios *ttys);
int		restore_here_end(struct termios *tty);

/*
** src/execution/operators.c
*/

/*
** src/execution/redirs.c
*/
#endif
