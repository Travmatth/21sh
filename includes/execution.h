/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 17:07:20 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/25 14:59:55 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# ifndef LIBFT_H
# include "../libftprintf/srcs/includes/ft_printf.h"
# endif

# include "semantic_analysis.h"
# include "execution_node.h"

# define NEW_FILE_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
# define NEGATE(x, y) (x ? !y : y)
# define WAITPID_ERR "Runtime Error: forked process %d exited with signal: %s\n"

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

enum	e_here_end_buf
{
	HERE_END_BUF,
	IN_BUF
};

enum	e_here_end_dfa
{
	ACCEPT,
	SIG,
	STATE,
	LAST_STATE,
	CHARS
};

/*
** src/execution/dup_redirs.c
*/

int		redir_input_dup(t_redir *redir);
int		redir_out_dup(t_redir *redir);

/*
** src/execution/execute.c
*/

int		execute_switch(t_exec_node *node);
int		execute(t_program *program);
int		parse_execute_input(char *complete_cmd);

/*
** src/execution/execute_nodes.c
*/

int		exec_command(t_simple *simple, int should_exit);
int		exec_simple_command(t_simple *simple);
void	exec_pipe_parent_process(t_pipe *pipe_node, int pid, int pipe_fd[2]);
int		exec_pipe(t_pipe *pipe_node);
int		exec_logical(t_operator *operator);

/*
** src/execution/execute_utils.c
*/

int		wait_loop(int pid, int *return_val);
void	free_redirs(t_redir *redir);
void	free_exec_node(t_exec_node *node);
void	free_program(t_program *program);

/*
** src/execution/heredoc_redirs.c
*/

int		redir_heredoc_dash(t_redir *redir);
int		redir_heredoc(t_redir *redir);

/*
** src/execution/orig_redirs.c
*/

int		redir_out(t_redir *redir);
int		redir_input(t_redir *redir);
int		redir_out_append(t_redir *redir);
int		redir_inout(t_redir *redir);
int		redir_clobber(t_redir *redir);

/*
** src/execution/redirs.c
*/

int		redirs_switch(t_redir *redir);
int		open_redirs(t_simple *simple);
int		perform_redirs(t_simple *simple);
#endif
