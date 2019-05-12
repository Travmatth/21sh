/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 17:07:20 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/11 16:05:36 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

#ifndef LIBFT_H
# include "../libftprintf/srcs/includes/ft_printf.h"
#endif

# include "semantic_analysis.h"

# define NEW_FILE_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

# define NEGATE(x, y) (x ? !y : y)

#define WAITPID_ERR "Runtime Error: forked process %d exited with signal: %s\n"

/*
** When a bash process exits normally, return value is 0
** When exiting because of an error, return value is 1
*/

# define NORMAL_CHILD_EXIT 0
# define ERROR_CHILD_EXIT 1
# define IS_NORMAL_CHILD_EXIT(x) (x == 0)
# define IS_ERROR_CHILD_EXIT(x) (x == 1)

/*
** When creating pipes, fd[0] is read end of pipe and fd[1] is write end
*/

# define PIPE_READ 0
# define PIPE_WRITE 1

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
** src/execution/execute.c
*/

int		execute_switch(t_exec_node *node);
int		parse_execute_input(char *complete_cmd);

/*
** src/execution/here_end.c
*/

int		manage_del(int **dfa, int dfa_state[5], char **read_buf);
int		manage_read(int dfa_state[5], char buf, char **read_buf);
int		read_until_here_end(char *here_end, int **dfa, int dfa_state[5], int fd);
int		swap_here_end_fd(int pipe_fds[2], int dfa_state[5], int io_num);

/*
** src/execution/operators.c
*/

/*
** src/execution/redirs.c
*/

int		open_redirs(t_simple *simple);
int		perform_redirs(t_simple *simple);

/*
** src/execution/execute_utils.c
*/

int		wait_loop(int pid, int *return_val);
void	free_program(t_program *program);
#endif
