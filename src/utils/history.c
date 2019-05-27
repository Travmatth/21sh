/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dysotoma <dysotoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/25 13:15:28 by dysotoma          #+#    #+#             */
/*   Updated: 2019/05/27 01:24:45 by dysotoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static t_history	*init_h_node(char *content)
{
	t_history	*new;

	if (!(new = (t_history*)ft_memalloc(sizeof(t_history))))
		return (NULL);
	new->content = ft_strdup(content);
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

// void				clear_history(t_history *history)
// {
// 	t_history *tmp;
	
// 	if (history)
// 	{
// 		while (history->prev)
// 			history = history->prev;
// 		while (history)
// 		{
// 			tmp = history->next;
// 			free(history);
// 			history = tmp;
// 		}
			
// 	}
// }

// write_to_history()

int					push_history(t_history **history, char *content)
{
	t_history	*new;

	if (!content || !content[0])
		return (0);
	new = init_h_node(content);
	if (!*history)
		*history = new;
	else
	{
		new->prev = *history;
		(*history)->next = new;
		*history = new;
	}
	return (1);
}

void				init_history(t_h_list *h_list)
{
	int		i;
	char	buf[2];
	char	line[INPUT_LEN];
	
	ft_bzero(line, INPUT_LEN);
	if ((h_list->fd = open("21sh_history", O_RDWR | O_CREAT | O_APPEND,
														S_IRWXU)) > 0)
	{
		
		while (1)
		{
			i = 0;
			ft_bzero(buf, 1);
			while (i < INPUT_LEN && read(h_list->fd, buf, 1))
			{
				buf[1] = '\0';
			// ft_printf("inside loop\n");
			// ft_printf("inside push history: %s\n", buf);
				if (buf[0] == ':' && (line[i] || ( i > 0 && line[i - 1])))
				{
					// ft_printf("break triggered\n");
					break ;
				}
				i > 0 && line[i - 1] == '\\' && buf[0] == '\n' ? i-- : 0;
				buf[0] == ':' ? 0 : (line[i++] = buf[0]);
				ft_bzero(buf, 1);
			}
			// ft_printf("outside loop\n");
			if ((push_history(&h_list->history, line)) == 0)
				break ;
			ft_bzero(line, i);
		}
	}
}
