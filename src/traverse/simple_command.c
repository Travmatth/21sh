/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 15:15:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/22 17:36:48 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	throw_redir_err(void)
{
}

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
		if (!ERR(e_ctx->orig_out))
		{
			dup2(e_ctx->orig_out, STDOUT);
			close(e_ctx->orig_out);
			e_ctx->out_fd = e_ctx->orig_out;
			e_ctx->orig_out = ERROR;
		}
		filename = ((t_ast_node*)((t_ast_node*)root->val[1])->val[0])->val[0];
		tmp_fd = ERR(io_num) ? e_ctx->out_fd : io_num;
		if (ERR((e_ctx->orig_out = dup(tmp_fd))))
			return (ERROR);
		close(e_ctx->out_fd);
		if (ERR((e_ctx->out_fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0666))))
			throw_redir_err();
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

int		process_here_end(char **here_end, t_ast_node *root)
{
	*here_end = NULL;
	if (ft_strcmp("WORD", root->rhs))
		return (ERROR);
	*here_end = (char*)((t_ast_node*)root->val[0])->val[0];
	return (SUCCESS);
}

# define EOT 0x04
int		process_io_here(int io_num, t_ectx *e_ctx, t_ast_node *root)
{
	char			read_buf[10];
	char			*buf;
	char			*tmp_buf;
	struct termios	tty;
	struct termios	old_tty;
	char			*here_end;
	ssize_t			bytes;
	ssize_t			total;
	int				pipe_fds[2];

	if (ERR(process_here_end(&here_end, root->val[1])) || !(buf = ft_strnew(0)))
		return (ERROR);
	total = 0;
	tmp_buf = buf;
	if (!ft_strcmp("DLESS here_end", root->rhs))
	{
		if (ERR(pipe(pipe_fds)))
			return (ERROR);
		prep_terminal_here_end(&tty, &old_tty, e_ctx);
		while (TRUE)
		{
			ft_bzero(read_buf, 10);
			if (NONE((bytes = read(STDIN, read_buf, 9))) || read_buf[0] == EOT)
				break ;
			if (!(tmp_buf = ft_strjoin(buf, read_buf)))
				break ;
			total += bytes;
			free(buf);
			buf = tmp_buf;
			if (ft_strstr(buf, here_end))
				break ;
		}
		restore_terminal_here_end(&old_tty, e_ctx);
		if (!tmp_buf)
			return (ERROR);
		write(pipe_fds[1], buf, total);
		if (ERR(io_num))
		{
			if (ERR((e_ctx->orig_in = dup(e_ctx->in_fd))))
				return (ERROR);
			close(pipe_fds[1]);
			if (ERR((e_ctx->in_fd = dup2(pipe_fds[0], e_ctx->in_fd))))
				return (ERROR);
		}
		else
			dup2(pipe_fds[0], io_num);
		close(pipe_fds[0]);
	}
	else if (ft_strcmp("DLESSDASH here_end", root->rhs))
	{
		ft_printf("<<- not implemented");
		return (ERROR);
	}
	return (SUCCESS);
}

int		process_io_redirect(t_ectx *e_ctx, t_ast_node *root)
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
		return (process_io_here(io_num, e_ctx, root->val[0]));
	else if (!ft_strcmp("IO_NUMBER io_here", root->rhs))
	{
		if (ERR(process_io_number(&io_num, root->val[0])))
			return (ERROR);
		return (process_io_here(io_num, e_ctx, root->val[1]));
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
		return (process_io_redirect(e_ctx, root->val[0]));
	else if (!ft_strcmp("cmd_suffix io_redirect", root->rhs))
	{
		if (ERR(process_suffix(e_ctx, command, root->val[0])))
			return (ERROR);
		return (process_io_redirect(e_ctx, root->val[1]));
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
