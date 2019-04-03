/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 17:17:19 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/02 18:02:39 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		push_redir(t_simple_command *cmd, t_redir *redir)
{
	t_redir	*r;

	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		r = cmd->redirs;
		while (r && r->next)
			r = r->next;
		r->next = redir;
	}
	return (SUCCESS);
}

int		get_filename(char **filename, t_ast_node *root)
{
	char	*name;

	name = NULL;
	if (!(name = ft_strdup(((t_ast_node*)root->val[0])->val[0])))
		return (ERROR);
	*filename = name;
	return (name && name[0] ? SUCCESS : NIL);
}

int		simple_redir(t_redir *redir, int io_num, char *filename, int type)
{
	char	*name;

	if (ERR(word_expansion(&name, filename)))
		return (NIL);
	redir->replacement = io_num;
	redir->word = name;
	redir->type = type;
	return (SUCCESS);
}

int		dup_redir(t_redir *redir, int io_num, char *filename, int type)
{
	char	*fd;

	if (ERR(word_expansion(&fd, filename)))
		return (ERROR);
	if (IS_A("-", fd))
		redir->word = fd;
	else if (IS_A("0", fd) && !ft_atoi(fd))
	{
		ft_printf("Error: invalid fd in [n]<&[fd]");
		free(fd);
		return (NIL);
	}
	else
	{
		redir->replacement = ft_atoi(fd);
		free(fd);
	}
	redir->original = io_num;
	redir->type = type;
	return (SUCCESS);
}

int		create_redir(t_redir **redir, t_ast_node *root, char **file)
{
	int		status;

	if (!(*redir = ft_memalloc(sizeof(t_redir)))
		|| !OK((status = get_filename(file, root->val[1]))))
		return (redir ? status : ERROR);
	(*redir)->original = ERROR;
	(*redir)->replacement = ERROR;
	(*redir)->type = ERROR;
	return (SUCCESS);
}
