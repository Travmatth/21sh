/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 17:07:20 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/03 13:25:07 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# ifndef LIBFT_H
#  include "../libftprintf/srcs/includes/ft_printf.h"
# endif

# include "semantic_analysis.h"
# include "execution_node.h"

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
