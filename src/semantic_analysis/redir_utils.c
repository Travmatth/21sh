/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 17:17:19 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/16 17:46:10 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Join a null terminated array of strings with given delimiter
*/

char	*ft_strarrjoin(char **arr, char delim)
{
	int		i;
	char	*tmp;
	char	*joined;

	i = 0;
	if (!(joined = ft_strnew(0)))
		return (NULL);
	while (arr[i])
	{
		tmp = ft_strjoin(joined, arr[i]);
		if (arr[i + 1])
		{
			if (!(joined = ft_strjoin(tmp, &delim)))
				return (NULL);
			free(tmp);
		}
		else
			joined = tmp;
		tmp = NULL;
		i += 1;
	}
	return (joined);
}

/*
** Push a given t_redir to the end of the list of t_redir's on the given command
*/

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

/*
** Extract the char *string contained within a child ast_node
*/

int		get_filename(char **filename, t_ast_node *root)
{
	char	*name;

	name = NULL;
	if (!(name = ft_strdup(((t_ast_node*)root->val[0])->val[0])))
		return (ERROR);
	*filename = name;
	return (name && name[0] ? SUCCESS : NIL);
}

/*
** Process given simple redirection operation to set t_redir, simple
** redirections do not duplicate an existing file descriptor
*/

int		simple_redir(t_redir *redir, int io_num, char *filename, int type)
{
	char	*name;
	char	**arr;

	if (ERR(full_word_expansion(&arr, filename))
		|| !(name = ft_strarrjoin(arr, ' ')))
		return (NIL);
	redir->replacement = io_num;
	redir->word = name;
	redir->type = type;
	return (SUCCESS);
}

/*
** Process given duplicate redirection operation to set t_redir, duplicate
** redirections duplicate an existing file descriptor
*/

int		dup_redir(t_redir *redir, int io_num, char *filename, int type)
{
	int		status;
	char	*fd;
	char	**arr;

	if (!OK((status = full_word_expansion(&arr, filename)))
		|| !(fd = ft_strarrjoin(arr, ' ')))
		return (OK(status) ? ERROR : status);
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

/*
** Create redirection struct that will set and pushed onto t_simple_command struct
*/

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
