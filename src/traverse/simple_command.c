/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 15:15:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/15 18:41:36 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	throw_redir_err(void)
{
}

/*
** The only difference between >&- and <&- is the default fd when not specified
** (>&- is 1>&- while <&- is 0<&-). Same for x>&y which is the same as x<&y
** except when x is not provided.
**
** The redirection operator
**		[n]<&word
** is used to duplicate input file descriptors. If word expands to one or
** more digits, the file descriptor denoted by n is made to be a copy of
** that file descriptor. If the digits in word do not specify a file
** descriptor open for input, a redirection error occurs. If word evalu‐
** ates to -, file descriptor n is closed. If n is not specified, the
** standard input (file descriptor 0) is used.
** 
** The operator
**		[n]>&word
** is used similarly to duplicate output file descriptors. If n is not
** specified, the standard output (file descriptor 1) is used. If the
** digits in word do not specify a file descriptor open for output, a re‐
** direction error occurs. As a special case, if n is omitted, and word
** does not expand to one or more digits, the standard output and standard
** error are redirected as described previously.
*/


int		process_io_file(int io_num, t_ectx *e_ctx, t_ast_node *root)
{
	int		tmp_fd;
	char	*filename;

	if (!ft_strcmp("< filename", root->rhs))
	{
		if (!ERR(e_ctx->orig_in))
		{
			dup2(e_ctx->orig_in, STDIN);
			close(e_ctx->orig_in);
			e_ctx->in_fd = e_ctx->orig_in;
			e_ctx->orig_in = ERROR;
		}
		if (!ERR(e_ctx->orig_err))
		{
			dup2(e_ctx->orig_err, STDERR);
			close(e_ctx->orig_err);
			e_ctx->err_fd = e_ctx->orig_err;
			e_ctx->orig_err = ERROR;
		}
		filename = ((t_ast_node*)((t_ast_node*)root->val[1])->val[0])->val[0];
		tmp_fd = ERR(io_num) ? e_ctx->in_fd : io_num;
		if (ERR((e_ctx->orig_in = dup(tmp_fd))))
			return (ERROR);
		close(e_ctx->in_fd);
		if (ERR((e_ctx->in_fd = open(filename, O_RDWR))))
			throw_redir_err();
	}
	else if (!ft_strcmp("LESSAND filename", root->rhs))
	{
		// if filename == '-', must close it
		ft_printf("'LESSAND filename' not implemented");
	}
	else if (!ft_strcmp("> filename", root->rhs))
	{
		if (!ERR(e_ctx->orig_out))
		{
			dup2(e_ctx->orig_out, STDOUT);
			close(e_ctx->orig_out);
			e_ctx->out_fd = e_ctx->orig_out;
			e_ctx->orig_out = ERROR;
		}
		if (!ERR(e_ctx->orig_err))
		{
			dup2(e_ctx->orig_err, STDERR);
			close(e_ctx->orig_err);
			e_ctx->err_fd = e_ctx->orig_err;
			e_ctx->orig_err = ERROR;
		}
		filename = ((t_ast_node*)((t_ast_node*)root->val[1])->val[0])->val[0];
		tmp_fd = ERR(io_num) ? e_ctx->out_fd : io_num;
		if (ERR((e_ctx->orig_out = dup(tmp_fd))))
			return (ERROR);
		close(e_ctx->out_fd);
		if (ERR((e_ctx->out_fd = open(filename, O_RDWR | O_CREAT, 0666))))
			throw_redir_err();
	}
	else if (!ft_strcmp("GREATAND filename", root->rhs))
	{
		// if filename == '-', must close it
		ft_printf("'GREATAND filename' not implemented");
	}
	else if (!ft_strcmp("DGREAT filename", root->rhs))
	{
		ft_printf("'DGREAT filename' not implemented");
	}
	else if (!ft_strcmp("LESSGREAT filename", root->rhs))
	{
		ft_printf("'LESSGREAT filename' not implemented");
	}
	else if (!ft_strcmp("CLOBBER filename", root->rhs))
	{
		ft_printf("'CLOBBER filename' not implemented");
	}
	return (SUCCESS);
}

int		process_io_number(int *io_num, t_ast_node *root)
{
	char	*io;

	io = (char*)root->val[0];
	if (!ft_strcmp("0", io))
		*io_num = 0;
	else
	{
		if (NONE((*io_num = ft_atoi(io))))
			return (ERROR);
	}
	return (SUCCESS);
}

int		process_io_here(int *io_num, t_ectx *e_ctx, char ***command, t_ast_node *root)
{
	(void)io_num;
	(void)e_ctx;
	(void)command;
	(void)root;
	return (SUCCESS);
}

int		process_io_redirect(t_ectx *e_ctx, char ***command, t_ast_node *root)
{
	int		io_num;

	io_num = ERROR;
	if (!ft_strcmp("io_file", root->rhs))
		return (process_io_file(io_num, e_ctx, root->val[0]));
	else if (!ft_strcmp("IO_NUMBER io_file", root->rhs))
	{
		if (ERR(process_io_number(&io_num, root->val[0])))
			return (ERROR);
		return (process_io_file(io_num, e_ctx, root->val[1]));
	}
	else if (!ft_strcmp("io_here", root->rhs))
		return (process_io_here(&io_num, e_ctx, command, root->val[0]));
	else if (!ft_strcmp("io_here WORD", root->rhs))
	{
		if (ERR(process_io_number(&io_num, root->val[0])))
			return (ERROR);
		return (process_io_here(&io_num, e_ctx, command, root->val[0]));
	}
	return (SUCCESS);
}

int		process_prefix(t_ectx *e_ctx, char ***command, t_ast_node *root)
{
	(void)e_ctx;
	(void)command;
	(void)root;
	return (SUCCESS);
}

char	**ft_strappend(char **strings, char *string)
{
	int		i;
	char	**new;

	if (!strings)
		return (NULL);
	else if (!string)
		return (strings);
	i = 0;
	while (strings[i])
		i += 1;
	if (!(new = (char**)ft_memalloc(sizeof(char*) * (i + 2))))
		return (NULL);
	new[i] = string;
	while (--i >= 0)
		new[i] = strings[i];
	return (new);
}

int		process_suffix(t_ectx *e_ctx, char ***command, t_ast_node *root)
{
	char	**new;

	if (!ft_strcmp("io_redirect", root->rhs))
		return (process_io_redirect(e_ctx, command, root->val[0]));
	else if (!ft_strcmp("cmd_suffix io_redirect", root->rhs))
	{
		if (ERR(process_suffix(e_ctx, command, root->val[0])))
			return (ERROR);
		return (process_io_redirect(e_ctx, command, root->val[1]));
	}
	else if (!ft_strcmp("WORD", root->rhs))
	{
		new = ft_strappend(*command, ((t_ast_node*)root->val[0])->val[0]);
		free(*command);
		*command = new;
	}
	else if (!ft_strcmp("cmd_suffix WORD", root->rhs))
	{
		if (ERR(process_suffix(e_ctx, command, root->val[0])))
			return (ERROR);
		new = ft_strappend(*command, ((t_ast_node*)root->val[1])->val[0]);
		free(*command);
		*command = new;
	}
	return (SUCCESS);
}

int		process_cmd(t_ectx *e_ctx, char ***command, t_ast_node *root)
{
	(void)e_ctx;
	if (!*command && !(*command = (char**)ft_memalloc(sizeof(char*) * 2)))
		return (ERROR);
	if (!((*command)[0] = ft_strdup(((t_ast_node*)root->val[0])->val[0])))
		return (ERROR);
	return (SUCCESS);
}

int		exec_simple_command(t_ectx *e_ctx, t_ast_node *root)
{
	char	**command;

	command = NULL;
	if (!ft_strcmp("cmd_prefix cmd_word cmd_suffix", root->rhs))
	{
		// not implemented
	}
	else if (!ft_strcmp("cmd_prefix cmd_word", root->rhs))
	{
		// not implemented
	}
	else if (!ft_strcmp("cmd_prefix", root->rhs))
	{
		// not implemented
	}
	else if (!ft_strcmp("cmd_name cmd_suffix", root->rhs))
	{
		if (ERR(process_cmd(e_ctx, &command, root->val[0])))
			return (ERROR);
		if (ERR(process_suffix(e_ctx, &command, root->val[1])))
			return (ERROR);
	}
	else if (!ft_strcmp("cmd_name", root->rhs))
	{
		if (ERR(process_cmd(e_ctx, &command, root->val[0])))
			return (ERROR);
	}
	e_ctx->exit_code = execute_cmd(command);
	reset_exec_ctx(e_ctx);
	return (SUCCESS);
}
