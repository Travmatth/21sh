/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dysotoma <dysotoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/25 13:15:28 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/02 01:50:43 by dysotoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static t_history	*init_h_node(char *content)
{
	t_history	*new;

	if (!(new = (t_history*)ft_memalloc(sizeof(t_history))))
		return (NULL);
		/////////////////////////////////////
		// best to leav this a regular strdup for less memory usage
		/////////////////////////////////////
	new->content = ft_strndup(content, INPUT_LEN);
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

void				write_to_history(char **line, t_interface *interface)
{
	int	i;

	i = 0;
	if (**line && (!interface->h_list.hst ||
					ft_strcmp(*line, interface->curr_last_history) != 0))
	{
		// ft_printf("line_len: %i\n", interface->line_len);
		write(interface->h_list.fd, ":", 1);
		while (i < interface->line_len)
		{
			// ft_printf("line: %s\n", *line);
			// ft_printf("i: %i | *line[i]: %c\n", i, (*line)[i]);
			if ((*line)[i] == '\n')
				(*line)[i] = '\\';
			write(interface->h_list.fd, *line + i, 1);
			if ((*line)[i] == '\\' || i + 1 == interface->line_len)	
				write(interface->h_list.fd, "\n", 1);
			i++;
		}
	}
}

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
		while (1)
		{
			i = 0;
			ft_bzero(buf, 1);
			while (i < INPUT_LEN && read(h_list->fd, buf, 1))
			{
				buf[1] = '\0';
				if (buf[0] == ':' && (line[i] || ( i > 0 && line[i - 1])))
					break ;
				i > 0 && line[i - 1] == '\\' && buf[0] == '\n' ? i-- : 0;
				buf[0] == ':' || (line[i] != '\\' && buf[0] == '\n') ? 0 : (line[i++] = buf[0]);
				ft_bzero(buf, 1);
			}
			if ((push_history(&h_list->hst, line)) == 0)
				break ;
			ft_bzero(line, i);
		}
}
