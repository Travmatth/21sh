/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 22:02:30 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/07 22:19:38 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_history	*init_h_node(char *content)
{
	t_history	*new;

	if (!(new = (t_history*)ft_memalloc(sizeof(t_history))))
		return (NULL);
	new->content = ft_strdup(content);
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void		init_history(t_h_list *h_list)
{
	int		i;
	char	buf[2];
	char	line[INPUT_LEN];

	ft_bzero(line, INPUT_LEN);
	if ((h_list->fd = open("21sh_history", O_RDWR | O_CREAT | O_APPEND
					, S_IRWXU)) > 0)
		while (1)
		{
			i = 0;
			ft_bzero(buf, 1);
			while (i < INPUT_LEN && read(h_list->fd, buf, 1))
			{
				buf[1] = '\0';
				if (buf[0] == ':' && (line[i] || (i > 0 && line[i - 1])))
					break ;
				i > 0 && line[i - 1] == '\\' && buf[0] == '\n' ? i-- : 0;
				if (buf[0] != ':')
					line[i++] = buf[0];
				ft_bzero(buf, 1);
			}
			if ((push_history(&h_list->hst, line)) == 0)
				break ;
			ft_bzero(line, i);
		}
}
