/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 14:29:05 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/29 17:49:15 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRAVERSE_H
# define TRAVERSE_H

# define DEL 0x7f
# define INTR 0x03
# define EOT 0x04
# define EXIT_OK(x) (x == 0)
# define IS_DONE(state, c) (state[STATE] == state[ACCEPT] && buf == '\n')
# define IS_SIG(state) (state[STATE] == state[SIG])

enum			e_here_end_dfa
{
	ACCEPT,
	SIG,
	STATE,
	LAST_STATE,
	CHARS
};

typedef struct	s_ectx
{
	int			in_fd;
	int			out_fd;
	int			err_fd;
	int			orig_in;
	int			orig_out;
	int			orig_err;
	int			is_bg;
	int			exit_code;
}				t_ectx;

/*
** src/traverse/affixes.c
*/

int				process_prefix(t_ectx *e_ctx, char ***command, t_ast_node *root);
int				process_suffix(t_ectx *e_ctx, char ***command, t_ast_node *root);

/*
** src/traverse/command.c
*/

int				execute_cmd(char **command);

/*
** src/traverse/here_end_utils.c
*/

int				create_here_end_dfa(char *here_end, int ***dfa, int dfa_state[5]);
int				prep_here_end(struct termios *ttys);
int				restore_here_end(struct termios *tty);

/*
** src/traverse/here_end.c
*/

int				process_io_here(int io_num, t_ectx *e_ctx, t_ast_node *root);

/*
** src/traverse/pipe.c
*/

int				exec_pipeline(t_ectx *e_ctx, t_ast_node *root);

/*
** src/traverse/separator.c
*/

int				separator_op_is_linebreak(int *is_bg, t_ast_node *root);
int				exec_separator(int *is_bg, t_ast_node *root);

/*
** src/traverse/simple_command.c
*/

int				exec_simple_command(t_ectx *e_ctx, t_ast_node *root);

/*
** src/traverse/traverse_uitls.c
*/

void			throw_redir_err(void);
void			set_exec_ctx(t_ectx *e_ctx);
void			reset_exec_ctx(t_ectx *e_ctx);
#endif
